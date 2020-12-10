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

#ifndef NVPARAM_H_
#define NVPARAM_H_

#include "network_protocol.pb.h"
#include "units.h"
#include <stddef.h>
#include <stdint.h>

// This structure defines the layout of the non-volatile
// parameter info stored in the I²C EEPROM.
struct NVparams {
  // Header info used to keep track of parameter info
  uint32_t crc{0};    // 32-bit CRC of remaining structure
  uint8_t count{0};   // Incremented on each write.
  uint8_t version{0}; // Version of the structure.

  uint8_t reinit{0}; // Write to 1 (through dbg_reinit) to request for a reinit
                     // on next boot. This should prove useful if our system
                     // memory ends up in a weird state during testing.

  uint8_t reserved{0}; // kept here for alignment's sake

  uint32_t vent_serial_number{0};
  // Non-volatile parameters should be added here
  uint32_t power_cycles{0};      // Count of device ON/OFF cycles.
  uint32_t cumulated_service{0}; // Cumulated power-ON time, stored in seconds.
                                 // May rollover after 136 years
  VentParams last_settings =
      VentParams_init_default; // Last settings seen by the vent
};

// We are reserving the first 8 kB out of our 32kB eeprom for nv params.
// Since we use a flip/flop that means NVparams should be at most 4kB.
static_assert(sizeof(NVparams) <= 4096);

enum class NVParamsAddress {
  kFlip = 0,
  kFlop = 4096,
};

// Class that encapsulates NVparams. We need the NVparams struct to be
// defined independently in order to facilitate access and casting, but
// the actual struct we use is encapsulated here in order to write the
// changes made to the struct in EEPROM.
class NVParams {
public:
  void Init();
  bool Set(uint16_t offset, void *value, uint8_t len);
  bool Get(uint16_t offset, void *value, uint8_t len);
  void Update(const Time now, const VentParams params);

private:
  NVparams nv_param_;
  bool linked_{true};
  // Address of valid parameter block - defaulted to Flip
  NVParamsAddress nvparam_addr_{NVParamsAddress::kFlip};
  Time last_update_{microsSinceStartup(0)};
  // Update cumulated service interval
  static constexpr Duration kUpdateInterval = seconds(60);
};

static NVParams nv_params;

// Convenience macro to read/write a member of the non-volatile
// parameter structure given it's name
#define NVparamsUpdate(member, value)                                          \
  nv_params.Set(static_cast<uint16_t>(offsetof(NVparams, member)), value,      \
                (sizeof(((NVparams *)0)->member)))
#define NVparamsRead(member, value)                                            \
  nv_params.Get(static_cast<uint16_t>(offsetof(NVparams, member)), value,      \
                (sizeof(((NVparams *)0)->member)))

#endif // NVPARAM_H_
