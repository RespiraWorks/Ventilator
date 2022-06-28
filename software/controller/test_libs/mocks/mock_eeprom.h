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

#include "eeprom.h"

class TestEeprom : public I2Ceeprom {
 public:
  TestEeprom(uint8_t address, uint8_t page_size, uint16_t size)
      : I2Ceeprom(address, page_size, size, nullptr) {
    for (int i = 0; i < size_; ++i) {
      memory_[i] = 0xFF;
    };
  }

 private:
  uint32_t address_pointer_{0};
  uint8_t memory_[MaxMemorySize];
  bool SendBytes(const I2C::Request &request) override {
    address_pointer_ = reinterpret_cast<uint8_t *>(request.data)[0] << 8 |
                       reinterpret_cast<uint8_t *>(request.data)[1];
    for (uint32_t i = 2; i < request.size; ++i) {
      memory_[address_pointer_++] = reinterpret_cast<uint8_t *>(request.data)[i];
    }
    if (request.processed != nullptr) {
      *(request.processed) = true;
    }
    return true;
  }

  bool ReceiveBytes(const I2C::Request &request) override {
    for (uint32_t i = 0; i < request.size; ++i) {
      reinterpret_cast<uint8_t *>(request.data)[i] = memory_[address_pointer_++];
    }
    if (request.processed != nullptr) {
      *(request.processed) = true;
    }
    return true;
  }
};
