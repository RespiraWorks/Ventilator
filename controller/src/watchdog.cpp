/* Copyright 2020, Edwin Chiu

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
#include <Arduino.h>
#include <avr/wdt.h>
#include <stdint.h>

#include "watchdog.h"

// Reset the device if wdt_reset() is not called within roughly this time
// period.
//
// Options are: 15ms, 30ms, 60ms, 120ms, 250ms, 500ms, 1s, 2s, 4s, 8s.
//
// TODO: This value was not chosen carefully.
#define WATCHDOG_TIMEOUT WDTO_120MS

void watchdog_init() {
  // FIXME Does this pose potential issues for arduino code updates?
  wdt_enable(WATCHDOG_TIMEOUT);
}

void watchdog_handler() { wdt_reset(); }

[[noreturn]] void reset_device() {
  // Reset the device by setting a short watchdog timeout and then entering an
  // infinite loop.
  wdt_enable(WDTO_15MS);
  while (1) {
  }
}
