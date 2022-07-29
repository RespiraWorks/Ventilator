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

#pragma once

#include "function.h"
#include "interrupts.h"  // for InterruptVector
#include "peripheral_id.h"
#include "units.h"
#include "vars.h"

class HighPriorityTrigger {
 public:
  //  // this is the only way to access it
  //  static HighPriorityTrigger &singleton() {
  //    // will privately initialize on first call
  //    static HighPriorityTrigger SingletonInstance;
  //    // will always return
  //    return SingletonInstance;
  //  }

  // Start the loop timer
  //  void start(PeripheralID id, InterruptVector interrupt_vector, Frequency cpu_frequency,
  //             const Duration &period, void (*callback)(void *), void *arg);

  void start(PeripheralID id, InterruptVector interrupt_vector, Frequency cpu_frequency,
             const Duration &period, naive_function<void(void)> callback);

  void interrupt_handler();

 private:
  Frequency cpu_frequency_{hertz(0)};
  PeripheralID id_;
  naive_function<void(void)> callback_;
  //  void (*controller_callback_)(void *){nullptr};
  //  void *controller_arg_{nullptr};

  using DbgFloat = Debug::Variable::Float;
  using DbgAccess = Debug::Variable::Access;

  DbgFloat loop_latency_{"loop_latency", DbgAccess::ReadOnly,        0.0f,
                         "\xB5s",        "Latency of loop function", "%.2f"};
  DbgFloat max_latency_{
      "max_latency", DbgAccess::ReadOnly, 0.0f, "\xB5s", "Maximum latency of loop function",
      "%.2f"};
  DbgFloat loop_time_{"loop_time", DbgAccess::ReadOnly,         0.0f,
                      "\xB5s",     "Duration of loop function", "%.2f"};

  //  // singleton assurance, because these are private
  //  HighPriorityTrigger() = default;                   // cannot default initialize
  //  HighPriorityTrigger(HighPriorityTrigger const &);  // cannot copy initialize
  //  void operator=(HighPriorityTrigger const &);       // cannot copy assign

  void record_latency(float loop_start_time);
  void record_loop_time(float loop_time);
};
