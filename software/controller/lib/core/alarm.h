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

#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>

enum alarm_proirity {
    NONE,
    ALARM_LOW_PRIORITY,
    ALARM_MEDIUM_PRIORITY,
    ALARM_HIGH_PRIORITY,
};

enum active_alarms {
    COMM_CHECK_ALARM = 0,
    UNDEFINED_ALARM = 1,
};

typedef struct _alarm_cb_t {
    Time        timestamp;
    void        (*callthis)(void);
    uint8_t     priority;
    uint8_t     triggered;
    uint8_t     alarm_pause;
    uint8_t     audio_pause;
    uint8_t     trigger_count;
    uint8_t     dummy[3];               // can be used for future
} alarm_cb_t;

// This module monitors if any alarms are triggered

void alarm_init(void);

// alarm_handler monitors/checks is there are any faults and despatches alarms
// accordingly.
void alarm_handler(void);

#endif // COMMS_H
