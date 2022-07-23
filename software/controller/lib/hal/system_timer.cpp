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

#include "system_timer.h"

#if defined(BARE_STM32)

#include "clocks_stm32.h"
#include "timers_stm32.h"

void SystemTimer::initialize(PeripheralID id, InterruptVector interrupt_vector,
                             Frequency cpu_frequency) {
  id_ = id;

  // Enable the clock to the timer
  Clocks::enable_peripheral_clock(id_);

  // Just set the timer up to count every microsecond.
  auto *tmr = Timer::get_register(id_);

  // The reload register gives the number of clock ticks (100ns in our case)
  // -1 until the clock wraps back to zero and generates an interrupt. This
  // setting will cause an interrupt every 10,000 clocks or 1 millisecond
  tmr->auto_reload = 9999;

  // We set the prescaler to create a 100ns (1E-7 s) clock. This is only precise if cpu_frequency is
  // a multiple of 10MHz. This way, 100ns is an exact number of cpu clock cycles (the prescaler
  // register is that number - 1). Note that we can't use Duration type to represent 100 ns.
  tmr->prescaler = static_cast<uint32_t>(cpu_frequency.hertz() * 1.0E-7f - 1.0f);

  tmr->event = 1;
  // Enable UIFREMAP.  This causes the top bit of tmr->counter to be true if a
  // timer interrupt is pending.
  tmr->control_reg1.bitfield.uif_remapping = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
  tmr->interrupts_enable = 1;

  Interrupts::singleton().EnableInterrupt(interrupt_vector, InterruptPriority::Standard);
}

void SystemTimer::interrupt_handler() {
  Timer::get_register(id_)->status = 0;
  ms_count_++;
}

void SystemTimer::delay(Duration d) {
  Time start = now();
  while (now() - start < d) {
  }
}

Time SystemTimer::now() const {
  // Disable interrupts so we can read ms_count_ and the timer state without
  // racing with the timer's interrupt handler.
  BlockInterrupts block_interrupts;

  // Bottom 16 bits of the counter are a value in the range [0, 1ms) in units
  // of 100ns.  Top bit of the counter is UIFCOPY, which indicates whether the
  // counter has rolled over and an interrupt to increment ms_count_ is pending.
  //
  // Since the counter is actively running, we need to read both the counter
  // value and UIFCOPY atomically.
  uint32_t counter = Timer::get_register(id_)->counter;
  int64_t micros = (counter & 0xffff) / 10;
  bool interrupt_pending = counter >> 31;

  return microsSinceStartup(ms_count_ * 1000 + micros + (interrupt_pending ? 1 : 0));
}

#else

void SystemTimer::initialize(PeripheralID id, InterruptVector interrupt_vector,
                             Frequency cpu_frequency) {}
void SystemTimer::interrupt_handler() {}
Time SystemTimer::now() const { return time_; }
void SystemTimer::delay(Duration d) { time_ = time_ + d; }

#endif
