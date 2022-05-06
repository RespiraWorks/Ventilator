/* Copyright 2020, RespiraWorks

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

#include "interrupts.h"

#if defined(BARE_STM32)

// Nested vectored interrupt controller (NVIC) [PM] 4.3 (pg 208)
struct InterruptControlStruct {
  uint32_t set_enable[32];
  uint32_t clear_enable[32];
  uint32_t set_pending[32];
  uint32_t clear_pending[32];
  uint32_t active[64];
  uint8_t priority[1024];
};
typedef volatile InterruptControlStruct InterruptControlReg;
inline InterruptControlReg *const NvicBase = reinterpret_cast<InterruptControlReg *>(0xE000E100);

// Enable an interrupt with a specified priority (0 to 15)
// See [RM] chapter 12 for more information on the NVIC.
void Interrupts::EnableInterrupt(InterruptVector vec, InterruptPriority pri) {
  InterruptControlReg *nvic = NvicBase;

  int addr = static_cast<int>(vec);

  int id = addr / 4 - 16;

  nvic->set_enable[id >> 5] = 1 << (id & 0x1F);

  // The STM32 processor implements bits 4-7 of the NVIM priority register.
  int p = static_cast<int>(pri);
  nvic->priority[id] = static_cast<uint8_t>(p << 4);
}

void Interrupts::DisableInterrupts() { asm volatile("cpsid i" ::: "memory"); }
void Interrupts::EnableInterrupts() { asm volatile("cpsie i" ::: "memory"); }
bool Interrupts::InterruptsEnabled() const {
  int ret;
  asm volatile("mrs %[output], primask" : [output] "=r"(ret));
  return ret == 0;
}

bool Interrupts::InInterruptHandler() {
  int ret;
  asm volatile("mrs %[output], ipsr" : [output] "=r"(ret));
  return ret > 0;
}

#else

void Interrupts::EnableInterrupt(InterruptVector vec, InterruptPriority pri) {}
void Interrupts::DisableInterrupts() { interrupts_enabled_ = false; }
void Interrupts::EnableInterrupts() { interrupts_enabled_ = true; }
bool Interrupts::InterruptsEnabled() const { return interrupts_enabled_; }
bool Interrupts::InInterruptHandler() { return false; }

#endif
