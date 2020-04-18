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

#include "respirationCtrl.h"

static enum ventilatorState vState;

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
