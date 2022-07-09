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

#pragma once

#include "clocks.h"

/// \TODO scope these functions, maybe also have abstract interface?

// Enable clocks to a specific peripheral.
// On the STM32 the clocks going to various peripherals on the chip are individually selectable and
// for the most part disabled on startup. Clocks to the specific peripherals need to be enabled
// through the RCC (Reset and Clock Controller) module before the peripheral can be used.
void enable_peripheral_clock(PeripheralID);

void enable_gpio_clocks();

void configure_pll();
