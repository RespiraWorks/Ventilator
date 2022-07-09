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

/// \TODO create more of an abstraction than just providing registers

#include <cstdint>

// Timer Register
// NOTE: Offset values and applicable registers depend on timer used
struct TimerStruct {
  union {
    struct {
      uint32_t counter_enable : 1;
      uint32_t update_disable : 1;
      uint32_t update_request_source : 1;
      uint32_t one_pulse_mode : 1;
      uint32_t direction : 1;
      uint32_t center_aligned_mode : 2;
      uint32_t auto_reload_preload : 1;
      uint32_t clock_division : 2;
      uint32_t reserved : 1;
      uint32_t uif_remapping : 1;
      uint32_t reserved2 : 20;
    } bitfield;
    uint32_t full_reg;
  } control_reg1;
  uint32_t control2;
  uint32_t slave_control;
  uint32_t interrupts_enable;
  uint32_t status;
  uint32_t event;
  uint32_t capture_compare_mode[2];
  uint32_t capture_compare_enable;
  // The topmost bit of counter will contain contain UIFCOPY if UIFREMAP is
  // enabled, but *this register should not be decomposed into a bitfield
  // struct*.  The idea behind UIFREMAP is to read the counter plus the UIFCOPY
  // value atomically, in one go.
  uint32_t counter;
  uint32_t prescaler;
  uint32_t auto_reload;
  uint32_t repeat;
  uint32_t capture_compare[4];
  uint32_t dead_time;
  uint32_t dma_control;
  uint32_t dma_address;
  uint32_t option1;
  uint32_t capture_compare_mode3;
  uint32_t capture_compare_mode5;
  uint32_t capture_compare_mode6;
  uint32_t option2;
  uint32_t option3;
};
typedef volatile TimerStruct TimerReg;

// [RM] 26 Advanced-control Timers (TIM1) (pg 718)
inline TimerReg *const Timer1Base = reinterpret_cast<TimerReg *>(0x40012C00);
// [RM] 27 General-purpose Timers (TIM2/TIM3) (pg 817)
inline TimerReg *const Timer2Base = reinterpret_cast<TimerReg *>(0x40000000);
inline TimerReg *const Timer3Base = reinterpret_cast<TimerReg *>(0x40000400);
// [RM] 29 Basic Timers (TIM6/TIM7) (pg 968)
inline TimerReg *const Timer6Base = reinterpret_cast<TimerReg *>(0x40001000);
inline TimerReg *const Timer7Base = reinterpret_cast<TimerReg *>(0x40001400);
// [RM] 28 General-purpose Timers (TIM15/TIM16) (pg 887)
inline TimerReg *const Timer15Base = reinterpret_cast<TimerReg *>(0x40014000);
inline TimerReg *const Timer16Base = reinterpret_cast<TimerReg *>(0x40014400);
