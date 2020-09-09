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

#include "commfail_alarm.h"
#include "comms.h"
#include "debug.h"
#include "hal.h"
#include <stdint.h>

extern CommFailAlarm alarm;

// This count checks how many times alarm is triggered before it raises alarm
// Alarm will be raised immediately considering as high priority alarm
static constexpr uint8_t kTriggerCount = 1;

void CommFailAlarm::Initialize() {
  // for COMMUNICATION_CHECK_ALARM
  alarm.time_stamp_ = Hal.now();
  debug.Print("Communication Failure Alarm Initialized\n");
}

void CommFailAlarm::Handler() {
  // does the communication timed out
  if (CommsIsTimeToRaiseCommFailAlarm()) {
    if (alarm.trigger_count_ < kTriggerCount) {
      alarm.trigger_count_++;
      if (alarm.trigger_count_ == kTriggerCount) {
        alarm.triggered_ = true;
        alarm.CommFailAlarmCb();
        debug.Print("Communication Failed, Raised Alarm\n");
      }
    }
  } else {
    // communication resumed so clear alarm
    if (alarm.trigger_count_ > 0) {
      alarm.trigger_count_--;
      if (alarm.trigger_count_ == 0) {
        alarm.time_stamp_ = Hal.now();
        alarm.triggered_ = false;
        Hal.BuzzerOff();
        debug.Print("Communication Normal/Resumed, Hence No Alarm\n");
      }
    }
  }
}

void CommFailAlarm::CommFailAlarmCb() {
  if (alarm.triggered_) {
    // take the time stamp when alarm occured
    alarm.time_stamp_ = Hal.now();
    if ((!alarm.alarm_pause_) && (!alarm.audio_pause_)) {
      Hal.BuzzerOn(1.0f);
    } else {
      Hal.BuzzerOff();
    }
  }
}
