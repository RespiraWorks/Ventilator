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
#if !defined(BARE_STM32)

/// \TODO should include mock header, not this
#include "gpio_stm32.h"

namespace GPIO {

Pin::Pin(Port port, uint8_t pin, PinMode mode) : port_(port), pin_(pin){};

void Pin::output_type(OutType output_type) {}

void Pin::output_speed(OutSpeed speed) {}

void Pin::pull_type(PullType pull) {}

void Pin::alternate_function(AlternativeFunction func) {}

DigitalOutputPin::DigitalOutputPin(Port port, uint8_t pin, bool start_high, OutSpeed speed,
                                   OutType type)
    : Pin(port, pin, PinMode::Output), value_(start_high) {}
void DigitalOutputPin::set() { value_ = true; }

void DigitalOutputPin::clear() { value_ = false; }

bool DigitalOutputPin::get() const { return value_; }

DigitalInputPin::DigitalInputPin(Port port, uint8_t pin, PullType pull)
    : Pin(port, pin, PinMode::Input) {}
void DigitalInputPin::set() { value_ = true; }

void DigitalInputPin::clear() { value_ = false; }

bool DigitalInputPin::get() const { return value_; }

AlternatePin::AlternatePin(Port port, uint8_t pin, AlternativeFunction func, PullType pull,
                           OutSpeed speed, OutType type)
    : Pin(port, pin, PinMode::AlternateFunction) {}
}  // namespace GPIO
#endif
