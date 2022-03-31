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

#include "clocks.h"

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
  uint32_t alternate_function[2];  // Alternate function low/high register [RM] 8.4.{9,10}
  uint32_t reset;                  // Reset register [RM] 8.4.11
};

typedef volatile RegisterStructure Register;

RegisterStructure *base_address(const Port port) {
  return reinterpret_cast<RegisterStructure *>(port);
}

void enable_all_clocks() {
  // Enable all the GPIO clocks
  enable_peripheral_clock(PeripheralID::GPIOA);
  enable_peripheral_clock(PeripheralID::GPIOB);
  enable_peripheral_clock(PeripheralID::GPIOC);
  enable_peripheral_clock(PeripheralID::GPIOD);
  enable_peripheral_clock(PeripheralID::GPIOE);
  enable_peripheral_clock(PeripheralID::GPIOH);
}

void pin_mode(Port port, uint8_t pin, PinMode mode) {
  Register *const gpio = base_address(port);
  gpio->mode &= ~(0b11 << (pin * 2));
  gpio->mode |= (static_cast<uint32_t>(mode) << (pin * 2));
}

void output_type(Port port, uint8_t pin, OutType output_type) {
  Register *const gpio = base_address(port);
  if (output_type == OutType::OpenDrain)
    gpio->output_type |= 1 << pin;
  else
    gpio->output_type &= ~(1 << pin);
}

// Output pin speeds are set using two consecutive bits / pin.
void output_speed(Port port, uint8_t pin, OutSpeed speed) {
  Register *const gpio = base_address(port);
  auto s = static_cast<uint32_t>(speed);
  gpio->output_speed &= ~(0b11 << (2 * pin));
  gpio->output_speed |= (s << (2 * pin));
}

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
void alternate_function(Port port, uint8_t pin, AlternativeFunction func) {
  Register *const gpio = base_address(port);
  pin_mode(port, pin, PinMode::AlternateFunction);

  uint8_t x = (pin < 8) ? 0 : 1;
  gpio->alternate_function[x] |= (static_cast<uint32_t>(func) << ((pin & 0b111) * 4));
}

// Set a specific output pin
void set_pin(Port port, uint8_t pin) {
  Register *const gpio = base_address(port);
  gpio->set = static_cast<uint16_t>(1 << pin);
}

// Clear a specific output pin
void clear_pin(Port port, uint8_t pin) {
  Register *const gpio = base_address(port);
  gpio->clear = static_cast<uint16_t>(1 << pin);
}

// Return the current value of an input pin
bool get_pin(Port port, uint8_t pin) {
  Register *const gpio = base_address(port);
  return gpio->input_data & (1 << pin);
}

// This adds a pull-up resistor to an input pin
void pull_up(Port port, uint8_t pin) {
  Register *const gpio = base_address(port);
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin));
  x |= 1 << (2 * pin);
  gpio->pullup_pulldown = x;
}

// This adds a pull-down resistor to an input pin
void pull_down(Port port, uint8_t pin) {
  Register *const gpio = base_address(port);
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin));
  x |= 2 << (2 * pin);
  gpio->pullup_pulldown = x;
}

}  // namespace GPIO
