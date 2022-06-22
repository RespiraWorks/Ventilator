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

#pragma once

#include <cstdint>

// Interrupt vectors that we currently use.
// [RM] 12.3 Table 46 (pg 321)
// The values here are the offsets into the interrupt table.
// \TODO implement mapping elsewhere, keep enum abstract
enum class InterruptVector : uint32_t {
  Dma1Channel2 = 0x70,
  Dma1Channel3 = 0x074,
  Timer15 = 0xA0,
  I2c1Event = 0xBC,
  I2c1Error = 0xC0,
  I2c2Event = 0xC4,
  I2c2Error = 0xC8,
  Spi1 = 0xCC,
  Uart1 = 0x0D4,
  Uart2 = 0x0D8,
  Uart3 = 0x0DC,
  Uart4 = 0x110,
  Timer6 = 0x118,
  Dma2Channel3 = 0x128,
  Dma2Channel6 = 0x150,
  Dma2Channel7 = 0x157,
  I2c3Event = 0x160,
  I2c3Error = 0x164,
  I2c4Event = 0x18C,
  I2c4Error = 0x190,
};

// Interrupts on the STM32 are prioritized.  This allows
// more important interrupts to interrupt less important
// ones.  When interrupts are enabled we give a priority
// value to indicate how important the interrupt is.
// The lower the priority number the more important the
// interrupt.  The range is 0 to 15, but I only use a few
// here.  Hard faults, NMI, resets, etc have a fixed
// priority of -1, so they can always interrupt any other
// priority level.
enum class InterruptPriority {
  Critical = 2,  // Very important interrupt
  Standard = 5,  // Normal hardware interrupts
  Low = 8,       // Less important.  Hardware interrupts can interrupt this
};

// This is conceptually a singleton as it functions in a bare-metal system, but is
// also a useful pattern for testing/mocking purposes.
//
// Where possible, prefer using the BlockInterrupts RAII class (see below).
//
// NOTE: Interrupts should only be disabled for short periods of time and
// only for very good reasons.  Leaving interrupts disabled for long can
// cause loss of serial data and other bad effects.
class Interrupts {
 public:
  // this is the only way to access it
  static Interrupts& singleton() {
    // will privately initialize on first call
    static Interrupts SingletonInstance;
    // will always return
    return SingletonInstance;
  }

  void EnableInterrupt(InterruptVector vec, InterruptPriority pri);

  // Interrupt enable/disable functions.
  void DisableInterrupts();
  void EnableInterrupts();

  // Returns true if interrupts are currently enabled.
  //
  // Where possible, prefer using the BlockInterrupts RAII class.
  bool InterruptsEnabled() const;

  // Return true if we are currently executing in an interrupt handler
  bool InInterruptHandler();

 private:
  // singleton assurance, because these are private
  Interrupts() = default;             // cannot default initialize
  Interrupts(Interrupts const&);      // cannot copy initialize
  void operator=(Interrupts const&);  // cannot copy assign

#if !defined(BARE_STM32)
  bool interrupts_enabled_{true};
#endif
};

// RAII class that disables interrupts.  For example:
//
// {
//   BlockInterrupts block;
//   // Interrupts are disabled until the close brace.
// }
//
// This class is reentrant, i.e. it's safe to BlockInterrupts even when
// interrupts are already disabled.
class [[nodiscard]] BlockInterrupts {
 public:
  BlockInterrupts() : active_(Interrupts::singleton().InterruptsEnabled()) {
    if (active_) {
      Interrupts::singleton().DisableInterrupts();
    }
  }

  // Not copyable or movable.  (Technically we could make this class
  // movable if necessary, but it probably isn't!)
  BlockInterrupts(const BlockInterrupts&) = delete;
  BlockInterrupts(BlockInterrupts &&) = delete;
  BlockInterrupts& operator=(const BlockInterrupts&) = delete;
  BlockInterrupts& operator=(BlockInterrupts&&) = delete;

  ~BlockInterrupts() {
    if (active_) {
      Interrupts::singleton().EnableInterrupts();
    }
  }

 private:
  bool active_;
};
