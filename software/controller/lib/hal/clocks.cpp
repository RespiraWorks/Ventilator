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
  uint32_t peripheral_reset[8];         // 0x28 [RM] 6.4.9-14 (pg 208-216)
  uint32_t peripheral_clock_enable[8];  // 0x48 [RM] 6.4.15-20 (pg 216-225)
  uint32_t sleep_clock_enable[8];       // 0x68 [RM] 6.4.21-26 (pg 225-234)
  uint32_t independent_clock_config;    // 0x88 [RM] 6.4.27 (pg 234)
  uint32_t reserved2;
  uint32_t backup;                     // 0x90 RCC_BDCR [RM] 6.4.28 (pg 237)
  uint32_t status;                     // 0x94 RCC_CSR [RM] 6.4.29 (pg 239)
  uint32_t recovery;                   // 0x98 RCC_CRRCR [RM] 6.4.30 (pg 241)
  uint32_t independent_clock_config2;  // 0x9C RCC_CCIPR2 [RM] 6.4.31 (pg 242)
};

typedef volatile RccStruct RccReg;

inline RccReg *const RccBase = reinterpret_cast<RccReg *>(0x40021000);

// Enable the clock of the requested peripheral
// 0x48 [RM] 6.4.15-20 (pg 216-225)
void enable_peripheral_clock(uint8_t index, uint8_t bit) {
  RccReg *rcc = RccBase;
  rcc->peripheral_clock_enable[index] |= (1 << bit);
}

void configure_pll() {
  // Enable the PLL.
  // We use the MSI clock as the source for the PLL
  // The MSI clock is running at its default frequency of
  // 4MHz.
  //
  // The PLL can generate several clocks with somewhat
  // less then descriptive names in the [RM].
  // These clocks are:
  //   P clock - Used for the SAI peripheral.  Not used here
  //   Q clock - 48MHz output clock used for USB.  Not used here.
  //   R clock - This is the main system clock.  We care about this one.
  //
  // When configuring the PLL there are several constants programmed
  // into the PLL register to set the frequency of the internal VCO
  // These constants are called N and M in the [RM]:
  //
  // Fin = 4MHz
  // Fvco = Fin * (N/M)
  //
  // Legal range for Fvco is 96MHz to 344MHz according to [DS].
  // I'll use 160MHz for Fvco and divide by 2 to get an 80MHz output clock
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
