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
#include "gtest/gtest.h"
#include <cmath>
#include <tuple>

// To test Handler function
TEST(CommFailAlarmTest, AlarmRaiseHandler) {
  Time ts = microsSinceStartup(0);               // start
  constexpr Time lastrx = microsSinceStartup(0); // start
  constexpr Duration step = milliseconds(10);
  CommFailAlarm cfa;

  cfa.Initialize(ts);
  for (int i = 0; i < 20; i++) {
    ts += step;
    if (ts - lastrx < milliseconds(101)) {
      EXPECT_FALSE(cfa.Handler(ts, lastrx)); // false for <= 100ms
    } else {
      EXPECT_TRUE(cfa.Handler(ts, lastrx)); // true for > 100ms
    }
  }
}

// To test CommunicationFailed function
TEST(CommFailAlarmTest, RaiseAlarm) {
  constexpr Time ts = microsSinceStartup(0); // start
  CommFailAlarm cfa;
  bool status = false;

  cfa.Initialize(ts);
  for (int i = 0; i < 5; i++) { // when i = 0, trigger_count_ will become 1
    status = cfa.CommunicationFailed(ts);
  }
  EXPECT_TRUE(status);
}

// To test CommunicationResumed function
TEST(CommFailAlarmTest, ClearAlarm) {
  constexpr Time ts = microsSinceStartup(0); // start
  CommFailAlarm cfa;
  bool status = true;

  cfa.Initialize(ts);
  for (int i = 0; i < 5; i++) { // in fail case when i = 0,
                                // trigger_count_ will become 1
    cfa.CommunicationFailed(ts);
  }
  for (int i = 0; i < 5; i++) { // in resume case when i = 0,
                                // trigger_count_ wlll become 0
    status = cfa.CommunicationResumed(ts);
  }
  EXPECT_FALSE(status);
}
