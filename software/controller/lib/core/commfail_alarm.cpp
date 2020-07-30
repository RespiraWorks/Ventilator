/* Copyright 2020, RespiraWorks

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

#include <stdint.h>
#include "debug.h"
#include "hal.h"
#include "alarm.h"
#include "commfail_alarm.h"

extern alarm_cb_t alarms[];

void commfail_alarm_cb(void)
{
    if ((alarms[COMM_CHECK_ALARM].triggered == 0) &&
            (alarms[COMM_CHECK_ALARM].priority == ALARM_HIGH_PRIORITY)) {
        // take the time stamp when alarm occured
        alarms[COMM_CHECK_ALARM].timestamp = Hal.now();
        // if triggered is more than 0;
        if (alarms[COMM_CHECK_ALARM].trigger_count > 0) {
            if ((alarms[COMM_CHECK_ALARM].alarm_pause == 0) &&
                    (alarms[COMM_CHECK_ALARM].audio_pause == 0)) {
                Hal.BuzzerOn(1.0f);
            }
            else {
                Hal.BuzzerOff();
            }
        }
        alarms[COMM_CHECK_ALARM].triggered = 1;
        debug.Print("alarm_triggered");
    }
    else {
        // take the latest timestamp when alarm occured again
        alarms[COMM_CHECK_ALARM].timestamp = Hal.now();
    }
}

