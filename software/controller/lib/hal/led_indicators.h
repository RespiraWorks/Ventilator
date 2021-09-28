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

#include <cstdint>

#if !defined(BARE_STM32)
#include <map>

#include "gpio.h"
#endif

// Binary pins set by the controller -- these are booleans, High or Low.
//
// PWM pins can of course be HIGH or LOW too, but we separate out purely on/off
// pins from PWM pins for reasons of "strong typing".
//
// Pins default to Input, so if you add a new pin here, be sure to update
// HalApi::Init() and set it to Output!
enum class BinaryPin {
  RedLED,
  YellowLED,
  GreenLED,
};

// Voltage level of a digital pin.
// Usage: VoltageLevel::High, Low
enum class VoltageLevel { High, Low };

class LEDIndicators {
 public:
  void initialize();

  // Sets `binary_pin` to high or low.
  void set(BinaryPin binary_pin, VoltageLevel value);

#if !defined(BARE_STM32)
 public:
  /// \TODO: is this the best thing to test? Isn't this implementation-specific?
  void set_pin_mode(BinaryPin pin, GPIO::PinMode mode);

 private:
  std::map<BinaryPin, GPIO::PinMode> binary_pin_modes_;
  std::map<BinaryPin, VoltageLevel> binary_pin_values_;
#endif
};
