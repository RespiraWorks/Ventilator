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

#include "pid.h"
#include "vars.h"
#include <math.h>

static constexpr Duration PID_SAMPLE_PERIOD = milliseconds(10);

static DebugFloat dbg_setpoint("setpoint", "Setpoint pressure, kPa");

static DebugFloat dbg_kp("kp", "Proportional gain for main loop", 3.5);
static DebugFloat dbg_ki("ki", "Integral gain for main loop", 1.0);
static DebugFloat dbg_kd("kd", "Derivative gain for main loop");
static DebugFloat dbg_sp("pc_setpoint", "Pressure control setpoint (cmH2O)");

Controller::Controller()
    : pid_(0.0f, 0.0f, 0.0f, ProportionalTerm::ON_ERROR,
           DifferentialTerm::ON_MEASUREMENT, 0.f, 1.0f, PID_SAMPLE_PERIOD) {}

Duration Controller::GetLoopPeriod() { return PID_SAMPLE_PERIOD; }

ActuatorsState Controller::Run(Time now, const VentParams &params,
                               const SensorReadings &readings) {
  BlowerSystemState desired_state = fsm_.DesiredState(now, params);

  ActuatorsState actuator_state;

  pid_.SetKP(dbg_kp.Get());
  pid_.SetKI(dbg_ki.Get());
  pid_.SetKD(dbg_kd.Get());

  // Not used yet
  actuator_state.fio2_valve = 0.0f;

  float pressure = cmH2O(readings.patient_pressure_cm_h2o).kPa();
  float setpoint = desired_state.setpoint_pressure.kPa();
  dbg_sp.Set(desired_state.setpoint_pressure.cmH2O());

  // TODO(jlebar): Add a boolean in ActuatorState that indicates whether
  // the blower and other actuators should be on. Then remove this switch
  // statement; we shouldn't be switching on the VentMode here.
  switch (params.mode) {
  case VentMode_OFF:
    actuator_state.setpoint_cm_h2o = 0.0f;
    actuator_state.blower_valve = std::nullopt;
    actuator_state.exhale_valve = std::nullopt;
    actuator_state.blower_power = 0.0f;
    break;

  case VentMode_PRESSURE_CONTROL:

    // If the pinch valves aren't ready yet then keep the
    // blower off so we don't force full pressure down the
    // patients throat.
    if (!AreActuatorsReady()) {
      actuator_state.exhale_valve = 1.0f;
      actuator_state.blower_valve = 1.0f;
      actuator_state.blower_power = 0.0f;
      break;
    }

    actuator_state.setpoint_cm_h2o = desired_state.setpoint_pressure.cmH2O();
    if (desired_state.expire_valve_state == ValveState::OPEN)
      actuator_state.exhale_valve = 1.0f;
    else
      actuator_state.exhale_valve = 0.0f;

    actuator_state.blower_power = 1.0f;

    actuator_state.blower_valve = pid_.Compute(now, pressure, setpoint);
    break;
  }

  return actuator_state;
}
