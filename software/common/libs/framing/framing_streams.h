#pragma once

#include "checksum.h"
#include "framing_spec_chars.h"
#include <stdint.h>

constexpr int32_t EndOfStream{-1};

enum class ResponseFlags {
  StreamSuccess = 0,
  WarningBufferFull = 1,
  ErrorBufferFull = 2,
  ErrorStreamBroken = 4
};

// A response object returned from Stream.Put function
struct StreamResponse {
  StreamResponse() = default;

  StreamResponse(uint32_t count, ResponseFlags flag)
      : count_written(count), flags(static_cast<uint32_t>(flag)) {}

  // The amount of bytes written
  uint32_t count_written{0};
  // Status flags
  uint32_t flags{0};

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
  // EndOfStream is a special char that designates the end of the stream. We
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

#define TO_BYTE(UI) static_cast<uint8_t>((UI)&0x000000FF)

// Passes bytes to the child Stream
// Calculates CRC while passing bytes
// Emits CRC to the child Stream upon receiving EndOfStream
class CrcStream : public Stream {

public:
  explicit CrcStream(Stream &os) : output_(os){};

  StreamResponse Put(int32_t b) override {
    StreamResponse ret;
    if (EndOfStream == b) {
      ret = EmitCrcAndReset();
      ret += output_.Put(EndOfStream);
      return ret;
    } else {
      crc_ = crc32_single(crc_, static_cast<uint8_t>(b));
      return output_.Put(b);
    }
  }

private:
  StreamResponse EmitCrcAndReset() {
    StreamResponse ret;
    ret = output_.Put(TO_BYTE(crc_ >> 24));
    ret += output_.Put(TO_BYTE(crc_ >> 16));
    ret += output_.Put(TO_BYTE(crc_ >> 8));
    ret += output_.Put(TO_BYTE(crc_));
    crc_ = ResetCRC;
    return ret;
  }

  static constexpr uint32_t ResetCRC = 0xFFFFFFFF;
  Stream &output_;
  uint32_t crc_{ResetCRC};
};

// Adds framing markers and escapes special chars
// Will emit FramingMark:
// * upon receiving EndOfStream
// * next time Put called after receiving EndOfStream
// Will emit FramingEscape upon receiving FramingMark or FramingEscape
// characters
class EscapeStream : public Stream {
public:
  explicit EscapeStream(Stream &os) : output_(os){};

  StreamResponse Put(int32_t b) override {
    StreamResponse ret;
    switch (b) {
    case EndOfStream:
      ret = StartFrameIfDone();
      ret += output_.Put(FramingMark);
      ret += output_.Put(b);
      FrameDone();
      return ret;
    case FramingMark:
    case FramingEscape:
      ret = StartFrameIfDone();
      ret += output_.Put(FramingEscape);
      ret += output_.Put(b ^ 0x20);
      FrameStartedMaybe();
      return ret;
    default:
      ret = StartFrameIfDone();
      ret += output_.Put(b);
      FrameStartedMaybe();
      return ret;
    };
  }

private:
  StreamResponse StartFrameIfDone() {
    return is_frame_done_ ? output_.Put(FramingMark)
                          : StreamResponse(0, ResponseFlags::StreamSuccess);
  }

  static bool shouldEscape(uint8_t b) {
    return FramingMark == b || FramingEscape == b;
  }

  void FrameDone() {
    if (!is_frame_done_) {
      is_frame_done_ = true;
    }
  }
  void FrameStartedMaybe() {
    if (is_frame_done_) {
      is_frame_done_ = false;
    }
  }

  Stream &output_;
  bool is_frame_done_{true};
};

// Just returns number of bytes written, designed to be used for evaluation of
// length of buffer being streamed
class CounterStream : public Stream {
public:
  StreamResponse Put(int32_t b) override {
    if (EndOfStream == b) {
      return StreamResponse(0, ResponseFlags::StreamSuccess);
    } else {
      return StreamResponse(1, ResponseFlags::StreamSuccess);
    }
  }
};
