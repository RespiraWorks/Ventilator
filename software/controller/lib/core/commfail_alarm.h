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

#ifndef COMMFAIL_ALARM_H
#define COMMFAIL_ALARM_H

#include <stdint.h>

// This module monitors if any alarms are triggered
class CommFailAlarm {
public:
  void Initialize();
  // alarm_handler monitors/checks is there are any faults and despatches alarms
  // accordingly.
  void Handler();

private:
  Time time_stamp_;
  bool triggered_{false};
  bool alarm_pause_{false};
  bool audio_pause_{false};
  uint8_t trigger_count_{0};
  void CommFailAlarmCb();
};

#endif // COMMFAIL_ALARM_H
