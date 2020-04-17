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

#include "respirationCtrl.h"

enum ventilatorState vState;

void respirationCtrl_init() {
    ACV_init();
    PRVC_init();

    vState = ventilatorState::not_running;
}

void respirationCtrl_start() {

    switch(parameters_getVentilatorMode()) {
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

void respirationCtrl_stop() {

    switch(parameters_getVentilatorMode()) {
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

void respirationCtrl_handler() {

    switch(parameters_getVentilatorMode()) {
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

enum ventilatorState respirationCtrl_getVentilatorState() { return vState; }
