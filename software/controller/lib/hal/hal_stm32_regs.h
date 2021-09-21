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

/*
The structures below represent the STM32 registers used
to configure various modules (like timers, serial ports, etc).

Detailed information on these modules and the registers
used to configure them can be found in [RM]

Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
*/

// The System control block (SCB) provides system implementation information, and system control.
// This includes configuration, control, and reporting of the system exceptions.
// Most of this is defined in [PM] 4.4 (pg 221), except the ones marked otherwise
struct SysControlStruct {
  uint32_t reserved0;
  uint32_t int_type;     // 0xE000E004 What is this ????? And WTF is this from ?????
  uint32_t aux_control;  // 0xE000E008 ACTLR
  uint32_t reserved1;
  uint32_t sys_tick[3];  // 0xE000E010 SysTick timer registers [PM] 4.5 (pg 246)
  uint32_t reserved2[57];
  uint32_t nvic[768];               // 0xE000E100 NVIC [RM] 4.3 (pg 208)
  uint32_t cpu_id;                  // 0xE000ED00 CPUID
  uint32_t interrupt_control;       // 0xE000ED04 ICSR
  uint32_t vector_table;            // 0xE000ED08 VTOR
  uint32_t app_interrupt;           // 0xE000ED0C AIRCR
  uint32_t system_control;          // 0xE000ED10 SCR
  uint32_t config_control;          // 0xE000ED14 CCR
  uint32_t system_priority[3];      // 0xE000ED18 SHPR1-3
  uint32_t system_handler_control;  // 0xE000ED24 SHCSR
  uint32_t fault_status;            // 0xE000ED28 CFSR/MMSR/BFSR/UFSR
  uint32_t hard_fault_status;       // 0xE000ED2C HFSR
  uint32_t reserved3;
  uint32_t mm_fault_address;  // 0xE000ED34 MMAR
  uint32_t bus_fault_addr;    // 0xE000ED38 BFAR
  uint32_t reserved4[19];
  uint32_t coproc_access_control;  // 0xE000ED88 CPACR [PM] 4.6 (pg 252)
};
typedef volatile SysControlStruct SysControlReg;
inline SysControlReg *const SysControlBase = reinterpret_cast<SysControlReg *>(0xE000E000);

// Watchdog timer
// [RM] 32.4 Watchdog Registers (pg 1016)
struct WatchdogStruct {
  uint32_t key;        // Key register [RM] 32.4.1
  uint32_t prescaler;  // Prescale register [RM] 32.4.2
  uint32_t reload;     // Reload register [RM] 32.4.3
  uint32_t status;     // Status register [RM] 32.4.4
  uint32_t window;     // Window register [RM] 32.4.5
};
typedef volatile WatchdogStruct WatchdogReg;
inline WatchdogReg *const WatchdogBase = reinterpret_cast<WatchdogReg *>(0x40003000);

// CRC calculation unit
// [RM] 14.4 CRC Registers (pg 341)
struct CrcStruct {
  uint32_t data;     // Data register [RM] 14.4.1
  uint32_t scratch;  // Independent data register [RM] 14.4.2
  uint32_t control;  // Control register [RM] 14.4.3
  uint32_t reserved;
  uint32_t init;        // Initial CRC value [RM] 14.4.4
  uint32_t polynomial;  // CRC polynomial [RM] 14.4.5
};
typedef volatile CrcStruct CrcReg;
inline CrcReg *const CrcBase = reinterpret_cast<CrcReg *>(0x40023000);
