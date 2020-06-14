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
#include <optional>
#include <string>

namespace {

constexpr SensorReadings readings_zero = {
    .patient_pressure = kPa(0),
    .inflow_pressure_diff = kPa(0),
    .outflow_pressure_diff = kPa(0),
    .inflow = ml_per_min(0),
    .outflow = ml_per_min(0),
    .volume = ml(0),
    .net_flow = ml_per_min(0),
};

TEST(BlowerFsmTest, InitiallyOff) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  BlowerSystemState s = fsm.DesiredState(Hal.now(), p, readings_zero);
  EXPECT_TRUE(s.pressure_setpoint == std::nullopt);
  EXPECT_EQ(s.flow_direction, FlowDirection::EXPIRATORY);
}

TEST(BlowerFsmTest, StaysOff) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  Hal.delay(milliseconds(1000));
  BlowerSystemState s = fsm.DesiredState(Hal.now(), p, readings_zero);
  EXPECT_TRUE(s.pressure_setpoint == std::nullopt);
  EXPECT_EQ(s.flow_direction, FlowDirection::EXPIRATORY);
}

// Checks that a sequence of calls to blower_fsm_desired_state() yield the
// expected results.
void testSequence(
    const std::vector<
        std::tuple<VentParams,
                   /*sensor_readings*/ SensorReadings,
                   /*time_millis*/ uint64_t,
                   /*expected_pressure_setpoint*/ std::optional<Pressure>,
                   /*expected_flow_direction*/ FlowDirection>> &seq) {
  BlowerFsm fsm;
  for (const auto &[params, readings, time_millis, expected_pressure,
                    expected_flow_direction] : seq) {
    Hal.delay(microsSinceStartup(time_millis * 1000) - Hal.now());
    SCOPED_TRACE("time = " + std::to_string(time_millis));
    EXPECT_EQ(time_millis * 1000, Hal.now().microsSinceStartup());

    BlowerSystemState s = fsm.DesiredState(Hal.now(), params, readings);
    EXPECT_EQ(s.pressure_setpoint.has_value(), expected_pressure.has_value());
    EXPECT_EQ(s.pressure_setpoint.value_or(cmH2O(0)).cmH2O(),
              expected_pressure.value_or(cmH2O(0)).cmH2O());
    EXPECT_EQ(s.flow_direction, expected_flow_direction);
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

  constexpr int64_t rise_time_us = PressureControlFsm::RISE_TIME.microseconds();
  static_assert(rise_time_us % 1000 == 0,
                "test assumes rise time is a whole number of ms.");
  constexpr int64_t rise_time_ms = rise_time_us / 1000;
  static_assert(rise_time_ms % 4 == 0,
                "test assumes we can divide rise time ms by 4.");
  testSequence({
      // Pressure starts out at PEEP and rises to PIP over period
      // PressureControlFsm::RISE_TIME.
      {p, readings_zero, 0, cmH2O(10), FlowDirection::INSPIRATORY},
      {p, readings_zero, rise_time_ms / 4, cmH2O(12.5),
       FlowDirection::INSPIRATORY},
      {p, readings_zero, rise_time_ms / 2, cmH2O(15),
       FlowDirection::INSPIRATORY},
      {p, readings_zero, 3 * rise_time_ms / 4, cmH2O(17.5),
       FlowDirection::INSPIRATORY},
      {p, readings_zero, 1000, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 1999, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 2001, cmH2O(10), FlowDirection::EXPIRATORY},
      {p, readings_zero, 2999, cmH2O(10), FlowDirection::EXPIRATORY},
      {p, readings_zero, 3001, cmH2O(10), FlowDirection::INSPIRATORY},
      {p, readings_zero, 3001 + rise_time_ms / 2, cmH2O(15),
       FlowDirection::INSPIRATORY},
      {p, readings_zero, 3001 + rise_time_ms, cmH2O(20),
       FlowDirection::INSPIRATORY},
  });
}

TEST(BlowerFsmTest, PressureAssist) {
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_PRESSURE_ASSIST;
  // 20 breaths/min = 3s/breath.  I:E = 2 means 2s for inspire, 1s for expire.
  p.breaths_per_min = 20;
  p.inspiratory_expiratory_ratio = 2;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  SensorReadings readings_breath = {.patient_pressure = kPa(0),
                                    .inflow_pressure_diff = kPa(0),
                                    .outflow_pressure_diff = kPa(0),
                                    .inflow = ml_per_min(20000.0f),
                                    .outflow = ml_per_min(0),
                                    .volume = ml(0),
                                    .net_flow = ml_per_min(20000.0f)};

  // - when flow is zero: breath is triggered on expire_deadline_ rather than
  // patient triggered, to enforce minimum respiratory rate
  // - when flow is breath: trigger breath if in expire mode
  testSequence({
      // first breath is mandatory
      {p, readings_zero, 0, cmH2O(20), FlowDirection::INSPIRATORY},
      // breath has no effect during inspire phase
      {p, readings_breath, 1000, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 1999, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 2001, cmH2O(10), FlowDirection::EXPIRATORY},
      // need to run with non-breath flow while already in exhale leg to
      // initialize detection threshold
      {p, readings_zero, 2002, cmH2O(10), FlowDirection::EXPIRATORY},
      // check that calling with zero flow before the end of the breath does not
      // tigger the next breath
      {p, readings_zero, 2500, cmH2O(10), FlowDirection::EXPIRATORY},
      {p, readings_zero, 2999, cmH2O(10), FlowDirection::EXPIRATORY},
      // trigger breath on expire_deadline_
      {p, readings_zero, 3001, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 4999, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 5001, cmH2O(10), FlowDirection::EXPIRATORY},
      // need to run with non-breath flow while already in exhale leg to
      // initialize detection threshold
      {p, readings_zero, 5002, cmH2O(10), FlowDirection::EXPIRATORY},
      {p, readings_breath, 5200, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 7199, cmH2O(20), FlowDirection::INSPIRATORY},
      {p, readings_zero, 7201, cmH2O(10), FlowDirection::EXPIRATORY},
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
      {p_off, readings_zero, 0, std::nullopt, FlowDirection::EXPIRATORY},
      // This is PEEP pressure even though it's inspiration, because ramp it up
      // to PIP over a duration of PressureControlFsm::RISE_TIME.
      {p_on, readings_zero, 1000, cmH2O(10), FlowDirection::INSPIRATORY},
      {p_off, readings_zero, 1001, std::nullopt, FlowDirection::EXPIRATORY},
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
      // Switching ON mode takes effect immidiately.  Because of pressure
      // control mode's ramp time, the initial pressure is PEEP, not PIP.
      {p_init, readings_zero, 0, cmH2O(10), FlowDirection::INSPIRATORY},
      // 2sec of inhalation 1sec of exhalation. Ignores param change, stays on
      // p_init pip.
      {p_change, readings_zero, 1999, cmH2O(20), FlowDirection::INSPIRATORY},
      {p_change, readings_zero, 2000, cmH2O(10), FlowDirection::EXPIRATORY},
      {p_change, readings_zero, 3000, cmH2O(10), FlowDirection::EXPIRATORY},
      // Previous state finished, switch to p_change settings, 1sec In 1sec Ex.
      {p_change, readings_zero, 3001, cmH2O(15), FlowDirection::INSPIRATORY},
      {p_init, readings_zero, 4000, cmH2O(30), FlowDirection::INSPIRATORY},
      // Ignore p_init setting in the middle of a breath.
      {p_init, readings_zero, 4001, cmH2O(15), FlowDirection::EXPIRATORY},
      {p_init, readings_zero, 5000, cmH2O(15), FlowDirection::EXPIRATORY},
      // Switching OFF device, takes effect immidiately.
      {p_off, readings_zero, 5005, std::nullopt, FlowDirection::EXPIRATORY},
      // Switching ON device, takes effect immidiately.
      {p_init, readings_zero, 5010, cmH2O(10), FlowDirection::INSPIRATORY},
  });
}

} // anonymous namespace
