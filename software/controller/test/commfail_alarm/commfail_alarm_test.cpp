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

TEST(CommFailAlarmTest, RaiseClearAlarm) {
  constexpr Time ts = microsSinceStartup(0);
  constexpr Time lastrx = microsSinceStartup(0);
  constexpr int itr_to_test = 15;
  constexpr int steps = 100;
  constexpr Duration test_duration = seconds(1);
  constexpr Duration step_duration =
      microseconds(test_duration.microseconds() / steps);

  CommFailAlarm cfa;

  cfa.Initialize(ts);
  for (int i = 0; i < itr_to_test; i++) {
    Time now = ts + i * step_duration;
    bool status = cfa.Handler(now, lastrx);
    if (status) {
      EXPECT_EQ(status, true);
    } else {
      EXPECT_EQ(status, false);
    }
  }
}
