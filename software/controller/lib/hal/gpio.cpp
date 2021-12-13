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
  uint32_t address = [port]() -> uint32_t {
    switch (port) {
      case GPIO::Port::A:
        return 0x48000000;
      case GPIO::Port::B:
        return 0x48000400;
      case GPIO::Port::C:
        return 0x48000800;
      case GPIO::Port::D:
        return 0x48000C00;
      case GPIO::Port::E:
        return 0x48001000;
      case GPIO::Port::H:
        return 0x48001C00;
    }
    __builtin_unreachable();
  }();
  return reinterpret_cast<RegisterStructure *>(address);
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

Pin::Pin(Port port, uint8_t pin, PinMode mode) : port_(port), pin_(pin) {
#if defined(BARE_STM32)
  auto *gpio = base_address(port_);
  // reset the mode bits to 0
  gpio->mode &= ~(0b11 << (pin_ * 2));
  // set the mode bits to the desired value
  gpio->mode |= (static_cast<uint32_t>(mode) << (pin_ * 2));
#endif
};

#if defined(BARE_STM32)
void Pin::output_type(OutType output_type) {
  auto *gpio = base_address(port_);
  if (output_type == OutType::OpenDrain)
    gpio->output_type |= 1 << pin_;
  else
    gpio->output_type &= ~(1 << pin_);
}

void Pin::output_speed(OutSpeed speed) {
  auto *gpio = base_address(port_);
  auto s = static_cast<uint32_t>(speed);
  gpio->output_speed &= ~(0b11 << (2 * pin_));
  gpio->output_speed |= (s << (2 * pin_));
}

void Pin::pull_type(PullType pull) {
  auto *gpio = base_address(port_);
  // Get the current value within the register, masking the bits we want to alter
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin_));
  // Only alter the appropriate bits
  x |= static_cast<uint32_t>(pull) << (2 * pin_);
  gpio->pullup_pulldown = x;
}

void Pin::alternate_function(AlternativeFunction func) {
  auto *gpio = base_address(port_);
  uint8_t index = (pin_ < 8) ? 0 : 1;
  gpio->alternate_function[index] |= (static_cast<uint32_t>(func) << ((pin_ & 0b111) * 4));
}

void DigitalOutputPin::set() {
  auto *gpio = base_address(port_);
  gpio->set = static_cast<uint16_t>(1 << pin_);
}

void DigitalOutputPin::clear() {
  auto *gpio = base_address(port_);
  gpio->clear = static_cast<uint16_t>(1 << pin_);
}

bool DigitalInputPin::get() const {
  auto *gpio = base_address(port_);
  return gpio->input_data & (1 << pin_);
}

#else
// \TODO add a real mock for those?
void Pin::output_type(OutType output_type) {}
void Pin::output_speed(OutSpeed speed) {}
void Pin::pull_type(PullType pull) {}
void Pin::alternate_function(AlternativeFunction func) {}

void DigitalOutputPin::set() { value_ = true; }
void DigitalOutputPin::clear() { value_ = false; }
bool DigitalOutputPin::get() const { return value_; }

void DigitalInputPin::set() { value_ = true; }
void DigitalInputPin::clear() { value_ = false; }
bool DigitalInputPin::get() const { return value_; }

#endif

DigitalOutputPin::DigitalOutputPin(Port port, uint8_t pin, bool start_high, OutSpeed speed,
                                   OutType type)
    : Pin(port, pin, PinMode::Output) {
  // set high ASAP
  if (start_high) {
    set();
  } else {
    clear();
  }
  output_type(type);
  output_speed(speed);
}

DigitalInputPin::DigitalInputPin(Port port, uint8_t pin, PullType pull)
    : Pin(port, pin, PinMode::Input) {
  pull_type(pull);
};

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
AlternatePin::AlternatePin(Port port, uint8_t pin, AlternativeFunction func, PullType pull,
                           OutSpeed speed, OutType type)
    : Pin(port, pin, PinMode::AlternateFunction) {
  output_type(type);
  output_speed(speed);
  alternate_function(func);
  pull_type(pull);
}

}  // namespace GPIO
