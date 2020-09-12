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

// There should be a packet from GUI for every 30ms. If a packet is not
// recived periodically, then assuming something went wrong we wait for
// 100ms before raising a communication failure alarm
static constexpr Duration kCommunicationTimeoutMs{milliseconds(100)};

// This count checks how many times alarm is triggered before it raises alarm
// Alarm will be raised immediately considering as high priority alarm
static constexpr uint8_t kTriggerCountThreshold{1};

// Initialize the time_stamp vartiable for CommFail_Alarm object
void CommFailAlarm::Initialize(Time start_time) {
  time_stamp_ = start_time;
  debug.Print("Communication Failure Alarm Initialized\n");
}

// This function checks for communication timeout, if yes raises alarm, if
// no suppresses alarm. in both the cases it notes the occurance time stamp
bool CommFailAlarm::Handler(Time current_time, Time lastrx_time) {
  bool status{false};
  if (current_time > lastrx_time) {
    // does the communication timed out
    if (current_time - lastrx_time > kCommunicationTimeoutMs) {
      status = CommunicationFailed(current_time);
    } else {
      status = CommunicationResumed(current_time);
    }
  }
  return status;
}

// communication failed so raise alarm
bool CommFailAlarm::CommunicationFailed(Time occurance_time) {
  if (trigger_count_ < kTriggerCountThreshold) {
    trigger_count_++;
    if (trigger_count_ == kTriggerCountThreshold) {
      // take the time stamp when alarm occured
      time_stamp_ = occurance_time;
      triggered_ = true;
      debug.Print("Communication Failed, Raised Alarm\n");
    }
  }
  return triggered_;
}

// communication resumed so clear alarm
bool CommFailAlarm::CommunicationResumed(Time occurance_time) {
  if (trigger_count_ > 0) {
    trigger_count_--;
    if (trigger_count_ == 0) {
      // take the time stamp when alarm cleared
      time_stamp_ = occurance_time;
      triggered_ = false;
      debug.Print("Communication Normal/Resumed, Hence No Alarm\n");
    }
  }
  return triggered_;
}
