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

/*
This file defines GPIO functions for HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.

Reference abbreviations [RM], [DS], etc are defined in hal/README.md.
*/

#pragma once

#include <cstdint>

namespace GPIO {

/// \TODO: MOve these mappings elsewhere
enum class Port : uint32_t {
  PortA = 0x48000000,
  PortB = 0x48000400,
  PortC = 0x48000800,
  PortD = 0x48000C00,
  PortE = 0x48001000,
  PortH = 0x48001C00,
};

// Each pin has a 2-bit mode value that can be set using this function.
// Pin 0 mode is in bits 0-1, pin 1 in 2-3, etc.  ([RM] 8.4.1)
enum class PinMode {
  Input = 0,
  Output = 1,
  AlternateFunction = 2,
  Analog = 3,
};

// Value for GPIOx_OTYPER ([RM] 8.4.2)
enum class OutType { PushPull = 0, OpenDrain = 1 };

// Output pin speeds are set using two consecutive bits / pin.
enum class OutSpeed { Slow = 0, Medium = 1, Fast = 2, Smoking = 3 };

void SetPinMode(Port port, int pin, PinMode mode);

void SetOutType(Port port, int pin, OutType output_type);

void SetOutSpeed(Port port, int pin, OutSpeed speed);

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
void PinAltFunc(Port port, int pin, int func);

// Set a specific output pin
void SetPin(Port port, int pin);

// Clear a specific output pin
void ClrPin(Port port, int pin);

// Return the current value of an input pin
int GetPin(Port port, int pin);

// This adds a pull-up resistor to an input pin
void PullUp(Port port, int pin);

// This adds a pull-down resistor to an input pin
void PullDn(Port port, int pin);

}  // namespace GPIO
