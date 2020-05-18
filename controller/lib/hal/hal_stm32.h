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

This file implements the HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.  Details of the processor's
peripherals can be found in the reference manual for that processor:
   https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

Details specific to the ARM processor used in this chip can be found in
the programmer's manual for the processor available here:
   https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf

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
#define INT_VEC_TIMER15 0xA0
#define INT_VEC_UART2 0x0D8
#define INT_VEC_UART3 0x0DC
#define INT_VEC_DMA1_CH2 0x070
#define INT_VEC_DMA1_CH3 0x074
#define INT_VEC_TIMER6 0x118

// Interrupts on the STM32 are prioritized.  This allows
// more important interrupts to interrupt less important
// ones.  When interrupts are enabled we give a priority
// value to indicate how important the interrupt is.
// The lower the priority number the more important the
// interrupt.  The range is 0 to 15, but I only use a few
// here.  Hard faults, NMI, resets, etc have a fixed
// priority of -1, so they can always interrupt any other
// priority level.
enum class IntPriority {
  CRITICAL = 2, // Very important interrupt
  STANDARD = 5, // Normal hardware interrupts
  LOW = 8,      // Less important.  Hardware interrutps can interrupt this
};

// Handy functions for controlling GPIO
enum class GPIO_PinMode {
  IN = 0,
  OUT = 1,
  ALT = 2,
  ANALOG = 3,
};
inline void GPIO_PinMode(GPIO_Regs *const gpio, int pin, GPIO_PinMode mode) {
  gpio->mode &= ~(3 << (pin * 2));
  gpio->mode |= ((int)mode << (pin * 2));
}

enum class GPIO_OutType { PUSHPULL = 0, OPENDRAIN = 1 };
inline void GPIO_OutType(GPIO_Regs *const gpio, int pin, GPIO_OutType outType) {
  if (outType == GPIO_OutType::OPENDRAIN)
    gpio->outType |= 1 << pin;
  else
    gpio->outType &= ~(1 << pin);
}

inline void GPIO_PinAltFunc(GPIO_Regs *const gpio, int pin, int func) {
  GPIO_PinMode(gpio, pin, GPIO_PinMode::ALT);

  int x = (pin < 8) ? 0 : 1;
  gpio->alt[x] |= (func << ((pin & 7) * 4));
}

inline void GPIO_SetPin(GPIO_Regs *const gpio, int pin) {
  gpio->set = static_cast<SREG>(1 << pin);
}

inline void GPIO_ClrPin(GPIO_Regs *const gpio, int pin) {
  gpio->clr = static_cast<SREG>(1 << pin);
}

inline int GPIO_GetPin(GPIO_Regs *const gpio, int pin) {
  return (gpio->inDat & (1 << pin)) ? 1 : 0;
}

inline void GPIO_PullUp(GPIO_Regs *const gpio, int pin) {
  uint32_t x = gpio->pullUpDn & ~(3 << (2 * pin));
  x |= 1 << (2 * pin);
  gpio->pullUpDn = x;
}

inline void GPIO_PullDn(GPIO_Regs *const gpio, int pin) {
  uint32_t x = gpio->pullUpDn & ~(3 << (2 * pin));
  x |= 2 << (2 * pin);
  gpio->pullUpDn = x;
}

#endif
