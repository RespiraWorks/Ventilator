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

#include "blower_fsm.h"

#include "hal.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include <string>

namespace {

class BlowerFsmTest : public ::testing::Test {
public:
  BlowerFsmTest() = default;
  void SetUp() override { blower_fsm_init(); }
};

TEST_F(BlowerFsmTest, InitiallyOff) {
  VentParams p = VentParams_init_zero;
  BlowerSystemState s = blower_fsm_desired_state(p);
  EXPECT_FLOAT_EQ(s.pressure.cmH2O(), 0);
  EXPECT_EQ(s.expire_valve_state, ValveState::OPEN);
}

TEST_F(BlowerFsmTest, StaysOff) {
  VentParams p = VentParams_init_zero;
  Hal.delay(1000);
  BlowerSystemState s = blower_fsm_desired_state(p);
  EXPECT_FLOAT_EQ(s.pressure.cmH2O(), 0);
  EXPECT_EQ(s.expire_valve_state, ValveState::OPEN);
}

// Checks that a sequence of calls to blower_fsm_desired_state() yield the
// expected results.
void testSequence(
    const std::vector<
        std::tuple<VentParams,
                   /*time_millis*/ int,
                   /*expected_setpoint_pressure*/ Pressure,
                   /*expected_expiratory_valve_state*/ ValveState>> &seq) {
  for (const auto &[params, time_millis, expected_pressure,
                    expected_valve_state] : seq) {
    Hal.delay(millisSinceStartup(time_millis) - Hal.now());
    SCOPED_TRACE("time = " + std::to_string(time_millis));
    EXPECT_EQ(time_millis, Hal.now().millisSinceStartup());

    BlowerSystemState s = blower_fsm_desired_state(params);
    EXPECT_EQ(s.pressure.cmH2O(), expected_pressure.cmH2O());
    EXPECT_EQ(s.expire_valve_state, expected_valve_state);
  }
}

TEST_F(BlowerFsmTest, PressureControl) {
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_PRESSURE_CONTROL;
  // 20 breaths/min = 3s/breath.  I:E = 2 means 2s for inspire, 1s for expire.
  p.breaths_per_min = 20;
  p.inspiratory_expiratory_ratio = 2;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  testSequence({
      {p, 0, cmH2O(20), ValveState::CLOSED},
      {p, 1000, cmH2O(20), ValveState::CLOSED},
      {p, 1999, cmH2O(20), ValveState::CLOSED},
      {p, 2001, cmH2O(10), ValveState::OPEN},
      {p, 2999, cmH2O(10), ValveState::OPEN},
      {p, 3001, cmH2O(20), ValveState::CLOSED},
  });
}

// TODO(#220): Test changing the state of the blower FSM while it's running.
//
//  - Changes to params should take effect only at the next breath, but
//  - Switching from ON to OFF and vice versa should happen immediately.

} // anonymous namespace
