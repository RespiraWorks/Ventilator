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

SOURCES OF TRUTH
================

This code is implemented on the basis of the following specifications.
In comments that accompany the code, they will be referred to by letter
abbreviation and some section or page number.

[DS] Data Sheet for Nucleo board we are using:
  https://www.st.com/resource/en/datasheet/stm32l452re.pdf

[RM] Reference Manual for the STM32L452 processor.
   https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
   Details of the processor's peripherals can be found here.

[PM] Programmer's manual for the Cortex M4 line of processors:
   https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf

[PCB] RespiraWorks custom printed circuit board schematic
  https://github.com/RespiraWorks/pcbreathe/blob/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2/20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-DWG-SCH.PDF
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

/* [RM] 8.4.1 GPIO port mode register (GPIOx_MODER) (x  =A to E and H) (pg 267)
    Bits 31:0 MODE[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
      00: Input mode
      01: General purpose output mode
      10: Alternate function mode
      11: Analog mode (reset state)
  
    pin * 2        bit pair position on GPIOx_MODER corresponding to pin
    2^(pin * 2)    binary representation of bit position
*/
enum class GPIO_PinMode {
  IN = 0,
  OUT = 1,
  ALT = 2,
  ANALOG = 3,
};
/* Set GPIO Pin Mode:
    (3 * 2^(pin * 2)            writes 1 to second value in bit pair of 
    mode &= ~(3 * 2^(pin * 2)   resets bit pair corresponding to pin
    mode |= 2^(pin * 2)         writes passed mode to register
*/
inline void GPIO_PinMode(GPIO_Regs *gpio, int pin, GPIO_PinMode mode) {
  gpio->mode &= ~(3 << (pin * 2));
  gpio->mode |= (static_cast<int>(mode) << (pin * 2));
}

/* [RM] 8.4.2 GPIO port output type register (GPIOx_OTYPER) (x = A to E and H) (pg 268)
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
      0: Output push-pull (reset state)
      1: Output open-drain
*/
enum class GPIO_OutType { PUSHPULL = 0, OPENDRAIN = 1 };
/*
  Set GPIO Output Type: 
    outType |= 2^(pin)       writes 1 to register
    outType &= ~(2^(pin))    writes 0 to register
*/
inline void GPIO_OutType(GPIO_Regs *gpio, int pin, GPIO_OutType outType) {
  if (outType == GPIO_OutType::OPENDRAIN)
    gpio->outType |= 1 << pin;
  else
    gpio->outType &= ~(1 << pin);
}

/* [RM] 8.4.3 GPIO port output speed register (GPIOx_OSPEEDR) (pg 268)
    Bits 31:0 OSPEED[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
      00: Low speed
      01: Medium speed
      10: High speed
      11: Very high speed
  See GPIO_PinMode for similar bitwise math explanation

  Set GPIO Port Output speed:
*/
enum class GPIO_OutSpeed { LOW = 0, MEDIUM = 1, HIGH = 2, SMOKIN = 3 };
inline void GPIO_OutSpeed(GPIO_Regs *gpio, int pin, GPIO_OutSpeed speed) {
  int S = static_cast<int>(speed);
  gpio->outSpeed &= ~(3 << (2 * pin));
  gpio->outSpeed |= (S << (2 * pin));
}

/* [DS] Table 17. Alternate function AF0 to AF7 (pg 74)
   [DS] Table 18. Alternate function AF8 to AF15 (pg 80)
   See above references for GPIO pin alternate functions

   [RM] 8.4.9 GPIO alternate function low register (GPIOx_AFRL)
   (x = A to E and H) (pg 272)
   [RM] 8.4.10 GPIO alternate function high register (GPIOx_AFRH)
   (x = A to E and H) (pg 272)
      Bits 31:0 AFSEL[7:0][3:0]: Alternate function selection 
      for port x I/O pin y (y = 7 to 0)
      Bits 31:0 AFSEL[15:8][3:0]: Alternate function selection 
      for port x I/O pin y (y = 15 to 8)
        0000: AF0
        0001: AF1
        0010: AF2
        ... 
        1111: AF15

   Set GPIO Pin Alternate Function:
    Write func * 2^((pin & 7) * 4) to pin's appropriate register bit
*/
inline void GPIO_PinAltFunc(GPIO_Regs *gpio, int pin, int func) {
  GPIO_PinMode(gpio, pin, GPIO_PinMode::ALT);

  int x = (pin < 8) ? 0 : 1;
  gpio->alt[x] |= (func << ((pin & 7) * 4));
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

/* [RM] 8.4.4 GPIO port pull-up/pull-down register (GPIOx_PUPDR)
    (x = A to E and H) (pg 268)
    Bits 31:0 PUPD[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
      00: No pull-up, pull-down
      01: Pull-up
      10: Pull-down
      11: Reserved
    See GPIO_OutSpeed for similar bitwise math explanation
*/
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
