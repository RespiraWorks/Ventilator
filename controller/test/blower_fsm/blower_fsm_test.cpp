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

TEST(BlowerFsmTest, InitiallyOff) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  BlowerSystemState s = fsm.DesiredState(Hal.now(), p);
  EXPECT_FLOAT_EQ(s.setpoint_pressure.cmH2O(), 0);
  EXPECT_EQ(s.expire_valve_state, ValveState::OPEN);
}

TEST(BlowerFsmTest, StaysOff) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  Hal.delay(milliseconds(1000));
  BlowerSystemState s = fsm.DesiredState(Hal.now(), p);
  EXPECT_FLOAT_EQ(s.setpoint_pressure.cmH2O(), 0);
  EXPECT_EQ(s.expire_valve_state, ValveState::OPEN);
}

// Checks that a sequence of calls to blower_fsm_desired_state() yield the
// expected results.
void testSequence(
    const std::vector<
        std::tuple<VentParams,
                   /*blower_enabled*/ bool,
                   /*time_millis*/ uint64_t,
                   /*expected_setpoint_pressure*/ Pressure,
                   /*expected_expiratory_valve_state*/ ValveState>> &seq) {
  BlowerFsm fsm;
  for (const auto &[params, blower_enabled, time_millis, expected_pressure,
                    expected_valve_state] : seq) {
    Hal.delay(millisSinceStartup(time_millis) - Hal.now());
    SCOPED_TRACE("time = " + std::to_string(time_millis));
    EXPECT_EQ(time_millis, Hal.now().millisSinceStartup());

    BlowerSystemState s = fsm.DesiredState(Hal.now(), params);
    EXPECT_EQ(s.blower_enabled, blower_enabled);
    EXPECT_EQ(s.setpoint_pressure.cmH2O(), expected_pressure.cmH2O());
    EXPECT_EQ(s.expire_valve_state, expected_valve_state);
  }
}

TEST(BlowerFsmTest, PressureControl) {
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_PRESSURE_CONTROL;
  // 20 breaths/min = 3s/breath.  I:E = 2 means 2s for inspire, 1s for expire.
  p.breaths_per_min = 20;
  p.inspiratory_expiratory_ratio = 2;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  testSequence({
      {p, /*blower_enabled=*/true, 0, cmH2O(20), ValveState::CLOSED},
      {p, /*blower_enabled=*/true, 1000, cmH2O(20), ValveState::CLOSED},
      {p, /*blower_enabled=*/true, 1999, cmH2O(20), ValveState::CLOSED},
      {p, /*blower_enabled=*/true, 2001, cmH2O(10), ValveState::OPEN},
      {p, /*blower_enabled=*/true, 2999, cmH2O(10), ValveState::OPEN},
      {p, /*blower_enabled=*/true, 3001, cmH2O(20), ValveState::CLOSED},
  });
}

TEST(BlowerFsmTest, TurnOff) {
  VentParams p_on = VentParams_init_zero;
  p_on.mode = VentMode_PRESSURE_CONTROL;
  // 20 breaths/min = 3s/breath.  I:E = 2 means 2s for inspire, 1s for expire.
  p_on.breaths_per_min = 20;
  p_on.inspiratory_expiratory_ratio = 2;
  p_on.peep_cm_h2o = 10;
  p_on.pip_cm_h2o = 20;

  VentParams p_off = VentParams_init_zero;

  testSequence({
      {p_off, /*blower_enabled=*/false, 0, cmH2O(0), ValveState::OPEN},
      {p_on, /*blower_enabled=*/true, 1000, cmH2O(20), ValveState::CLOSED},
      {p_off, /*blower_enabled=*/false, 1001, cmH2O(0), ValveState::OPEN},
  });
}

TEST(BlowerFsmTest, ChangeOfParamsStartAtTheNextBreath) {
  VentParams p_init = VentParams_init_zero;
  p_init.mode = VentMode_PRESSURE_CONTROL;
  p_init.breaths_per_min = 20;
  p_init.inspiratory_expiratory_ratio = 2; // I: 2s, E: 1s
  p_init.pip_cm_h2o = 20;
  p_init.peep_cm_h2o = 10;

  VentParams p_change = p_init;
  p_change.breaths_per_min = 30;
  p_change.inspiratory_expiratory_ratio = 1; // I: 1s, E: 1s
  p_change.pip_cm_h2o = 30;
  p_change.peep_cm_h2o = 15;

  VentParams p_off = VentParams_init_default;
  //|---------------------|----------|----------|----------|
  // 0                   1999        2999       3999       4999
  //         I                 E           I          E
  testSequence({
      // Switching ON mode takes effect immidiately.
      {p_init, /*blower_enabled=*/true, 0, cmH2O(20), ValveState::CLOSED},
      // 2sec of inhalation 1sec of exhalation. Ignores param change, stays on
      // p_init pip.
      {p_change, /*blower_enabled=*/true, 1999, cmH2O(20), ValveState::CLOSED},
      {p_change, /*blower_enabled=*/true, 2000, cmH2O(10), ValveState::OPEN},
      {p_change, /*blower_enabled=*/true, 3000, cmH2O(10), ValveState::OPEN},
      // Previous state finished, switch to p_change settings, 1sec In 1sec Ex.
      {p_change, /*blower_enabled=*/true, 3001, cmH2O(30), ValveState::CLOSED},
      {p_init, /*blower_enabled=*/true, 4000, cmH2O(30), ValveState::CLOSED},
      // Ignore p_init setting in the middle of a breath.
      {p_init, /*blower_enabled=*/true, 4001, cmH2O(15), ValveState::OPEN},
      {p_init, /*blower_enabled=*/true, 5000, cmH2O(15), ValveState::OPEN},
      // Switching OFF device, takes effect immidiately.
      {p_off, /*blower_enabled*/ false, 5005, cmH2O(0), ValveState::OPEN},
      // Switching ON device, takes effect immidiately.
      {p_init, /*blower_enabled*/ true, 5010, cmH2O(20), ValveState::CLOSED},
  });
}

} // anonymous namespace
