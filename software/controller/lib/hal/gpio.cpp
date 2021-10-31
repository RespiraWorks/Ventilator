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

void enable_all_clocks() {
  // Enable all the GPIO clocks
  enable_peripheral_clock(PeripheralID::GPIOA);
  enable_peripheral_clock(PeripheralID::GPIOB);
  enable_peripheral_clock(PeripheralID::GPIOC);
  enable_peripheral_clock(PeripheralID::GPIOD);
  enable_peripheral_clock(PeripheralID::GPIOE);
  enable_peripheral_clock(PeripheralID::GPIOH);
}

Pin::Pin(Port port, uint8_t pin, PinMode mode) : pin_(pin) {
#if defined(BARE_STM32)
  gpio_ = reinterpret_cast<RegisterStructure *>(port);
  // reset the mode bits to 0
  gpio_->mode &= ~(0b11 << (pin_ * 2));
  // set the mode bits to the desired value
  gpio_->mode |= (static_cast<uint32_t>(mode) << (pin_ * 2));
#endif
};

#if defined(BARE_STM32)
// Helper functions to set output type for either Output or Alt function pin
void Pin::output_type(OutType output_type) {
  if (output_type == OutType::OpenDrain)
    gpio_->output_type |= 1 << pin_;
  else
    gpio_->output_type &= ~(1 << pin_);
}

// Helper functions to set output speed for either Output or Alt function pin
void Pin::output_speed(OutSpeed speed) {
  auto s = static_cast<uint32_t>(speed);
  gpio_->output_speed &= ~(0b11 << (2 * pin_));
  gpio_->output_speed |= (s << (2 * pin_));
}

// Helper function to set pullup/pulldown resistor for either Input or Alt function pin
void Pin::pull_type(PullType pull) {
  // Get the current value within the register, masking the bits we want to alter
  uint32_t x = gpio_->pullup_pulldown & ~(3 << (2 * pin_));
  // Only alter the appropriate bits
  x |= static_cast<uint32_t>(pull) << (2 * pin_);
  gpio_->pullup_pulldown = x;
}

void DigitalOutputPin::set() { gpio_->set = static_cast<uint16_t>(1 << pin_); }

void DigitalOutputPin::clear() { gpio_->clear = static_cast<uint16_t>(1 << pin_); }

bool DigitalInputPin::get() const { return gpio_->input_data & (1 << pin_); }

#else
// \TODO add a real mock?
void Pin::output_type(OutType output_type) {}
void Pin::output_speed(OutSpeed speed) {}
void Pin::pull_type(PullType pull) {}

void DigitalOutputPin::set() {}
void DigitalOutputPin::clear() {}
bool DigitalInputPin::get() const { return true; }

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
  // set AlternateFunction number
#if defined(BARE_STM32)
  uint8_t index = (pin < 8) ? 0 : 1;
  gpio_->alternate_function[index] |= (static_cast<uint32_t>(func) << ((pin & 0b111) * 4));
#endif
  // Set pull type register:
  pull_type(pull);
}

AnalogInputPin::AnalogInputPin(Port port, uint8_t pin, ADC *adc, AdcChannel channel)
    : Pin(port, pin, PinMode::Analog), adc_(adc), channel_(channel) {
#if defined(BARE_STM32)
  adc_->add_channel(channel_);
#endif
}

Voltage AnalogInputPin::read() const { return adc_->read(channel_); }

}  // namespace GPIO
