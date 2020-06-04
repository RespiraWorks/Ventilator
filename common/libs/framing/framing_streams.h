#ifndef __FRAMING_STREAMS_H_
#define __FRAMING_STREAMS_H_

#include "checksum.h"
#include "framing_spec_chars.h"
#include <stdint.h>

constexpr int32_t END_OF_STREAM = -1;

enum ResponseFlags {
  STREAM_SUCCESS = 0,
  WARNING_BUFFER_FULL = 1,
  ERROR_BUFFER_FULL = 2,
  ERROR_STREAM_BROKEN = 4
};

// A response object returned from Stream.Put function
struct StreamResponse {
  // The amount of bytes written
  uint32_t count_written = 0;
  // Status flags
  uint32_t flags = 0;

  StreamResponse &operator+=(const StreamResponse &r) {
    this->count_written += r.count_written;
    this->flags |= r.flags;
    return *this;
  }
};

// Stream interface
class Stream {
public:
  // Puts a byte into the stream.
  // END_OF_STREAM is a special char that designates the end of the stream. We
  // use this for framing.
  virtual StreamResponse Put(int32_t b) = 0;
};

// OutputStream is connected to some output mechanism that knows how many bytes
// can be written to it at the moment
class OutputStream : public Stream {
  // Returns number of bytes that can be written.
public:
  virtual uint32_t BytesAvailableForWrite() = 0;
};

#define TO_BYTE(UI) static_cast<uint8_t>(UI & 0x000000FF)

// Passes bytes to the child Stream
// Calculates CRC while passing bytes
// Emits CRC to the child Stream upon receiving END_OF_STREAM
class CrcStream : public Stream {
  static constexpr uint32_t CRC_RESET_VALUE = 0xFFFFFFFF;
  Stream &output;
  uint32_t crc = CRC_RESET_VALUE;

public:
  CrcStream(Stream &os) : output(os){};

  StreamResponse Put(int32_t b) {
    StreamResponse r = {0, 0};
    if (END_OF_STREAM == b) {
      r = EmitCrcAndReset();
      r += output.Put(END_OF_STREAM);
      return r;
    } else {
      crc = crc32_single(crc, static_cast<uint8_t>(b));
      return output.Put(b);
    }
  }

private:
  StreamResponse EmitCrcAndReset() {
    StreamResponse r;
    r = output.Put(TO_BYTE(crc >> 24));
    r += output.Put(TO_BYTE(crc >> 16));
    r += output.Put(TO_BYTE(crc >> 8));
    r += output.Put(TO_BYTE(crc));
    crc = CRC_RESET_VALUE;
    return r;
  }
};

// Adds framing markers and escapes special chars
// Will emit FRAMING_MARK:
// * upon receiving END_OF_STREAM
// * next time Put called after receiving END_OF_STREAM
// Will emit FRAMING_ESC upon receiving FRAMING_MARK or FRAMING_ESC characters
class EscapeStream : public Stream {
  Stream &output;
  bool is_frame_done = true;

public:
  EscapeStream(Stream &os) : output(os){};

  StreamResponse Put(int32_t b) {
    StreamResponse r = {0, 0};
    switch (b) {
    case END_OF_STREAM:
      r = StartFrameIfDone();
      r += output.Put(FRAMING_MARK);
      r += output.Put(b);
      FrameDone();
      return r;
      break;
    case FRAMING_MARK:
    case FRAMING_ESC:
      r = StartFrameIfDone();
      r += output.Put(FRAMING_ESC);
      r += output.Put(b ^ 0x20);
      FrameStartedMaybe();
      return r;
      break;
    default:
      r = StartFrameIfDone();
      r += output.Put(b);
      FrameStartedMaybe();
      return r;
      break;
    };
  }

private:
  StreamResponse StartFrameIfDone() {
    return is_frame_done ? output.Put(FRAMING_MARK)
                         : (StreamResponse){0, STREAM_SUCCESS};
  }

  static bool shouldEscape(uint8_t b) {
    return FRAMING_MARK == b || FRAMING_ESC == b;
  }

  void FrameDone() {
    if (!is_frame_done) {
      is_frame_done = true;
    }
  }
  void FrameStartedMaybe() {
    if (is_frame_done) {
      is_frame_done = false;
    }
  }
};

// Just returns number of bytes written, designed to be used for evaluation of
// length of buffer being streamed
class CounterStream : public Stream {
public:
  StreamResponse Put(int32_t b) override {
    if (END_OF_STREAM == b) {
      return {0, STREAM_SUCCESS};
    } else {
      return {1, STREAM_SUCCESS};
    }
  }
};

#endif
