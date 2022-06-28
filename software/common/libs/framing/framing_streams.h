/* Copyright 2020-2021, RespiraWorks
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <cstddef>
#include <cstdint>

#include "checksum.h"
#include "framing_spec_chars.h"

static constexpr int32_t EndOfStream{-1};

// \todo ensure this behaves like flags, add tests
enum class ResponseFlags {
  StreamSuccess = 0,
  WarningBufferFull = 1,
  ErrorBufferFull = 2,
  ErrorStreamBroken = 4
};

// A response object returned from Stream.put function
class StreamResponse {
 public:
  StreamResponse() = default;

  StreamResponse(size_t count, ResponseFlags flag)
      : count_written_(count), flags_(static_cast<uint32_t>(flag)) {}

  StreamResponse &operator+=(const StreamResponse &r) {
    this->count_written_ += r.count_written_;
    this->flags_ |= r.flags_;
    return *this;
  }

  // accessors for testing purposes
  size_t Count() { return count_written_; };
  size_t Flags() { return flags_; };

 protected:
  // The amount of bytes written
  size_t count_written_{0};
  // Status flags
  uint32_t flags_{0};
};

// Stream interface
class Stream {
 public:
  // Puts a byte into the stream.
  // EndOfStream is a special char that designates the end of the stream. We use this for framing.
  virtual StreamResponse put(int32_t b) = 0;
};

// OutputStream is connected to some output mechanism that knows how many bytes can be written to
// it at the moment.
class OutputStream : public Stream {
  // Returns number of bytes that can be written.
 public:
  virtual size_t bytes_available_for_write() = 0;
};

#define TO_BYTE(UI) static_cast<uint8_t>((UI)&0x000000FF)

// Passes bytes to the child Stream
// Calculates CRC while passing bytes
// Emits CRC to the child Stream upon receiving EndOfStream
class CrcStream : public Stream {
 public:
  explicit CrcStream(Stream &os) : output_(os){};

  StreamResponse put(int32_t b) override {
    StreamResponse ret;
    if (EndOfStream == b) {
      ret = emit_CRC_and_reset();
      ret += output_.put(EndOfStream);
      return ret;
    }
    crc_ = crc32_single(crc_, static_cast<uint8_t>(b));
    return output_.put(b);
  }

 private:
  StreamResponse emit_CRC_and_reset() {
    StreamResponse ret;
    ret = output_.put(TO_BYTE(crc_ >> 24));
    ret += output_.put(TO_BYTE(crc_ >> 16));
    ret += output_.put(TO_BYTE(crc_ >> 8));
    ret += output_.put(TO_BYTE(crc_));
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
// * next time put called after receiving EndOfStream
// Will emit FramingEscape upon receiving FramingMark or FramingEscape characters
class EscapeStream : public Stream {
 public:
  explicit EscapeStream(Stream &os) : output_(os){};

  StreamResponse put(int32_t b) override {
    StreamResponse ret = start_frame_if_done();
    switch (b) {
      case EndOfStream:
        ret += output_.put(FramingMark);
        ret += output_.put(b);
        frame_done();
        return ret;
      case FramingMark:
      case FramingEscape:
        ret += output_.put(FramingEscape);
        ret += output_.put(EscapedChar(b));
        frame_started();
        return ret;
      default:
        ret += output_.put(b);
        frame_started();
        return ret;
    };
  }

 private:
  StreamResponse start_frame_if_done() {
    return is_frame_done_ ? output_.put(FramingMark)
                          : StreamResponse(0, ResponseFlags::StreamSuccess);
  }

  // helper function to set is_frame_done_ flag if needed
  void frame_done() {
    if (!is_frame_done_) {
      is_frame_done_ = true;
    }
  }

  // helper function to unset the is_frame_done_ flag if needed
  void frame_started() {
    if (is_frame_done_) {
      is_frame_done_ = false;
    }
  }

  Stream &output_;
  bool is_frame_done_{true};
};

// Just returns number of bytes written, designed to be used for evaluation of length of buffer
// being streamed
class CounterStream : public Stream {
 public:
  StreamResponse put(int32_t b) override {
    if (EndOfStream == b) {
      return StreamResponse(0, ResponseFlags::StreamSuccess);
    }
    return StreamResponse(1, ResponseFlags::StreamSuccess);
  }
};
