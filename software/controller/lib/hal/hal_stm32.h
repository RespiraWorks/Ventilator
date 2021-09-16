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

#pragma once

#if !defined(BARE_STM32)
#error \
    "the header stm32.h is part of the HAL for the BARE_STM32 build.  It should only be used as part of that build"
#endif

#include <cstdint>

#include "hal_stm32_regs.h"

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
