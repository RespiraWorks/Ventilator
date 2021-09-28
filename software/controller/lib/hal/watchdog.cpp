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

#include "watchdog.h"

#include <cstdint>

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

/// \TODO: parametrize timeout based on clock frequency
void Watchdog::initialize() {
  WatchdogReg *wdog = WatchdogBase;

  // Enable the watchdog timer by writing the appropriate value to its key
  // register
  wdog->key = 0xCCCC;

  // Enable register access
  wdog->key = 0x5555;

  // Set the pre-scaler to 0.  That setting will cause the watchdog
  // clock to be updated at approximately 8KHz.
  wdog->prescaler = 0;

  // The reload value gives the number of clock cycles before the
  // watchdog timer times out.  I'll set it to 2000 which gives
  // us about 250ms before a reset.
  wdog->reload = 2000;

  // Since the watchdog timer runs off its own clock which is pretty
  // slow, it takes a little time for the registers to actually get
  // updated.  I wait for the status register to go to zero which
  // means its done.
  while (wdog->status) {
  }

  // Reset the timer.  This also locks the registers again.
  wdog->key = 0xAAAA;
}

// Pet the watchdog so it doesn't bite us.
void Watchdog::pet() { WatchdogBase->key = 0xAAAA; }
