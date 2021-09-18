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

// Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md

#include "clocks.h"

// Same memory layout pattern for reset, enable and sleep [RM] 6.4.9-26
// This is for blocks of 32 bits, thus advancing by 0x04 bytes
enum class PeripheralBlock : uint8_t {
  AHB1 = 0,
  AHB2 = 1,
  AHB3 = 2,
  // Skip 3
  APB1R1 = 4,
  APB1R2 = 5,
  APB2 = 6,
  // Skip 7
};

// Reset and clock control (RCC) registers
// [RM] 6.4 (pg 192) and registers map in Table 32 [RM] 6.4.32 (pg 243-)
struct RccStruct {
  uint32_t clock_control;      // 0x00 RCC_CR [RM] 6.4.1 (pg 192)
  uint32_t clock_calibration;  // 0x04 RCC_ICSCR [RM] 6.4.2 (pg 196)
  uint32_t clock_config;       // 0x08 RCC_CFGR [RM] 6.4.3 (pg 196)
  uint32_t pll_config;         // 0x0C RCC_PLLCFGR [RM] 6.4.4 (pg 198)
  uint32_t pll_sai1_config;    // 0x10 RCC_PLLSAI1CFGR [RM] 6.4.5 (pg 201)
  uint32_t reserved0;
  uint32_t clock_interrupt_enable;  // 0x18 RCC_CIER [RM] 6.4.6 (pg 204)
  uint32_t clock_interrupt_flag;    // 0x1C RCC_CIFR [RM] 6.4.7 (pg 206)
  uint32_t clock_interrupt_clear;   // 0x20 RCC_CICR [RM] 6.4.8 (pg 207)
  uint32_t reserved1;
  uint32_t peripheral_reset[8];               // 0x28 [RM] 6.4.9-14 (pg 208-216)
  uint32_t peripheral_clock_enable[8];        // 0x48 [RM] 6.4.15-20 (pg 216-225)
  uint32_t peripheral_sleep_clock_enable[8];  // 0x68 [RM] 6.4.21-26 (pg 225-234)
  uint32_t independent_clock_config;          // 0x88 [RM] 6.4.27 (pg 234)
  uint32_t reserved2;
  uint32_t backup;                     // 0x90 RCC_BDCR [RM] 6.4.28 (pg 237)
  uint32_t status;                     // 0x94 RCC_CSR [RM] 6.4.29 (pg 239)
  uint32_t recovery;                   // 0x98 RCC_CRRCR [RM] 6.4.30 (pg 241)
  uint32_t independent_clock_config2;  // 0x9C RCC_CCIPR2 [RM] 6.4.31 (pg 242)
};

typedef volatile RccStruct RccReg;

inline RccReg *const RccBase = reinterpret_cast<RccReg *>(0x40021000);

struct PeripheralMapping {
  PeripheralBlock offset;
  uint8_t bit;
};

PeripheralMapping get_peripheral_mapping(const PeripheralID id) {
  switch (id) {
    // [RM] 6.4.9, 6.4.15, 6.4.21
    case PeripheralID::TSC:
      return {PeripheralBlock::AHB1, 16};
    case PeripheralID::CRC:
      return {PeripheralBlock::AHB1, 12};
    case PeripheralID::Flash:
      return {PeripheralBlock::AHB1, 8};
    case PeripheralID::DMA2:
      return {PeripheralBlock::AHB1, 1};
    case PeripheralID::DMA1:
      return {PeripheralBlock::AHB1, 0};
    // [RM] 6.4.10, 6.4.16, 6.4.22
    //   1. only on STM32L42xxx, STM32L44xxx, STM32L46xxx
    //   2. not on STM32L41xxx, STM32L42xxx, STM32L432xx and STM32L442xx
    case PeripheralID::RNG:
      return {PeripheralBlock::AHB2, 18};
    case PeripheralID::AES:
      return {PeripheralBlock::AHB2, 16};  // note [1] above
    case PeripheralID::ADC:
      return {PeripheralBlock::AHB2, 13};
    case PeripheralID::GPIOH:
      return {PeripheralBlock::AHB2, 7};
    case PeripheralID::GPIOE:
      return {PeripheralBlock::AHB2, 4};  // note [2] above
    case PeripheralID::GPIOD:
      return {PeripheralBlock::AHB2, 3};  // note [2] above
    case PeripheralID::GPIOC:
      return {PeripheralBlock::AHB2, 2};
    case PeripheralID::GPIOB:
      return {PeripheralBlock::AHB2, 1};
    case PeripheralID::GPIOA:
      return {PeripheralBlock::AHB2, 0};
    // [RM] 6.4.11, 6.4.17, 6.4.23
    case PeripheralID::QSPI:
      return {PeripheralBlock::AHB3, 8};
    // [RM] 6.4.12, 6.4.18, 6.4.24
    //   1. Not available on STM3L41xxx and STM32L42xxx devices
    //   2. Available on STM32L4x2xx and STM32L4x3xx devices only.
    //   3. Not available on STM32L432xx and STM32L442xx devices.
    //   4. Available on STM32L45xxx and STM32L46xxx devices only.
    //   5. Available on STM32L4x3xx devices only.
    //   6. Available on STM32L43xxx and STM32L44xxx devices only.
    case PeripheralID::LowPowerTimer1:
      return {PeripheralBlock::APB1R1, 31};
    case PeripheralID::OPAMP:
      return {PeripheralBlock::APB1R1, 30};
    case PeripheralID::DAC1:
      return {PeripheralBlock::APB1R1, 29};  // See note 1 above
    case PeripheralID::Power:
      return {PeripheralBlock::APB1R1, 28};
    case PeripheralID::USBFS:
      return {PeripheralBlock::APB1R1, 26};  // See note 2 above
    case PeripheralID::CAN1:
      return {PeripheralBlock::APB1R1, 25};  // See note 1 above
    case PeripheralID::CRS:
      return {PeripheralBlock::APB1R1, 24};
    case PeripheralID::I2C3:
      return {PeripheralBlock::APB1R1, 23};
    case PeripheralID::I2C2:
      return {PeripheralBlock::APB1R1, 22};  // Note 3
    case PeripheralID::I2C1:
      return {PeripheralBlock::APB1R1, 21};
    case PeripheralID::UART4:
      return {PeripheralBlock::APB1R1, 19};  // Note 4
    case PeripheralID::USART3:
      return {PeripheralBlock::APB1R1, 18};  // Note 3
    case PeripheralID::USART2:
      return {PeripheralBlock::APB1R1, 17};
    case PeripheralID::SPI3:
      return {PeripheralBlock::APB1R1, 15};  // Note 1
    case PeripheralID::SPI2:
      return {PeripheralBlock::APB1R1, 14};
    case PeripheralID::WindowWatchdog:
      return {PeripheralBlock::APB1R1, 11};  // Enable clock or sleep only, not reset
    case PeripheralID::RTCAPB:
      return {PeripheralBlock::APB1R1, 10};  // Enable clock or sleep only, not reset
    case PeripheralID::LCD:
      return {PeripheralBlock::APB1R1, 9};  // Note 5
    case PeripheralID::Timer7:
      return {PeripheralBlock::APB1R1, 5};  // Note 6
    case PeripheralID::Timer6:
      return {PeripheralBlock::APB1R1, 4};
    case PeripheralID::Timer3:
      return {PeripheralBlock::APB1R1, 1};  // Note 4
    case PeripheralID::Timer2:
      return {PeripheralBlock::APB1R1, 0};
    // [RM] 6.4.13, 6.4.19, 6.4.25
    //   1. Not available on STM32L41xxx and STM32L42xxx devices.
    //   2. Available on STM32L45xxx and STM32L46xxx devices only.
    case PeripheralID::LowPowerTimer2:
      return {PeripheralBlock::APB1R2, 5};
    case PeripheralID::SingleWirePMI1:
      return {PeripheralBlock::APB1R2, 2};  // Note 1
    case PeripheralID::I2C4:
      return {PeripheralBlock::APB1R2, 1};  // Note 2
    case PeripheralID::LowPowerUART1:
      return {PeripheralBlock::APB1R2, 0};
    // [RM] 6.4.14, 6.4.20, 6.4.26
    //   1. Available on STM32L45xxx and STM32L46xxx devices only.
    //   2. Not available on STM3L41xxx and STM32L42xxx devices.
    //   3. Not available on STM3L41xxx and STM32L42xxx, STM32L432 and STM32L442 devices.
    case PeripheralID::DFSDM1:
      return {PeripheralBlock::APB2, 24};  // Note 1
    case PeripheralID::SAI1:
      return {PeripheralBlock::APB2, 21};  // Note 2
    case PeripheralID::Timer16:
      return {PeripheralBlock::APB2, 17};
    case PeripheralID::Timer15:
      return {PeripheralBlock::APB2, 16};
    case PeripheralID::USART1:
      return {PeripheralBlock::APB2, 14};
    case PeripheralID::SPI1:
      return {PeripheralBlock::APB2, 12};
    case PeripheralID::Timer1:
      return {PeripheralBlock::APB2, 11};
    case PeripheralID::SDMMC1:
      return {PeripheralBlock::APB2, 10};  // Note 3
    case PeripheralID::Firewall:
      return {PeripheralBlock::APB2, 7};  // Clock enable only, not reset or sleep
    case PeripheralID::SYSCFG:
      return {PeripheralBlock::APB2, 0};
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}

// Enable the clock of the requested peripheral
// 0x48 [RM] 6.4.15-20 (pg 216-225)
void enable_peripheral_clock(const PeripheralID id) {
  auto mapping = get_peripheral_mapping(id);
  RccBase->peripheral_clock_enable[static_cast<uint8_t>(mapping.offset)] |= (1 << mapping.bit);
}

void configure_pll() {
  // We use the MSI clock as the source for the PLL.
  // MSI clock is running at its default frequency of 4MHz.
  //
  // The PLL can generate several clocks with somewhat less than descriptive names in the [RM].
  // These clocks are:
  //   P clock - Used for the SAI peripheral.  Not used here
  //   Q clock - 48MHz output clock used for USB.  Not used here.
  //   R clock - This is the main system clock.  We care about this one.
  //
  // When configuring the PLL there are several constants programmed into the PLL register to set
  // the frequency of the internal VCO. These constants are called N and M in the [RM]:
  //
  // Fin = 4MHz
  // Fvco = Fin * (N/M)
  //
  // Legal range for Fvco is 96MHz to 344MHz according to [DS].
  // We shall use 160MHz for Fvco and divide by 2 to get an 80MHz output clock
  //
  // See [RM] chapter 6
  int n = 40;
  int m = 1;
  RccReg *rcc = RccBase;
  rcc->pll_config = 0x01000001 | (n << 8) | ((m - 1) << 4);

  // Turn on the PLL
  rcc->clock_control |= 0x01000000;

  // Wait for the PLL ready indication
  while (!(rcc->clock_control & 0x02000000)) {
  }

  // Set PLL as system clock
  rcc->clock_config = 0x00000003;

  // Use system clock as the A/D clock
  rcc->independent_clock_config = 0x30000000;
};
