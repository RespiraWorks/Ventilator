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

#pragma once

#include <cstdint>

// Please read manual carefully, as not all peripherals ara available on all processors
// [RM] 6.4.9-26 (pg 208-234)
enum class PeripheralID {
  ADC,
  AES,   // AES accelerator
  CAN1,  // CAN1
  CRC,
  CRS,
  DAC1,
  DFSDM1,  // Digital filters for sigma-delta modulators
  DMA1,
  DMA2,
  Flash,  // Flash memory interface
  Firewall,
  GPIOA,
  GPIOB,
  GPIOC,
  GPIOD,
  GPIOE,
  GPIOH,
  I2C1,
  I2C2,
  I2C3,
  I2C4,
  LCD,
  LowPowerTimer1,
  LowPowerTimer2,
  LowPowerUART1,
  OPAMP,  // OPAMP interface
  Power,
  QSPI,    // Quad SPI
  RNG,     // Random number generator
  RTCAPB,  // RTC APB
  SAI1,    // SAI1
  SDMMC1,  // SDMMC
  SPI1,    // SPI1
  SPI2,    // SPI2
  SPI3,    // SPI3
  SingleWirePMI1,
  SYSCFG,  // SYSCFG + COMP + VREFBUF
  Timer1,
  Timer2,
  Timer3,
  Timer6,
  Timer7,
  Timer15,
  Timer16,
  TSC,  // Touch sensing controller
  UART4,
  USART1,
  USART2,
  USART3,
  USBFS,  // USB FS
  WindowWatchdog,
};

// Enable clocks to a specific peripheral.
// On the STM32 the clocks going to various peripherals on the chip are individually selectable and
// for the most part disabled on startup. Clocks to the specific peripherals need to be enabled
// through the RCC (Reset and Clock Controller) module before the peripheral can be used.
void enable_peripheral_clock(PeripheralID);

void configure_pll();
