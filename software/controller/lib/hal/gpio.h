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

#include "adc.h"
#include "pwm.h"

/******************************************************************
 * General Purpose I/O support. [RM] Chapter 8
 *
 * Please refer to the PCB schematic as the ultimate source of which
 * pin is used for which function.  A less definitive, but perhaps
 * easier to read version is available in [PCBsp]
 *****************************************************************/

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

// Value for GPIOx_PUPDR ([RM] 8.4.4)
enum class PullType : uint8_t { None = 0b00, Up = 0b01, Down = 0b10 };

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

// Abstract pin, note that for some types of pin (most notably AlternateFunction), we only need to
// instantiate the pin so the registers are properly set, but we don't really need to keep a
// reference to the pin itself
class Pin {
 public:
  Pin(Port port, uint8_t pin, PinMode mode);

 protected:
  Register *gpio_{nullptr};
  uint8_t pin_;
  // helper functions to set properties of the pin (writes them to registers).
  void output_type(OutType output_type);              // for output or alternate pins
  void output_speed(OutSpeed speed);                  // for output or alternate pins
  void pull_type(PullType pull);                      // for input or alternate pins
  void alternate_function(AlternativeFunction func);  // for analog output or alternate pins
};

// Output pin, which can be set to 1 or cleared to 0
class DigitalOutputPin : public Pin {
 public:
  DigitalOutputPin(Port port, uint8_t pin, bool start_high = false, OutSpeed speed = OutSpeed::Slow,
                   OutType type = OutType::PushPull);
  void set();
  void clear();
#if !defined(BARE_STM32)
  bool get() const;

 private:
  bool value_;
#endif
};

// Input pins can be read as a boolean (0 or 1)
class DigitalInputPin : public Pin {
 public:
  DigitalInputPin(Port port, uint8_t pin, PullType pull = PullType::None);
  bool get() const;
#if !defined(BARE_STM32)
  void set();
  void clear();

 private:
  bool value_;
#endif
};

class AlternatePin : public Pin {
 public:
  AlternatePin(Port port, uint8_t pin, AlternativeFunction func, PullType pull = PullType::None,
               OutSpeed speed = OutSpeed::Slow, OutType type = OutType::PushPull);
};

class AnalogInputPin : public Pin {
 public:
  AnalogInputPin(Port port, uint8_t pin, ADC *adc, AdcChannel channel);
  Voltage read() const;

 private:
  ADC *adc_{nullptr};
  AdcChannel channel_;
};

class AnalogOutputPin : public Pin {
 public:
  AnalogOutputPin(Port port, uint8_t pin, const AlternativeFunction func, const Frequency pwm_freq,
                  TimerReg *timer, uint8_t channel, const PeripheralID peripheral,
                  const Frequency cpu_frequency);
  void set(float value);

 private:
  PWM pwm_;
};

}  // namespace GPIO
