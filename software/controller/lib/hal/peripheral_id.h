/* Copyright 2020-2022, RespiraWorks

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

/// \todo this may not be generic enough; what processors is this true for?

// Please read manual carefully, as not all peripherals ara available on all processors
// [RM] 6.4.9-26 (pg 208-234)
// \todo likely needs an "Invalid" value for default initializations
enum class PeripheralID {
  ADC,
  AES,  // AES accelerator
  CAN1,
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
  SAI1,
  SDMMC1,
  SPI1,
  SPI2,
  SPI3,
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
