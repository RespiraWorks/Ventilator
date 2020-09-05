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
#include "vars.h"
#include "debug.h"
#include "hal.h"
#include "alarm.h"
#include "comms.h"
#include "commfail_alarm.h"

Alarm alarm;

Alarm::Alarm()
{
}

void Alarm::AlarmInit(void)
{
  uint8_t idx;

  // for COMMUNICATION_CHECK_ALARM 1
  idx = static_cast<uint8_t>(ActiveAlarms::COMM_CHECK_ALARM);
  alarms[idx].timestamp = Hal.now();
  alarms[idx].callthis = commfail_alarm_cb;
  alarms[idx].priority = AlarmProirity::ALARM_HIGH_PRIORITY;
  alarms[idx].triggered = 0;
  alarms[idx].alarm_pause = 0;          // 0 not pausable, 1 pausable
  alarms[idx].audio_pause = 0;          // 0 not pausable, 1 pausable
  alarms[idx].trigger_count = 0;
  // for UNDEFINED_ALARM 2
  idx = static_cast<uint8_t>(ActiveAlarms::UNDEFINED_ALARM);
  alarms[idx].timestamp = Hal.now();
  alarms[idx].callthis = NULL;
  alarms[idx].priority = AlarmProirity::NONE;
  alarms[idx].triggered = 0;
  alarms[idx].alarm_pause = 0;
  alarms[idx].audio_pause = 0;
  alarms[idx].trigger_count = 0;
  alarm_check_counter = ActiveAlarms::COMM_CHECK_ALARM;
  debug.Print("alarm_init ");
}

void Alarm::AlarmHandler(void)
{
  uint8_t idx;

  switch (alarm_check_counter) {
  case ActiveAlarms::COMM_CHECK_ALARM:
    idx = static_cast<uint8_t>(ActiveAlarms::COMM_CHECK_ALARM);
    // does the communication timed out
    if (comms_is_time_to_raise_commfail_alarm()) {
      if (alarms[idx].trigger_count < 100) {
        alarms[idx].trigger_count++;
        if (alarms[idx].trigger_count == 100) {
          debug.Print("yes_alarm ");
        }
      }
      alarms[idx].callthis();
    }
    else {
      // communication resumed so clear alarm
      if (alarms[idx].trigger_count > 0) {
        alarms[idx].trigger_count--;
        if (alarms[idx].trigger_count == 0) {
          alarms[idx].timestamp = Hal.now();
          alarms[idx].triggered = 0;
          Hal.BuzzerOff();
          debug.Print("no_alarm ");
        }
      }
    }
    alarm_check_counter = ActiveAlarms::UNDEFINED_ALARM;
    break;
  case ActiveAlarms::UNDEFINED_ALARM:
    alarm_check_counter = ActiveAlarms::COMM_CHECK_ALARM;
    break;
  default:
    break;
  }
}

