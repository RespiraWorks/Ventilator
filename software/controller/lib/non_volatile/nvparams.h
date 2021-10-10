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

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <array>

#include "eeprom.h"
#include "network_protocol.pb.h"
#include "units.h"

// pinch_valve calibration size, defined here because the tables are stored in NVParams.
static constexpr size_t ActuatorsCalSize{11};

namespace NVParams {

// This structure defines the layout of the non-volatile
// parameter info stored in the I²C EEPROM.
struct Structure {
  // Header info used to keep track of parameter info
  uint32_t crc{0};     // 32-bit CRC of remaining structure
  uint8_t count{0};    // Incremented on each write.
  uint8_t version{0};  // Version of the structure. If some fields in the structure change,
                       // there is a risk of misinterpreting data that is in EEPROM during
                       // initialization. This version is compared to the one in EEPROM and
                       // the params in EEPROM are declared invalid and overwritten with default
                       // valuesif there is a mismatch.
                       // Note that if the size of the structure changes, the chances that the crc
                       // matches are non-existent and incrementing the version is not useful: the
                       // EEPROM params will be invalid no matter what.

  uint8_t reinit{0};  // Write to 1 (through dbg_reinit) to request for a reinit
                      // on next boot. This should prove useful if our system
                      // memory ends up in a weird state during testing.

  uint8_t reserved{0};  // Kept here for 32bits-alignment's sake

  uint32_t vent_serial_number{0};
  // Non-volatile parameters should be added here
  uint32_t power_cycles{0};       // Count of device ON/OFF cycles.
  uint32_t cumulated_service{0};  // Cumulated power-ON time, stored in seconds.
                                  // May rollover after 136 years
  VentParams last_settings = VentParams_init_default;  // Last settings seen by the vent

  // The following 2 tables are used to roughly linearize the pinch valves output.  They were built
  // by adjusting the pinch valve and monitoring the flow through the venturi tube. The entries
  // should give pinch valve settings for a list of equally spaced flow rates.  The first entry
  // should be the setting for 0 flow rate (normally 0) and the last entry should be the setting
  // for 100% flow rate. The minimum length of the table is 2 entries.
  std::array<float, ActuatorsCalSize> blower_pinch_cal{0.0000f, 0.0410f, 0.0689f, 0.0987f,
                                                       0.1275f, 0.1590f, 0.1932f, 0.2359f,
                                                       0.2940f, 0.3988f, 1.0000f};
  std::array<float, ActuatorsCalSize> exhale_pinch_cal{0.0000f, 0.0410f, 0.0689f, 0.0987f,
                                                       0.1275f, 0.1590f, 0.1932f, 0.2359f,
                                                       0.2940f, 0.3988f, 1.0000f};
};

// We are reserving the first 8 kB out of our 32kB eeprom for nv params.
// Since we use a double buffer Structure should be at most 4kB.
static_assert(sizeof(Structure) <= 4096);

enum class Address {
  Flip = 0,
  Flop = 4096,
};

// Class that encapsulates NVParams. We need the Structure to be
// defined independently in order to facilitate access and casting, but
// the actual struct we use is encapsulated here in order to write the
// changes made to the struct in EEPROM.
class Handler {
 public:
  Handler() = default;
  void Init(I2Ceeprom *eeprom);
  bool Set(uint16_t offset, const void *value, uint16_t len);
  bool Get(uint16_t offset, void *value, uint16_t len);
  void Update(Time now, VentParams *params);

 private:
  Structure nv_param_;
  // Address of valid parameter block - defaulted to Flip
  Address nvparam_addr_{Address::Flip};
  Time last_update_{microsSinceStartup(0)};
  I2Ceeprom *eeprom_{nullptr};
  // Update cumulated service interval
  static constexpr Duration UpdateInterval{seconds(60)};
  bool linked_to_eeprom_{false};  // in case of EEPROM failure at startup,
                                  // we can spare ourselves the need to write
                                  // data to the eeprom, even if we will still
                                  // update contents in our internal memory.

  void WriteFullParams(Address address);
  bool ReadFullParams(Address address, Structure *param, I2Ceeprom *eeprom);
};

}  // namespace NVParams

// Convenience macros to read/write a member of the non-volatile parameter
// structure given its name.
// Because these are macros, they cannot be part of the NVParams namespace, but
// should still only be used as member functions of the Handler class.
#define NV_PARAMS_UPDATE(member, value) \
  Set(static_cast<uint16_t>(offsetof(Structure, member)), value, (sizeof(((Structure *)0)->member)))
#define NV_PARAMS_READ(member, value) \
  Get(static_cast<uint16_t>(offsetof(Structure, member)), value, (sizeof(((Structure *)0)->member)))
