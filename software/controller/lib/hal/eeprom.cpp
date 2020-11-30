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
#include "hal_stm32.h"
#include "i2c.h"
#include <string.h>

I2Ceeprom eeprom = I2Ceeprom(0x50, 64, 32768);

bool I2Ceeprom::ReadBytes(uint16_t offset, uint16_t length, void *data,
                          bool *processed) {

  if (offset + length > size_) {
    // requesting outside of memory capacity
    return false;
  }

  // set the read pointer to the desired offset
  // split the offset into two bytes and send this to the chip (write request)
  uint8_t offset_address[2] = {static_cast<uint8_t>((offset & 0xFF00) >> 8),
                               static_cast<uint8_t>(offset & 0xFF)};
  bool discarded = false;
  I2CRequest pointer_set = {
      .slave_address = address_,
      .read_write = I2CExchangeDir::kWrite,
      .size = 2,
      .data = &offset_address[0],
      .processed = &discarded,
  };

  I2CRequest read_request = {
      .slave_address = address_,
      .read_write = I2CExchangeDir::kRead,
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
};

bool I2Ceeprom::WriteBytes(uint16_t offset, uint16_t length, const void *data,
                           bool *processed) {
  if (offset + length > size_) {
    // requesting outside of memory capacity
    return false;
  }

  // Write operations are a single request where data is prefixed with the
  // offset where we write
  uint8_t write_data[length + 2];
  write_data[0] = static_cast<uint8_t>(offset & 0xFF00 >> 8);
  write_data[1] = static_cast<uint8_t>(offset & 0xFF);
  memcpy(&write_data[2], data, length);

  I2CRequest request = {
      .slave_address = address_,
      .read_write = I2CExchangeDir::kWrite,
      .size = static_cast<uint8_t>(length + 2),
      .data = &write_data[0],
      .processed = processed,
  };

  return i2c1.SendRequest(request);
};
