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
  for (int i = 0; i < breaths_to_test * steps_per_breath; i++) {
    Time now = start + i * step_duration;
    SCOPED_TRACE("i = " + std::to_string(i) +
                 ", time = " + std::to_string((now - start).seconds()));

    // Where are we in this breath, [0, 1]?
    float breath_pos =
        static_cast<float>(i % steps_per_breath) / steps_per_breath;
    Pressure inflow_pressure = cmH2O(0.5f + (breath_pos < 0.5 ? 1.f : 0.f));
    Pressure outflow_pressure = cmH2O(0.4f + (breath_pos >= 0.5 ? 1.f : 0.f));
    SensorReadings readings = {
        .patient_pressure = kPa(0),
        .inflow_pressure_diff = inflow_pressure,
        .outflow_pressure_diff = outflow_pressure,
        .inflow = Sensors::PressureDeltaToFlow(inflow_pressure),
        .outflow = Sensors::PressureDeltaToFlow(outflow_pressure),
    };
    VolumetricFlow uncorrected_flow = readings.inflow - readings.outflow;
    flow_integrator.AddFlow(now, uncorrected_flow);

    auto [unused_actuator_status, status] = c.Run(now, params, readings);
    (void)unused_actuator_status;

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
