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
                          bool *processed) {

  if (offset + length > size_) {
    // requesting outside of memory capacity
    return false;
  }

  // set the read pointer to the desired offset
  // split the offset into two bytes and send this to the chip (write request)
  uint8_t offset_address[2];
  offset_address[0] = static_cast<uint8_t>((offset & 0x7F00) >> 8);
  offset_address[1] = static_cast<uint8_t>(offset & 0xFF);

  bool discarded = false;
  I2C::Request pointer_set = {
      .slave_address = address_,
      .direction = I2C::ExchangeDirection::kWrite,
      .size = 2,
      .data = &offset_address[0],
      .processed = &discarded,
  };

  I2C::Request read_request = {
      .slave_address = address_,
      .direction = I2C::ExchangeDirection::kRead,
      .size = length,
      .data = data,
      .processed = processed,
  };

  // Queue both requests back to back, the second only if the first is succesful
  if (SendBytes(pointer_set)) {
    return ReceiveBytes(read_request);
  } else {
    return false;
  }
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
        static_cast<uint8_t>(page_size_ - (current_offset % page_size_));

    if (current_offset + request_length > offset + length) {
      // last request, only write the remaining bytes and not a full page
      request_length = static_cast<uint8_t>(offset + length - current_offset);
    }

    uint8_t write_data[request_length + 2];
    write_data[0] = static_cast<uint8_t>((current_offset & 0x7F00) >> 8);
    write_data[1] = static_cast<uint8_t>(current_offset & 0xFF);

    memcpy(&write_data[2], current_data, request_length);

    I2C::Request request = {
        .slave_address = address_,
        .direction = I2C::ExchangeDirection::kWrite,
        .size = static_cast<uint8_t>(request_length + 2),
        .data = &write_data[0],
        .processed = processed,
    };

    success &= SendBytes(request);
    if (!success) {
      break;
    }
    current_offset = static_cast<uint16_t>(current_offset + request_length);
    current_data = current_data + request_length;
  }
  return success;
};

bool TestEeprom::SendBytes(const I2C::Request &request) {
  address_pointer_ = reinterpret_cast<uint8_t *>(request.data)[0] << 8 |
                     reinterpret_cast<uint8_t *>(request.data)[1];
  for (uint32_t i = 2; i < request.size; ++i) {
    memory_[address_pointer_++] = reinterpret_cast<uint8_t *>(request.data)[i];
  }
  if (request.processed != nullptr)
    *(request.processed) = true;
  return true;
}

bool TestEeprom::ReceiveBytes(const I2C::Request &request) {
  for (uint32_t i = 0; i < request.size; ++i) {
    reinterpret_cast<uint8_t *>(request.data)[i] = memory_[address_pointer_++];
  }
  if (request.processed != nullptr)
    *(request.processed) = true;
  return true;
}
