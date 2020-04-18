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

#ifndef RESPIRATIONCTRL_H
#define RESPIRATIONCTRL_H

#include "parameters.h"
#include "packet_types.h"
#include "ACV.h"
#include "PRVC.h"

// Used to keep track of which ventilator mode is currently running
enum class ventilatorState {
    not_running = 0x00,
    ACV_running = 0x01,
    PRVC_running = 0x02,

    count /* Sentinel */
};


void respirationCtrl_init();
void respirationCtrl_handler();
void respirationCtrl_start();
void respirationCtrl_stop();
enum ventilatorState respirationCtrl_getVentilatorState();

#endif // RESPIRATIONCTRL_H
