/* Copyright 2020-2022, RespiraWorks
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

#include "framing_streams.h"

class ObserverStream : public OutputStream {
 public:
  static constexpr size_t StreamLength{200};
  size_t i{0};
  uint8_t saved[StreamLength];

  StreamResponse put(int32_t b) override {
    if (EndOfStream == b) {
      return {};
    }

    if (i < StreamLength) {
      saved[i++] = static_cast<uint8_t>(b);
      return StreamResponse(1, ResponseFlags::StreamSuccess);
    } else {
      return StreamResponse(0, ResponseFlags::ErrorBufferFull);
    }
  }
  // Returns number of bytes that can be written.
  size_t bytes_available_for_write() override { return StreamLength - i; }
  uint8_t *get_saved() { return saved; }
  size_t get_index() { return i; }
};
