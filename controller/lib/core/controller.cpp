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

static constexpr Duration LOOP_PERIOD = milliseconds(10);

static DebugFloat dbg_setpoint("setpoint", "Setpoint pressure, kPa");

static DebugFloat dbg_kp("kp", "Proportional gain for main loop", 0.7f);
static DebugFloat dbg_ki("ki", "Integral gain for main loop", 1.0f);
static DebugFloat dbg_kd("kd", "Derivative gain for main loop");
static DebugFloat dbg_sp("pc_setpoint", "Pressure control setpoint (cmH2O)");
static DebugUInt32 dbg_per("loop_period", "Loop period (usec)",
                           static_cast<uint32_t>(LOOP_PERIOD.microseconds()));

Controller::Controller()
    : pid_(dbg_kp.Get(), dbg_ki.Get(), dbg_kd.Get(), ProportionalTerm::ON_ERROR,
           DifferentialTerm::ON_MEASUREMENT, /*output_min=*/0.f,
           /*output_max=*/1.0f) {}

/*static*/ Duration Controller::GetLoopPeriod() { return LOOP_PERIOD; }

std::pair<ActuatorsState, ControllerState>
Controller::Run(Time now, const VentParams &params,
                const SensorReadings &sensor_readings) {
  BlowerSystemState desired_state =
      fsm_.DesiredState(now, params, sensor_readings);

  if (!desired_state.blower_enabled) {
    pid_.Reset();
  }

  pid_.SetKP(dbg_kp.Get());
  pid_.SetKI(dbg_ki.Get());
  pid_.SetKD(dbg_kd.Get());

  float pressure = sensor_readings.patient_pressure.kPa();
  float setpoint = desired_state.setpoint_pressure.kPa();
  dbg_sp.Set(desired_state.setpoint_pressure.cmH2O());

  ControllerState controller_state = {
      .is_new_breath = desired_state.is_new_breath,
      .setpoint_pressure = desired_state.setpoint_pressure,
  };

  ActuatorsState actuators_state = [&]() -> ActuatorsState {
    if (!desired_state.blower_enabled) {
      // System disabled.  Disable blower, close inspiratory pinch valve, and
      // open expiratory pinch valve.  This way if someone is hooked up, they
      // can breathe through the expiratory branch, and they can't contaminate
      // the inspiratory branch.
      //
      // If the pinch valves are not yet homed, this will home them and then
      // move them to the desired positions.
      return {
          .fio2_valve = 0,
          .blower_power = 0,
          .blower_valve = 0,
          .exhale_valve = 1,
      };
    }

    // Start controlling pressure.
    return {
        .fio2_valve = 0, // not used yet
        // In normal mode, blower is always full power; pid controls pressure by
        // actuating the blower pinch valve.
        .blower_power = 1,
        .blower_valve = pid_.Compute(now, pressure, setpoint),
        .exhale_valve =
            desired_state.expire_valve_state == ValveState::OPEN ? 1 : 0,
    };
  }();

  return {actuators_state, controller_state};
}
