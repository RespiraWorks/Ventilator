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

#include "watchdog.h"

void watchdog_init() {
  // FIXME Does this pose potential issues for arduino code updates?
  wdt_enable(WDTO_8S);
}

void watchdog_handler() {
  static uint32_t time;
  static bool first_call = true;

  if (first_call == true) {
    first_call = false;
    time = millis();
  } else {
    // TODO does this rollover properly?
    if ((millis() - time) > WDT_1SECOND) {
      wdt_reset();
      time = millis();
    }
  }
}

void watchdog_reboot() {
  wdt_enable(WDTO_15MS);
  while (1) {
  }
}
