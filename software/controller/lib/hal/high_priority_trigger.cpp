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

#include "high_priority_trigger.h"

#if defined(BARE_STM32)

#include "clocks_stm32.h"
#include "timers_stm32.h"
#include "watchdog.h"

void HighPriorityTrigger::start(PeripheralID id, InterruptVector interrupt_vector,
                                Frequency cpu_frequency, const Duration &period,
                                void (*callback)(void *), void *arg) {
  id_ = id;
  cpu_frequency_ = cpu_frequency;
  controller_callback_ = callback;
  controller_arg_ = arg;

  // Init the watchdog timer now.  The watchdog timer is serviced by the loop callback function.
  // I don't init it until the loop starts because otherwise it may expire before the function that
  // resets it starts running
  Watchdog::initialize();

  // Find the loop period in clock cycles
  int32_t reload = static_cast<int32_t>(cpu_frequency_.hertz() * period.seconds());
  int prescale = 1;

  // Adjust the prescaler so that my reload count will fit in the 16-bit timer.
  if (reload > 65536) {
    prescale = static_cast<int>(reload / 65536.0) + 1;
    reload /= prescale;
  }

  // Enable the clock to the timer
  Clocks::enable_peripheral_clock(id_);

  // Just set the timer up to count every microsecond.
  auto *tmr = Timer::get_register(id_);
  tmr->auto_reload = reload - 1;
  tmr->prescaler = prescale - 1;
  tmr->event = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
  tmr->interrupts_enable = 1;

  // Enable the interrupt that will call the controller function callback periodically.
  // We are using a lower priority then that which I use for normal hardware interrupts.
  // This means that other interrupts can be serviced while controller functions are running.
  Interrupts::singleton().EnableInterrupt(interrupt_vector, InterruptPriority::Low);
}

void HighPriorityTrigger::interrupt_handler() {
  auto *timer_register = Timer::get_register(id_);
  uint32_t start = timer_register->counter;
  timer_register->status = 0;

  // Keep track of loop latency in uSec
  // Also max latency since it was last zeroed
  record_latency(static_cast<float>(start));

  // Call the function
  controller_callback_(controller_arg_);

  uint32_t end = timer_register->counter;
  record_loop_time(static_cast<float>(end - start));
}

#else

void HighPriorityTrigger::start(PeripheralID id, InterruptVector interrupt_vector,
                                Frequency cpu_frequency, const Duration &period,
                                void (*callback)(void *), void *arg) {}
void HighPriorityTrigger::interrupt_handler() {}

#endif

void HighPriorityTrigger::record_latency(float loop_start_time) {
  auto latency = loop_start_time * (1.0f / cpu_frequency_.megahertz());
  loop_latency_.set(latency);
  if (latency > max_latency_.get()) {
    max_latency_.set(latency);
  }
}

void HighPriorityTrigger::record_loop_time(float loop_time) {
  loop_time_.set(loop_time * (1.0f / cpu_frequency_.megahertz()));
}
