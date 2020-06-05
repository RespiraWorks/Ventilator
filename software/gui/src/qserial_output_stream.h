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

#include <QSerialPort>

#include "framing_streams.h"

class QSerialOutputStream : public OutputStream {
 public:
  QSerialOutputStream(std::unique_ptr<QSerialPort> &port) : port_(port) {}

  // We can write virtually unlimited number of bytes
  uint32_t bytes_available_for_write() override { return 9001; }

  StreamResponse put(int32_t b) override {
    if (EndOfStream == b) {
      return {.count_written = 0, .flags = ResponseFlags::StreamSuccess};
    }
    port_->putChar(static_cast<uint8_t>(b));
    return {.count_written = 0, .flags = ResponseFlags::StreamSuccess};
  }

 private:
  std::unique_ptr<QSerialPort> &port_;
};
