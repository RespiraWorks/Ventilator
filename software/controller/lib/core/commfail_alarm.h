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

#include "debug.h"
#include <stdint.h>

// This module monitor & raise/suppress alarm:
// When communication between controller board & GUI fails, alarm will be
// raised with timestamp of occurance
// When communication between controller board & GUI resumes, alarm will be
// suppressed with timestamp of occurance
class CommFailAlarm {
public:
  void Initialize(Time ts);
  bool Handler(Time ts, Time lastrx);

private:
  Time time_stamp_;
  bool triggered_{false};
  uint8_t trigger_count_{0};
  void CommunicationFailed(Time ts);
  void CommunicationResumed(Time ts);
};

#endif // COMMFAIL_ALARM_H
