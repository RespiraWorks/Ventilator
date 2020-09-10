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
#include "units.h"
#include <stdint.h>

// There should be a packet from GUI for every 30ms. If a packet is not
// recived periodically, then assuming something went wrong we wait for
// 100ms before raising a communication failure alarm
static constexpr Duration kCommunicationTimeoutMs{milliseconds(100)};

// This count checks how many times alarm is triggered before it raises alarm
// Alarm will be raised immediately considering as high priority alarm
static constexpr uint8_t kTriggerCount{1};

// Initialize the time_stamp vartiable for CommFail_Alarm object
void CommFailAlarm::Initialize(CommFailAlarm *alarm) {
  alarm->time_stamp_ = Hal.now();
  debug.Print("Communication Failure Alarm Initialized\n");
}

// This function checks for communication timeout, if yes raises alarm, if
// no suppresses alarm. in both the cases it notes the occurance time stamp
void CommFailAlarm::Handler(CommFailAlarm *alarm) {
  Time ts;

  ts = Hal.now();
  // does the communication timed out
  if (ts - CommsGetLastRxTime() > kCommunicationTimeoutMs) {
    if (alarm->trigger_count_ < kTriggerCount) {
      alarm->trigger_count_++;
      if (alarm->trigger_count_ == kTriggerCount) {
        alarm->triggered_ = true;
        // take the time stamp when alarm occured
        alarm->time_stamp_ = ts;
        if ((!alarm->alarm_pause_) && (!alarm->audio_pause_)) {
          Hal.BuzzerOn(1.0f);
        } else {
          Hal.BuzzerOff();
        }
        debug.Print("Communication Failed, Raised Alarm\n");
      }
    }
  } else {
    // communication resumed so clear alarm
    if (alarm->trigger_count_ > 0) {
      alarm->trigger_count_--;
      if (alarm->trigger_count_ == 0) {
        alarm->time_stamp_ = ts;
        alarm->triggered_ = false;
        Hal.BuzzerOff();
        debug.Print("Communication Normal/Resumed, Hence No Alarm\n");
      }
    }
  }
}
