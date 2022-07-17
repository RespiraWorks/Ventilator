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

#include "sys_control.h"

#include <cstdint>

// The System control block (SCB) provides system implementation information, and system control.
// This includes configuration, control, and reporting of the system exceptions.
// Most of this is defined in [PM] 4.4 (pg 221), except the ones marked otherwise
struct SysControlStruct {
  uint32_t reserved0;               // 0xE000E000
  uint32_t int_type;                // 0xE000E004 What is this ????? And WTF is this from ?????
  uint32_t aux_control;             // 0xE000E008 ACTLR
  uint32_t reserved1;               // 0xE000E00C
  uint32_t sys_tick[3];             // 0xE000E010 SysTick timer registers [PM] 4.5 (pg 246)
  uint32_t reserved2[57];           // 0xE000E014
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
  uint32_t reserved3;               // 0xE000ED30
  uint32_t mm_fault_address;        // 0xE000ED34 MMAR
  uint32_t bus_fault_addr;          // 0xE000ED38 BFAR
  uint32_t reserved4[19];           // 0xE000ED3C
  uint32_t coproc_access_control;   // 0xE000ED88 CPACR [PM] 4.6 (pg 252)
};
typedef volatile SysControlStruct SysControlReg;
inline SysControlReg *const SysControlBase = reinterpret_cast<SysControlReg *>(0xE000E000);

void SysControl::enable_fpu() {
  // The system control registers are documented in [PM] chapter 4.
  // Details on enabling the FPU are in section 4.6.6.

  /// \todo define magic value
  SysControlBase->coproc_access_control = 0x00F00000;
}

void SysControl::reset_processor() {
  // Note that the system control registers are a standard ARM peripheral they are documented in the
  // [PM] rather than the [RM]. The register we use to reset the system is called the
  // "Application interrupt and reset control register (AIRCR)"

  /// \todo define magic value
  SysControlBase->app_interrupt = 0x05FA0004;
}
