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
This file implements the HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.

Reference abbreviations [RM], [DS], etc are defined in hal/README.md.
*/

#ifndef HAL_STM32_H
#define HAL_STM32_H

#if !defined(BARE_STM32)
#error                                                                         \
    "the header stm32.h is part of the HAL for the BARE_STM32 build.  It should only be used as part of that build"
#endif

#include "hal_stm32_regs.h"
#include <stdint.h>

#define CPU_FREQ_MHZ 80
#define CPU_FREQ (CPU_FREQ_MHZ * 1000000)

// Interrupt vectors that we currently use.
// The values here are the offsets into the interrupt table.
// These can be found in [RM] chapter 12 (NVIC)
enum class InterruptVector {
  Dma1Channel2 = 0x70,
  Dma1Channel3 = 0x074,
  Timer15 = 0xA0,
  I2c1Event = 0xBC,
  I2c1Error = 0xC0,
  Spi1 = 0xCC,
  Uart2 = 0x0D8,
  Uart3 = 0x0DC,
  Timer6 = 0x118,
  Dma2Channel3 = 0x128,
  Dma2Channel6 = 0x150,
  Dma2Channel7 = 0x157,
};

// Handy functions for controlling GPIO

// Each pin has a 2-bit mode value that can be set using this function.
// Pin 0 mode is in bits 0-1, pin 1 in 2-3, etc.  ([RM] 8.4.1)
enum class GPIOPinMode {
  Input = 0,
  Output = 1,
  AlternateFunction = 2,
  Analog = 3,
};

inline void GpioPinMode(GpioReg *gpio, int pin, GPIOPinMode mode) {
  gpio->mode &= ~(0b11 << (pin * 2));
  gpio->mode |= (static_cast<int>(mode) << (pin * 2));
}

// Value for GPIO{A,B,...E,H}_OTYPER ([RM] 8.4.2)
enum class GPIOOutType { PushPull = 0, OpenDrain = 1 };

inline void GpioOutType(GpioReg *gpio, int pin, GPIOOutType output_type) {
  if (output_type == GPIOOutType::OpenDrain)
    gpio->output_type |= 1 << pin;
  else
    gpio->output_type &= ~(1 << pin);
}

// Output pin speeds are set using two consecutive bits / pin.
enum class GPIOOutSpeed { Slow = 0, Medium = 1, Fast = 2, Smoking = 3 };
inline void GpioOutSpeed(GpioReg *gpio, int pin, GPIOOutSpeed speed) {
  int s = static_cast<int>(speed);
  gpio->output_speed &= ~(0b11 << (2 * pin));
  gpio->output_speed |= (s << (2 * pin));
}

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
inline void GpioPinAltFunc(GpioReg *gpio, int pin, int func) {
  GpioPinMode(gpio, pin, GPIOPinMode::AlternateFunction);

  int x = (pin < 8) ? 0 : 1;
  gpio->alternate_function[x] |= (func << ((pin & 0b111) * 4));
}

// Set a specific output pin
inline void GpioSetPin(GpioReg *gpio, int pin) {
  gpio->set = static_cast<uint16_t>(1 << pin);
}

// Clear a specific output pin
inline void GpioClrPin(GpioReg *gpio, int pin) {
  gpio->clear = static_cast<uint16_t>(1 << pin);
}

// Return the current value of an input pin
inline int GpioGetPin(GpioReg *gpio, int pin) {
  return (gpio->input_data & (1 << pin)) ? 1 : 0;
}

// This adds a pull-up resistor to an input pin
inline void GpioPullUp(GpioReg *gpio, int pin) {
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin));
  x |= 1 << (2 * pin);
  gpio->pullup_pulldown = x;
}

// This adds a pull-down resistor to an input pin
inline void GpioPullDn(GpioReg *gpio, int pin) {
  uint32_t x = gpio->pullup_pulldown & ~(3 << (2 * pin));
  x |= 2 << (2 * pin);
  gpio->pullup_pulldown = x;
}

#endif
