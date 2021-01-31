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

// Datasheet for our EEPROM (Microchip 24AA256) is accessible here:
// https://ww1.microchip.com/downloads/en/DeviceDoc/24AA256-24LC256-24FC256-Data-Sheet-20001203W.pdf
// The EEPROM we have on the board is at address 0x50 (see [PCBsp])

#ifndef EEPROM_H_
#define EEPROM_H_

#include "i2c.h"
#include <stdint.h>

static constexpr uint32_t kMaxMemorySize{65535};

// This class defines an I²C addressable EEPROM.
class I2Ceeprom {
public:
  I2Ceeprom(uint8_t address, uint8_t page_size, uint16_t size,
            I2C::Channel *channel)
      : address_(address), size_(size), page_size_(page_size),
        channel_(channel){};

  // Because of the way I²C works, read/write operations take some time,
  // we use pointers to the place the data has to be put and to a boolean
  // that informs the caller once his request is processed.
  // As for the I2C::Requests, it is up to the caller to ensure length and
  // data are consistent.
  bool ReadBytes(uint16_t offset, uint16_t length, void *data, bool *processed);
  bool WriteBytes(uint16_t offset, uint16_t length, void *data,
                  bool *processed);

protected:
  uint8_t address_;   // 7 bits I²C address
  uint16_t size_;     // in bytes
  uint8_t page_size_; // in bytes
  // pointer to the I²C channel the EEPROM is wired to
  I2C::Channel *channel_;
  virtual bool SendBytes(const I2C::Request &request) {
    return channel_->SendRequest(request);
  }
  virtual bool ReceiveBytes(const I2C::Request &request) {
    return channel_->SendRequest(request);
  }
};

class TestEeprom : public I2Ceeprom {
public:
  TestEeprom(uint8_t address, uint8_t page_size, uint16_t size,
             I2C::Channel *channel)
      : I2Ceeprom(address, page_size, size, channel) {
    for (int i = 0; i < size_; ++i) {
      memory_[i] = 0xFF;
    };
  }

private:
  uint32_t address_pointer_{0};
  uint8_t memory_[kMaxMemorySize];
  bool SendBytes(const I2C::Request &request);
  bool ReceiveBytes(const I2C::Request &request);
};

#endif // EEPROM_H_
