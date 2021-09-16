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

#include "gpio.h"

#include "gpio_regs.h"

namespace GPIO {

void SetPinMode(Port port, int pin, PinMode mode) {
  auto *gpio = base_address(port);
  gpio->mode &= ~(0b11 << (pin * 2));
  gpio->mode |= (static_cast<int>(mode) << (pin * 2));
}

// Value for GPIOx_OTYPER ([RM] 8.4.2)
void SetOutType(Port port, int pin, OutType output_type) {
  auto *gpio = base_address(port);
  if (output_type == OutType::OpenDrain)
    gpio->output_type |= 1 << pin;
  else
    gpio->output_type &= ~(1 << pin);
}

// Output pin speeds are set using two consecutive bits / pin.
void SetOutSpeed(Port port, int pin, OutSpeed speed) {
  auto *gpio = base_address(port);
  int s = static_cast<int>(speed);
  gpio->output_speed &= ~(0b11 << (2 * pin));
  gpio->output_speed |= (s << (2 * pin));
}

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
void PinAltFunc(Port port, int pin, int func) {
  auto *gpio = base_address(port);
  SetPinMode(port, pin, PinMode::AlternateFunction);

  int x = (pin < 8) ? 0 : 1;
  gpio->alternate_function[x] |= (func << ((pin & 0b111) * 4));
}

// Set a specific output pin
void SetPin(Port port, int pin) {
  auto *gpio = base_address(port);
  gpio->set = static_cast<uint16_t>(1 << pin);
}

// Clear a specific output pin
void ClrPin(Port port, int pin) {
  auto *gpio = base_address(port);
  gpio->clear = static_cast<uint16_t>(1 << pin);
}

// Return the current value of an input pin
int GetPin(Port port, int pin) {
  auto *gpio = base_address(port);
  return (gpio->input_data & (1 << pin)) ? 1 : 0;
}

// This adds a pull-up resistor to an input pin
void PullUp(Port port, int pin) {
  auto *gpio = base_address(port);
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin));
  x |= 1 << (2 * pin);
  gpio->pullup_pulldown = x;
}

// This adds a pull-down resistor to an input pin
void PullDn(Port port, int pin) {
  auto *gpio = base_address(port);
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin));
  x |= 2 << (2 * pin);
  gpio->pullup_pulldown = x;
}

}  // namespace GPIO
