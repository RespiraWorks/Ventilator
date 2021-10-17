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

#include "units.h"

class SystemTimer {
 public:
  // this is the only way to access it
  static SystemTimer &singleton() {
    // will privately initialize on first call
    static SystemTimer SingletonInstance;
    // will always return
    return SingletonInstance;
  }

  void initialize(Frequency cpu_frequency);

  void interrupt_handler();

  // Amount of time that has passed since the board started running the program.
  // Faked when testing.  Time doesn't advance unless you call Delay().
  Time now();

  // Sleeps for some number of milliseconds.
  // Faked when testing.  Does not sleep, but does advance the time returned by millis().
  void delay(Duration d);

 private:
  /// \TODO: do 64-bit types work on STM32?
  volatile uint64_t ms_count_{0};

  // singleton assurance, because these are private
  SystemTimer() = default;              // cannot default initialize
  SystemTimer(SystemTimer const &);     // cannot copy initialize
  void operator=(SystemTimer const &);  // cannot copy assign

#if !defined(BARE_STM32)
  Time time_{microsSinceStartup(0)};
#endif
};
