/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "watchdog.h"

void watchdog_init() {
    // FIXME Does this pose potential issues for arduino code updates?
    wdt_enable(WDTO_8S);
}

void watchdog_kick() {

    static uint32_t time;
    static bool first_call = true;

    if(first_call == true) {
        first_call = false;
        time = millis();
    }
    else {
        // TODO does this rollover properly?
        if((millis() - time) > WDT_1SECOND) {
            wdt_reset();
            time = millis();
        }
    }
}
