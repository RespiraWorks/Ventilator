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

#include "gpio.h"

/*
The structures below represent the STM32 registers used
to configure the GPIO module

Detailed information on these modules and the registers
used to configure them can be found in [RM]

Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
*/

namespace GPIO {

// General Purpose I/O
// [RM] 8.4 GPIO Registers (pg 267)
struct RegisterStructure {
  uint32_t mode;                   // Mode register [RM] 8.4.1
  uint32_t output_type;            // Output type register [RM] 8.4.2
  uint32_t output_speed;           // Output speed register [RM] 8.4.3
  uint32_t pullup_pulldown;        // Pull-up/pull-down register [RM] 8.4.4
  uint32_t input_data;             // Input data register [RM] 8.4.5
  uint32_t output_data;            // Output data register [RM] 8.4.6
  uint16_t set;                    // Bit set register [RM] 8.4.7
  uint16_t clear;                  // Bit reset register [RM] 8.4.7
  uint32_t flash_lock;             // Configuration lock register [RM] 8.4.8
  uint32_t alternate_function[2];  // Alternate function low/high register
  // [RM] 8.4.{9,10}
  uint32_t reset;  // Reset register [RM] 8.4.11
};

typedef volatile RegisterStructure Register;

inline Register* const base_address(const Port port) { return reinterpret_cast<Register*>(port); }

}  // namespace GPIO
