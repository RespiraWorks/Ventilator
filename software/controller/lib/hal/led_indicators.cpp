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

#include "led_indicators.h"

#include <algorithm>
#include <cassert>

#include "gpio.h"

#if defined(BARE_STM32)

/******************************************************************
 * LED outputs.
 *  PC13 - red
 *  PC14 - yellow
 *  PC15 - green
 *****************************************************************/
void LEDIndicators::initialize() {
  using IOPort = GPIO::Port;
  using IOMode = GPIO::PinMode;

  // Configure LED pins as outputs
  GPIO::pin_mode(IOPort::C, 13, IOMode::Output);
  GPIO::pin_mode(IOPort::C, 14, IOMode::Output);
  GPIO::pin_mode(IOPort::C, 15, IOMode::Output);

  // Turn all three LEDs off initially
  GPIO::clear_pin(IOPort::C, 13);
  GPIO::clear_pin(IOPort::C, 14);
  GPIO::clear_pin(IOPort::C, 15);
}

// Set or clear the specified digital output
void LEDIndicators::set(BinaryPin binary_pin, VoltageLevel value) {
  auto [port, pin] = [&]() -> std::pair<GPIO::Port, uint8_t> {
    switch (binary_pin) {
      case BinaryPin::RedLED:
        return {GPIO::Port::C, 13};
      case BinaryPin::YellowLED:
        return {GPIO::Port::C, 14};
      case BinaryPin::GreenLED:
        return {GPIO::Port::C, 15};
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  switch (value) {
    case VoltageLevel::High:
      GPIO::set_pin(port, pin);
      break;

    case VoltageLevel::Low:
      GPIO::clear_pin(port, pin);
      break;
  }
}

#else

void LEDIndicators::initialize() {}
void LEDIndicators::set(BinaryPin pin, VoltageLevel value) {
  if (binary_pin_modes_[pin] != GPIO::PinMode::Output) {
    assert(false && "Can only write to an OUTPUT pin");
  }
  binary_pin_values_[pin] = value;
}

void LEDIndicators::set_pin_mode(BinaryPin pin, GPIO::PinMode mode) {
  binary_pin_modes_[pin] = mode;
}

#endif
