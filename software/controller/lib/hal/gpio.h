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

/// \TODO: Move these mappings elsewhere
enum class Port : uint32_t {
  PortA = 0x48000000,
  PortB = 0x48000400,
  PortC = 0x48000800,
  PortD = 0x48000C00,
  PortE = 0x48001000,
  PortH = 0x48001C00,
};

// Value for GPIOx_MODER ([RM] 8.4.1)
enum class PinMode : uint8_t {
  Input = 0b00,
  Output = 0b01,
  AlternateFunction = 0b10,
  Analog = 0b11,
};

// Value for GPIOx_OTYPER ([RM] 8.4.2)
enum class OutType : uint8_t { PushPull = 0b0, OpenDrain = 0b1 };

// Value for GPIOx_OSPEEDR ([RM] 8.4.3)
enum class OutSpeed : uint8_t { Slow = 0b00, Medium = 0b01, Fast = 0b10, Smoking = 0b11 };

void SetPinMode(Port port, uint8_t pin, PinMode mode);

void SetOutType(Port port, uint8_t pin, OutType output_type);

void SetOutSpeed(Port port, uint8_t pin, OutSpeed speed);

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
void PinAltFunc(Port port, uint8_t pin, uint8_t func);

// Set a specific output pin
void SetPin(Port port, uint8_t pin);

// Clear a specific output pin
void ClrPin(Port port, uint8_t pin);

// Return the current value of an input pin ([RM] 8.4.5)
uint16_t GetPin(Port port, uint8_t pin);

// This adds a pull-up resistor to an input pin
void PullUp(Port port, uint8_t pin);

// This adds a pull-down resistor to an input pin
void PullDn(Port port, uint8_t pin);

}  // namespace GPIO
