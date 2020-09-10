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

#include "controller.h"
#include "gtest/gtest.h"
#include <cmath>
#include <tuple>

// TODO: There ought to be many more tests in here.

TEST(ControllerTest, ControllerVolumeMatchesFlowIntegrator) {
  constexpr Time start = microsSinceStartup(0);
  constexpr int breaths_to_test = 5;
  constexpr int steps_per_breath = 6;
  constexpr Duration breath_duration = seconds(6);
  constexpr Duration step_duration =
      microseconds(breath_duration.microseconds() / steps_per_breath);
  static_assert(breath_duration.microseconds() % steps_per_breath == 0);

  Controller c;
  VentParams params = VentParams_init_zero;
  params.mode = VentMode::VentMode_PRESSURE_CONTROL;
  params.peep_cm_h2o = 5;
  params.breaths_per_min =
      static_cast<uint32_t>(std::round(1.f / breath_duration.minutes()));
  params.pip_cm_h2o = 15;
  params.inspiratory_expiratory_ratio = 1;

  FlowIntegrator flow_integrator;

  SensorReadings readings = {
      .patient_pressure = kPa(0),
      .inflow_pressure_diff = kPa(0),
      .outflow_pressure_diff = kPa(0),
      .fio2 = 0.21f,
      .inflow = ml_per_min(0),
      .outflow = ml_per_min(0),
  };
  // need to Run once in order to initialize the volume integrators
  auto [unused_actuator_state, unused_status] = c.Run(start, params, readings);
  (void)unused_actuator_state;
  (void)unused_status;

  for (int i = 0; i < breaths_to_test * steps_per_breath; i++) {
    Time now = start + i * step_duration;
    SCOPED_TRACE("i = " + std::to_string(i) +
                 ", time = " + std::to_string((now - start).seconds()));

    // Where are we in this breath, [0, 1]?
    float breath_pos =
        static_cast<float>(i % steps_per_breath) / steps_per_breath;
    Pressure inflow_pressure = cmH2O(0.5f + (breath_pos < 0.5 ? 1.f : 0.f));
    Pressure outflow_pressure = cmH2O(0.4f + (breath_pos >= 0.5 ? 1.f : 0.f));
    readings = {
        .patient_pressure = kPa(0),
        .inflow_pressure_diff = inflow_pressure,
        .outflow_pressure_diff = outflow_pressure,
        .fio2 = 0.21f,
        .inflow = Sensors::PressureDeltaToFlow(inflow_pressure),
        .outflow = Sensors::PressureDeltaToFlow(outflow_pressure),
    };
    VolumetricFlow uncorrected_flow = readings.inflow - readings.outflow;
    flow_integrator.AddFlow(now, uncorrected_flow);

    auto [unused_actuator_state, status] = c.Run(now, params, readings);
    (void)unused_actuator_state;

    EXPECT_FLOAT_EQ(
        status.net_flow.ml_per_sec(),
        (uncorrected_flow + flow_integrator.FlowCorrection()).ml_per_sec());
    EXPECT_FLOAT_EQ(status.patient_volume.ml(),
                    flow_integrator.GetVolume().ml());

    if (breath_pos == 0) {
      flow_integrator.NoteExpectedVolume(ml(0));
    }
  }
}

TEST(ControllerTest, BreathId) {
  constexpr Time start = microsSinceStartup(0);
  constexpr int breaths_to_test = 5;
  constexpr int steps_per_breath = 6;
  constexpr Duration breath_duration = seconds(6);
  constexpr Duration step_duration =
      microseconds(breath_duration.microseconds() / steps_per_breath);
  static_assert(breath_duration.microseconds() % steps_per_breath == 0);

  Controller c;
  VentParams params = VentParams_init_zero;
  params.mode = VentMode::VentMode_PRESSURE_CONTROL;
  params.peep_cm_h2o = 5;
  params.breaths_per_min =
      static_cast<uint32_t>(std::round(1.f / breath_duration.minutes()));
  params.pip_cm_h2o = 15;
  params.inspiratory_expiratory_ratio = 1;

  // No need to initialize "readings" because breath_id does not depend on
  // any sensor values in this mode.
  SensorReadings readings;

  for (int i = 0; i < breaths_to_test * steps_per_breath; i++) {
    Time now = start + i * step_duration;
    auto [unused_actuator_state, status] = c.Run(now, params, readings);
    (void)unused_actuator_state;
    Time breath_start = (start + (i - i % steps_per_breath) * step_duration);
    EXPECT_EQ(breath_start.microsSinceStartup(), status.breath_id);
  }
}

struct ActuatorsTest {
  Time time{microsSinceStartup(0)};
  VentParams params;
  SensorReadings readings;
  ActuatorsState expected_state;
};

// Checks that a sequence of calls to controller_run() yields the expected
// actuators state
void actuatorsTestSequence(const std::vector<ActuatorsTest> &seq) {
  if (seq.empty()) {
    // I see two ways to handle the call with an empty vector:
    //  - fail test because it really shouldn't be empty, but this may become an
    //  issue if we read input data from a file
    //  - pass test assuming the caller knew what he was doing. This is what I
    //  choose for now.
    return;
  }
  constexpr float kValveStateTolerance{.001f};

  // Reset time to test's start time.
  Hal.delay(seq.front().time - Hal.now());

  Controller controller;
  VentParams last_params = VentParams_init_zero;
  SensorReadings last_readings = {
      .patient_pressure = cmH2O(0),
      .inflow_pressure_diff = kPa(0),
      .outflow_pressure_diff = kPa(0),
      .fio2 = 0,
      .inflow = ml_per_min(0),
      .outflow = ml_per_min(0),
  };

  for (const auto &actuators_test : seq) {

    SCOPED_TRACE("time = " + actuators_test.time.microsSinceStartup() / 1000);
    // Move time forward to t in steps of Controller::GetLoopPeriod().
    while (Hal.now() < actuators_test.time) {
      Hal.delay(Controller::GetLoopPeriod());
      (void)controller.Run(Hal.now(), last_params, last_readings);
    }
    EXPECT_EQ(actuators_test.time.microsSinceStartup(),
              Hal.now().microsSinceStartup());

    auto [act_state, unused_status] = controller.Run(
        Hal.now(), actuators_test.params, actuators_test.readings);
    (void)unused_status;

    EXPECT_FLOAT_EQ(act_state.blower_power,
                    actuators_test.expected_state.blower_power);
    EXPECT_NEAR(act_state.fio2_valve, actuators_test.expected_state.fio2_valve,
                kValveStateTolerance);
    EXPECT_NEAR(act_state.blower_valve.value(),
                actuators_test.expected_state.blower_valve.value(),
                kValveStateTolerance);
    EXPECT_NEAR(act_state.exhale_valve.value(),
                actuators_test.expected_state.exhale_valve.value(),
                kValveStateTolerance);
    last_params = actuators_test.params;
    last_readings = actuators_test.readings;
  }
}

TEST(ControllerTest, ControlLaws) {

  // typical expected values
  constexpr float kBlowerOn{1.0f};
  constexpr float kBlowerOff{0};
  constexpr float kValveOpen{1.0f};
  constexpr float kAlmostClosed{0.05f};
  constexpr float kValveClosed{0};
  constexpr float kExhaleMaxOpen{0.6f};
  // typical 02 param/reading
  constexpr float kAmbientAir{0.21f};
  // Very low pressure to make readings below desired PIP trigger a maximum
  // inflow with outlet valve closed or almost closed.
  constexpr Pressure kVeryLowPressure{cmH2O(-500)};

  VentParams params = VentParams_init_zero;
  params.mode = VentMode::VentMode_PRESSURE_CONTROL;
  // Setting PIP=PEEP in order not to care about respiratory cycle
  params.peep_cm_h2o = 50;
  params.pip_cm_h2o = params.peep_cm_h2o;
  params.breaths_per_min = 15;
  params.inspiratory_expiratory_ratio = 1;
  params.fio2 = kAmbientAir;

  // Readings with patient pressure == Setpoint, should lead to inlet valve
  // almost closed and exhale valve in its max openness state.
  SensorReadings readings_pip = {
      .patient_pressure = cmH2O(static_cast<float>(params.pip_cm_h2o)),
      .inflow_pressure_diff = kPa(0),
      .outflow_pressure_diff = kPa(0),
      .fio2 = kAmbientAir,
      .inflow = ml_per_min(0),
      .outflow = ml_per_min(0),
  };

  // Readings with very low patient pressure (much smaller than Setpoint), which
  // should lead to inlet valve fully open and exhale valve either almost closed
  // (when using air) or fully closed (when using oxygen, in order not to waste
  // oxygen), whatever the controller gains and desired params are.
  SensorReadings readings_below_pip = {
      .patient_pressure = kVeryLowPressure,
      .inflow_pressure_diff = kPa(0),
      .outflow_pressure_diff = kPa(0),
      .fio2 = kAmbientAir,
      .inflow = ml_per_min(0),
      .outflow = ml_per_min(0),
  };

  // pressure control pure air test
  actuatorsTestSequence({{.time = microsSinceStartup(0),
                          .params = params,
                          .readings = readings_pip,
                          .expected_state = {.fio2_valve = kValveClosed,
                                             .blower_power = kBlowerOn,
                                             .blower_valve = kAlmostClosed,
                                             .exhale_valve = kExhaleMaxOpen}},
                         {.time = microsSinceStartup(1E6),
                          .params = params,
                          .readings = readings_below_pip,
                          .expected_state = {.fio2_valve = kValveClosed,
                                             .blower_power = kBlowerOn,
                                             .blower_valve = kValveOpen,
                                             .exhale_valve = kAlmostClosed}}});

  // pressure control oxygen test (same tests but with oxygen).
  params.fio2 = 1.0f;
  actuatorsTestSequence({{.time = microsSinceStartup(0),
                          .params = params,
                          .readings = readings_pip,
                          .expected_state = {.fio2_valve = kAlmostClosed,
                                             .blower_power = kBlowerOff,
                                             .blower_valve = kValveClosed,
                                             .exhale_valve = kExhaleMaxOpen}},
                         {.time = microsSinceStartup(1E6),
                          .params = params,
                          .readings = readings_below_pip,
                          .expected_state = {.fio2_valve = kValveOpen,
                                             .blower_power = kBlowerOff,
                                             .blower_valve = kValveClosed,
                                             .exhale_valve = kValveClosed}}});

  // Off mode: blower is off, inhale valves are closed, exhale valve is open.
  params.mode = VentMode::VentMode_OFF;
  actuatorsTestSequence({{.time = microsSinceStartup(0),
                          .params = params,
                          .readings = readings_pip,
                          .expected_state = {.fio2_valve = kValveClosed,
                                             .blower_power = kBlowerOff,
                                             .blower_valve = kValveClosed,
                                             .exhale_valve = kValveOpen}}});
}
