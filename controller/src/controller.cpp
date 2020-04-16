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

#include "controller.h"

static enum ventilatorMode vMode;

enum class ventilatorState {
    not_running     = 0x00,
    ACV_running     = 0x01,
    PRVC_running    = 0x02,

    count                   /* Sentinel */
};

enum ventilatorState vState;

void controller_init() {
    ACV_init();
    PRVC_init();

    vState = ventilatorState::not_running;
}

void controller_start() {

    switch(vMode) {
        case ventilatorMode::ACV:

            ACV_start();
            vState = ventilatorState::ACV_running;

            break;

        case ventilatorMode::PRVC:

            PRVC_start();
            vState = ventilatorState::PRVC_running;

            break;

        default:

            // TODO Flag error

            break;
    }
}

void controller_stop() {

    switch(vMode) {
        case ventilatorMode::ACV:

            ACV_stop();

            break;

        case ventilatorMode::PRVC:

            PRVC_stop();

            break;

        default:

            // TODO Flag error
            break;
    }

    vState = ventilatorState::not_running;
}

void controller_handler() {

    switch(vMode) {
        case ventilatorMode::ACV:

            ACV_handler();

            break;

        case ventilatorMode::PRVC:

            PRVC_handler();

            break;

        default:

            // TODO Flag error

            break;
    }
}

void controller_setVentilatorMode(enum ventilatorMode mode_value) {

    // Only allow one to change the state when the ventilator hasn't been started
    if(vState == ventilatorState::not_running) {
        vMode = mode_value;
    }
}
