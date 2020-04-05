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
#include <Arduino.h>

#include "ventilator.h"
#include "comms.h"
#include "alarm.h"
#include "watchdog.h"

void ventilator_start() {

    alarm_init();

    comms_sendResetState(); // Inform the Interface Controller that we just started/restarted

	for (;;) {
		ventilator_control();
	}
}

static void ventilator_control() {

	// Check any new commands?
    comms_handler();

    // pid_execute();
    //comms_sendResetState();
    watchdog_handler();
}
