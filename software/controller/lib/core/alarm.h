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

enum class AlarmProirity : uint8_t {
  NONE = 0x00,
  ALARM_LOW_PRIORITY = 0x01,
  ALARM_MEDIUM_PRIORITY = 0x02,
  ALARM_HIGH_PRIORITY = 0x03,
};

enum class ActiveAlarms : uint8_t {
  COMM_CHECK_ALARM = 0,
  UNDEFINED_ALARM = 1,
};

struct AlarmCb{
  Time          timestamp;
  void          (*callthis)(void);
  bool          triggered;
  bool          alarm_pause;
  bool          audio_pause;
  AlarmProirity priority;
  uint8_t       trigger_count;
};

// Represents one of the stepper motors in the system
class Alarm {
public:
  static constexpr uint8_t kMaxControllerAlarms = {2};
  // This module monitors if any alarms are triggered

  Alarm();
  AlarmCb alarms[kMaxControllerAlarms];
  ActiveAlarms alarm_check_counter;
  void AlarmInit(void);
  // alarm_handler monitors/checks is there are any faults and despatches alarms
  // accordingly.
  void AlarmHandler(void);
};

extern Alarm alarm;

#endif // COMMS_H
