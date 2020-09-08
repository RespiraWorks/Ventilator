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
#include "commfail_alarm.h"
#include "comms.h"

// This count checks how many times alarm is triggered before it raises alarm
// for medium & low priority alarms, where as alarm will be raised 
// immediately for high priority alarms
static constexpr uint8_t kTriggerCount = 100;

CommFailAlarm commfailalarm;

CommFailAlarm::CommFailAlarm()
{
}

void CommFailAlarm::AlarmInit(void)
{
  // for COMMUNICATION_CHECK_ALARM
  commfailalarm.time_stamp_ = Hal.now();
  commfailalarm.priority_ = AlarmProirity::ALARM_HIGH_PRIORITY;
  commfailalarm.triggered_ = false;
  commfailalarm.alarm_pause_ = false;    // false not pausable, true pausable
  commfailalarm.audio_pause_ = false;    // false not pausable, true pausable
  commfailalarm.trigger_count_ = 0;
  debug.Print("alarm_init ");
}

void CommFailAlarm::AlarmHandler(void)
{
  // does the communication timed out
  if (CommsIsTimeToRaiseCommFailAlarm()) {
    if (commfailalarm.trigger_count_ < kTriggerCount) {
      commfailalarm.trigger_count_++;
      if (commfailalarm.trigger_count_ == kTriggerCount) {
        if ((commfailalarm.priority_ ==
              AlarmProirity::ALARM_MEDIUM_PRIORITY) ||
            (commfailalarm.priority_ ==
             AlarmProirity::ALARM_LOW_PRIORITY)) {
          commfailalarm.triggered_ = true;
        }
        debug.Print("yes_alarm ");
      }
      commfailalarm.CommFailAlarmCb();
    }
  }
  else {
    // communication resumed so clear alarm
    if (commfailalarm.trigger_count_ > 0) {
      commfailalarm.trigger_count_--;
      if (commfailalarm.trigger_count_ == 0) {
        commfailalarm.time_stamp_ = Hal.now();
        commfailalarm.triggered_ = false;
        Hal.BuzzerOff();
        debug.Print("no_alarm ");
      }
    }
  }
}

void CommFailAlarm::CommFailAlarmCb(void)
{
  if (commfailalarm.triggered_ == false) {
    if (commfailalarm.priority_ == AlarmProirity::ALARM_HIGH_PRIORITY) {
      commfailalarm.triggered_ = true;
    }
    if (commfailalarm.triggered_ == true) {
      // take the time stamp when alarm occured
      commfailalarm.time_stamp_ = Hal.now();
      // if triggered is true
      if (commfailalarm.trigger_count_ > 0) {
        if ((commfailalarm.alarm_pause_ == false) &&
            (commfailalarm.audio_pause_ == false)) {
          Hal.BuzzerOn(1.0f);
        }
        else {
          Hal.BuzzerOff();
        }
      }
      debug.Print("alarm_triggered\n");
    }
  }
  else {
    // take the latest timestamp when alarm occured again
    commfailalarm.time_stamp_ = Hal.now();
  }
}

