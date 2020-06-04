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

// This file implements the ventilator controller used when the ventilator
// is in pressure control mode.

#include "controller.h"
#include "vars.h"

// Debug variabels use to tune the loop
static DebugFloat pc_kp("pc_kp", "Proportional gain for pressure control mode");
static DebugFloat pc_ki("pc_ki", "Integral gain for pressure control mode");
static DebugFloat pc_ff("pc_ff", "Feed forward gain for pressure control mode");
static DebugFloat
    pc_setpoint("pc_setpoint",
                "Pressure set point in pressure control mode (cm H2O)");

// Called when we first enter this state
void VentModePresCtrl::Enter() {
  pc_kp.Set(0.25f);
  pc_ki.Set(0.5f);
  pc_ff.Set(0.0f);
  start_time = Hal.now();
  inhale = true;

  pid.Reset();
  pid.SetKP(pc_kp.Get());
  pid.SetKI(pc_ki.Get());
  pid.SetKFF(pc_ff.Get());

  pid.SetLimits(0.0f, 1.0f);
  pid.SetSamplePeriod(GetLoopPeriod());
}

ActuatorsState VentModePresCtrl::Run(const VentParams &params,
                                     const SensorReadings &readings) {
  ActuatorsState state;

  state.blower_valve_enabled = true;
  state.exhale_valve_enabled = true;

  // Make sure the pinch valves are ready for use.
  // I leave the blower off until they are ready because I
  // don't want to be forcing air down the patients throat
  // before the valves are functioning
  if (!AreActuatorsReady()) {
    start_time = Hal.now();
    return state;
  }

  if (pc_kp.HasBeenSet())
    pid.SetKP(pc_kp.Get());

  if (pc_ki.HasBeenSet())
    pid.SetKI(pc_ki.Get());

  if (pc_ff.HasBeenSet())
    pid.SetKFF(pc_ff.Get());

  // See if it's time to switch between inhale & exhale
  Duration dt = Hal.now() - start_time;
  Duration per = inhale ? inspire_duration(params) : expire_duration(params);
  if (dt > per) {
    inhale = !inhale;
    start_time = Hal.now();
  }

  uint32_t cmh2o = inhale ? params.pip_cm_h2o : params.peep_cm_h2o;
  Pressure set_point(cmH2O(static_cast<float>(cmh2o)));
  pc_setpoint.Set(set_point.cmH2O());

  // For the moment I'm just setting the blower to 100%
  state.fan_power = 1.0f;

  // The exhale valve is just fully open or closed
  state.exhale_valve = inhale ? 0.0f : 1.0f;

  // The fan valve is controlled using a simple PI loop
  float command = set_point.cmH2O();
  float actual = readings.patient_pressure_cm_h2o;
  state.fan_valve = pid.Compute(actual, command);

  return state;
}
