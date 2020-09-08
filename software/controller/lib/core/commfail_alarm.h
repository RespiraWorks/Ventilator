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

enum class AlarmProirity : uint8_t {
  NONE = 0x00,
  ALARM_LOW_PRIORITY = 0x01,
  ALARM_MEDIUM_PRIORITY = 0x02,
  ALARM_HIGH_PRIORITY = 0x03,
};

// Represents one of the stepper motors in the system
class CommFailAlarm {
public:
  // This module monitors if any alarms are triggered

  CommFailAlarm();

  void AlarmInit(void);
  // alarm_handler monitors/checks is there are any faults and despatches alarms
  // accordingly.
  void AlarmHandler(void);
private:
  Time          time_stamp_;
  bool          triggered_;
  bool          alarm_pause_;
  bool          audio_pause_;
  AlarmProirity priority_;
  uint8_t       trigger_count_;
  void CommFailAlarmCb(void);
};

extern CommFailAlarm commfailalarm;

#endif // COMMFAIL_ALARM_H

