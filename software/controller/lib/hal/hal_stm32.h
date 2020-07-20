/* Copyright 2020, RespiraWorks

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

#ifndef HAL_STM32_H_
#define HAL_STM32_H_

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
// These can be found in the NVIC chapter (chapter 12) of the
// processor reference manual
enum class InterruptVector {
  DMA1_CH2 = 0x70,
  DMA1_CH3 = 0x074,
  TIMER15 = 0xA0,
  SPI1 = 0xCC,
  UART2 = 0x0D8,
  UART3 = 0x0DC,
  TIMER6 = 0x118,
  DMA2_CH3 = 0x128,
};

// Handy functions for controlling GPIO

// Each pin has a 2-bit mode value that can be set using this function.
// Pin 0 mode is in bits 0-1, pin 1 in 2-3, etc.  ([RM] 8.4.1)
enum class GPIO_PinMode {
  IN = 0,
  OUT = 1,
  ALT = 2,
  ANALOG = 3,
};

inline void GPIO_PinMode(GPIO_Regs *gpio, int pin, GPIO_PinMode mode) {
  gpio->mode &= ~(0b11 << (pin * 2));
  gpio->mode |= (static_cast<int>(mode) << (pin * 2));
}

// Value for GPIO{A,B,...E,H}_OTYPER ([RM] 8.4.2)
enum class GPIO_OutType { PUSHPULL = 0, OPENDRAIN = 1 };

inline void GPIO_OutType(GPIO_Regs *gpio, int pin, GPIO_OutType outType) {
  if (outType == GPIO_OutType::OPENDRAIN)
    gpio->outType |= 1 << pin;
  else
    gpio->outType &= ~(1 << pin);
}

// Output pin speeds are set using two consecutive bits / pin.
enum class GPIO_OutSpeed { LOW = 0, MEDIUM = 1, HIGH = 2, SMOKIN = 3 };
inline void GPIO_OutSpeed(GPIO_Regs *gpio, int pin, GPIO_OutSpeed speed) {
  int S = static_cast<int>(speed);
  gpio->outSpeed &= ~(0b11 << (2 * pin));
  gpio->outSpeed |= (S << (2 * pin));
}

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
inline void GPIO_PinAltFunc(GPIO_Regs *gpio, int pin, int func) {
  GPIO_PinMode(gpio, pin, GPIO_PinMode::ALT);

  int x = (pin < 8) ? 0 : 1;
  gpio->alt[x] |= (func << ((pin & 0b111) * 4));
}

// Set a specific output pin
inline void GPIO_SetPin(GPIO_Regs *gpio, int pin) {
  gpio->set = static_cast<SREG>(1 << pin);
}

// Clear a specific output pin
inline void GPIO_ClrPin(GPIO_Regs *gpio, int pin) {
  gpio->clr = static_cast<SREG>(1 << pin);
}

// Return the current value of an input pin
inline int GPIO_GetPin(GPIO_Regs *gpio, int pin) {
  return (gpio->inDat & (1 << pin)) ? 1 : 0;
}

// This adds a pull-up resistor to an input pin
inline void GPIO_PullUp(GPIO_Regs *gpio, int pin) {
  uint32_t x = gpio->pullUpDn & ~(3 << (2 * pin));
  x |= 1 << (2 * pin);
  gpio->pullUpDn = x;
}

// This adds a pull-down resistor to an input pin
inline void GPIO_PullDn(GPIO_Regs *gpio, int pin) {
  uint32_t x = gpio->pullUpDn & ~(3 << (2 * pin));
  x |= 2 << (2 * pin);
  gpio->pullUpDn = x;
}

#endif
