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

#pragma once

#include <cstdint>

/******************************************************************
 * General Purpose I/O support. [RM] Chapter 8
 *
 * Please refer to the PCB schematic as the ultimate source of which
 * pin is used for which function.  A less definitive, but perhaps
 * easier to read version is available in [PCBsp]
 *****************************************************************/

namespace GPIO {

/// \TODO: Move these mappings elsewhere
enum class Port : uint32_t {
  A = 0x48000000,
  B = 0x48000400,
  C = 0x48000800,
  D = 0x48000C00,
  E = 0x48001000,
  H = 0x48001C00,
};

// Value for GPIOx_MODER ([RM] 8.4.1)
enum class PinMode : uint8_t {
  Input = 0b00,
  Output = 0b01,
  AlternateFunction = 0b10,
  Analog = 0b11,
};

// Value for GPIOx_OTYPER ([RM] 8.4.2)
enum class OutType : uint8_t { PushPull = 0b0, OpenDrain = 0b1 };

// Value for GPIOx_OSPEEDR ([RM] 8.4.3)
enum class OutSpeed : uint8_t { Slow = 0b00, Medium = 0b01, Fast = 0b10, Smoking = 0b11 };

// [DS] Tables 17-18 (pg 76-86)
enum class AlternativeFunction : uint32_t {
  AF0 = 0,    //< SYS_AF
  AF1 = 1,    //< TIM1/TIM2/LPTIM1
  AF2 = 2,    //< I2C4/TIM1/TIM2/TIM3
  AF3 = 3,    //< I2C4/USART2/CAN1/TIM1
  AF4 = 4,    //< I2C1/I2C2/I2C3/I2C4
  AF5 = 5,    //< SPI1/SPI2/I2C4
  AF6 = 6,    //< SPI3/DFSDM/COMP1
  AF7 = 7,    //< USART1/USART2/USART3
  AF8 = 8,    //< UART4/LPUART1/CAN1
  AF9 = 9,    //< CAN1/TSC
  AF10 = 10,  //< CAN1/USB/QUADSPI
  AF11 = 11,  //< -
  AF12 = 12,  //< SDMMC1/COMP1/COMP2
  AF13 = 13,  //< SAI1
  AF14 = 14,  //< TIM2/TIM15/TIM16/LPTIM2
  AF15 = 15   //< EVENTOUT
};

void enable_all_clocks();

void pin_mode(Port port, uint8_t pin, PinMode mode);

void output_type(Port port, uint8_t pin, OutType output_type);

void output_speed(Port port, uint8_t pin, OutSpeed speed);

// Many GPIO pins can be repurposed with an alternate function
// See Table 17 and 18 [DS] for alternate functions
// See [RM] 8.4.9 and 8.4.10 for GPIO alternate function selection
void alternate_function(Port port, uint8_t pin, AlternativeFunction func);

// Set a specific output pin
void set_pin(Port port, uint8_t pin);

// Clear a specific output pin
void clear_pin(Port port, uint8_t pin);

// Return the current value of an input pin ([RM] 8.4.5)
bool get_pin(Port port, uint8_t pin);

// This adds a pull-up resistor to an input pin
void pull_up(Port port, uint8_t pin);

// This adds a pull-down resistor to an input pin
void pull_down(Port port, uint8_t pin);

}  // namespace GPIO
