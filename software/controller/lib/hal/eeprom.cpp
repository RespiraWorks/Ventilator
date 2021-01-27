/* Copyright 2020, RespiraWorks

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

#include "eeprom.h"
#include "hal.h"
#include <string.h>

bool I2Ceeprom::ReadBytes(uint16_t offset, uint16_t length, void *data,
                          bool *processed) const {

  if (offset + length > size_) {
    // requesting outside of memory capacity
    return false;
  }

  // set the read pointer to the desired offset
  // split the offset into two bytes and send this to the chip (write request)
  uint8_t offset_address[2];
  offset_address[0] = static_cast<uint8_t>((offset & 0x7F00) >> 8);
  offset_address[1] = static_cast<uint8_t>(offset & 0xFF);

#ifdef TEST_MODE
  // faked when testing
  uint reconstructed_offset = (offset_address[0] << 8) | offset_address[1];
  memcpy(data, &memory_[reconstructed_offset], length);
  if (processed != nullptr)
    *processed = true;
  return true;
#elif defined(BARE_STM32)
  bool discarded = false;
  I2C::Request pointer_set = {
      .slave_address = address_,
      .read_write = I2C::ExchangeDirection::kWrite,
      .size = 2,
      .data = &offset_address[0],
      .processed = &discarded,
  };

  I2C::Request read_request = {
      .slave_address = address_,
      .read_write = I2C::ExchangeDirection::kRead,
      .size = length,
      .data = data,
      .processed = processed,
  };

  // Queue both requests back to back, the second only if the first is succesful
  if (i2c1.SendRequest(pointer_set)) {
    return i2c1.SendRequest(read_request);
  } else {
    return false;
  }
#endif
};

bool I2Ceeprom::WriteBytes(uint16_t offset, uint16_t length, void *data,
                           bool *processed) {
  if (offset + length > size_) {
    // requesting outside of memory capacity
    return false;
  }

  // Break write requests into page writes, no write accross page boudaries
  bool success{true};
  uint16_t current_offset{offset};
  uint8_t *current_data = reinterpret_cast<uint8_t *>(data);

  while (current_offset < offset + length) {
    // provision request length from current offset to the end of the page
    uint8_t request_length =
        static_cast<uint8_t>(kPageLength - (current_offset % kPageLength));

    if (current_offset + request_length > offset + length) {
      // last request, only write the remaining bytes and not a full page
      request_length = static_cast<uint8_t>(offset + length - current_offset);
    }

    uint8_t write_data[request_length + 2];
    write_data[0] = static_cast<uint8_t>((current_offset & 0x7F00) >> 8);
    write_data[1] = static_cast<uint8_t>(current_offset & 0xFF);

    memcpy(&write_data[2], current_data, request_length);

#ifdef TEST_MODE
    // faked when testing
    memcpy(&memory_[current_offset], &write_data[2], request_length);
#elif defined(BARE_STM32)
    I2C::Request request = {
        .slave_address = address_,
        .read_write = I2C::ExchangeDirection::kWrite,
        .size = static_cast<uint8_t>(request_length + 2),
        .data = &write_data[0],
        .processed = processed,
    };

    success &= i2c1.SendRequest(request);
    if (!success) {
      break;
    }
#endif
    current_offset = static_cast<uint16_t>(current_offset + request_length);
    current_data = current_data + request_length;
  }
#ifdef TEST_MODE
  if (processed != nullptr)
    *processed = true;
#endif
  return success;
};
