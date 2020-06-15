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

// Inputs - set from external debug program, read but never modified here.
static DebugFloat dbg_blower_valve_kp("blower_valve_kp",
                                      "Proportional gain for blower valve PID",
                                      0.7f);
static DebugFloat dbg_blower_valve_ki("blower_valve_ki",
                                      "Integral gain for blower valve PID",
                                      1.0f);
static DebugFloat dbg_blower_valve_kd("blower_valve_kd",
                                      "Derivative gain for blower valve PID");

// Unchanging outputs - read from external debug program, never modified here.
static DebugUInt32 dbg_per("loop_period", "Loop period, read-only (usec)",
                           static_cast<uint32_t>(LOOP_PERIOD.microseconds()));

// Outputs - read from external debug program, modified here.
static DebugFloat dbg_sp("pc_setpoint", "Pressure control setpoint (cmH2O)");
static DebugFloat dbg_net_flow("net_flow", "Net flow rate, cc/sec");
static DebugFloat dbg_volume("volume", "Patient volume, ml");
static DebugFloat
    dbg_net_flow_uncorrected("net_flow_uncorrected",
                             "Net flow rate w/o correction, cc/sec");
static DebugFloat dbg_volume_uncorrected("uncorrected_volume",
                                         "Patient volume w/o correction, ml");
static DebugFloat dbg_flow_correction("flow_correction",
                                      "Correction to flow, cc/sec");

Controller::Controller()
    : blower_valve_pid_(dbg_blower_valve_kp.Get(), dbg_blower_valve_ki.Get(),
                        dbg_blower_valve_kd.Get(), ProportionalTerm::ON_ERROR,
                        DifferentialTerm::ON_MEASUREMENT, /*output_min=*/0.f,
                        /*output_max=*/1.0f) {}

/*static*/ Duration Controller::GetLoopPeriod() { return LOOP_PERIOD; }

std::pair<ActuatorsState, ControllerState>
Controller::Run(Time now, const VentParams &params,
                const SensorReadings &sensor_readings) {
  blower_valve_pid_.SetKP(dbg_blower_valve_kp.Get());
  blower_valve_pid_.SetKI(dbg_blower_valve_ki.Get());
  blower_valve_pid_.SetKD(dbg_blower_valve_kd.Get());

  VolumetricFlow uncorrected_net_flow =
      sensor_readings.inflow - sensor_readings.outflow;
  flow_integrator_->AddFlow(now, uncorrected_net_flow);
  uncorrected_flow_integrator_->AddFlow(now, uncorrected_net_flow);

  Volume patient_volume = flow_integrator_->GetVolume();
  VolumetricFlow net_flow =
      uncorrected_net_flow + flow_integrator_->FlowCorrection();

  BlowerSystemState desired_state = fsm_.DesiredState(
      now, params, {.patient_volume = patient_volume, .net_flow = net_flow});

  if (desired_state.is_new_breath) {
    // The "correct" volume at the breath boundary is 0.
    flow_integrator_->NoteExpectedVolume(ml(0));
    breath_id_ = now.microsSinceStartup();
  }

  ActuatorsState actuators_state;
  if (desired_state.pressure_setpoint == std::nullopt) {
    // System disabled.  Disable blower, close inspiratory pinch valve, and
    // open expiratory pinch valve.  This way if someone is hooked up, they can
    // breathe through the expiratory branch, and they can't contaminate the
    // inspiratory branch.
    //
    // If the pinch valves are not yet homed, this will home them and then move
    // them to the desired positions.
    blower_valve_pid_.Reset();

    actuators_state = {
        .fio2_valve = 0,
        .blower_power = 0,
        .blower_valve = 0,
        .exhale_valve = 1,
    };
    ventilator_was_on_ = false;
  } else {
    if (!ventilator_was_on_) {
      // reset volume integrators
      flow_integrator_.emplace();
      uncorrected_flow_integrator_.emplace();
    }
    // Start controlling pressure.
    actuators_state = {
        .fio2_valve = 0, // not used yet
        // In normal mode, blower is always full power; pid controls pressure by
        // actuating the blower pinch valve.
        .blower_power = 1,
        .blower_valve = blower_valve_pid_.Compute(
            now, sensor_readings.patient_pressure.kPa(),
            desired_state.pressure_setpoint->kPa()),
        .exhale_valve =
            desired_state.flow_direction == FlowDirection::EXPIRATORY ? 1 : 0,
    };
    ventilator_was_on_ = true;
  }

  ControllerState controller_state = {
      .pressure_setpoint = desired_state.pressure_setpoint.value_or(kPa(0)),
      .patient_volume = patient_volume,
      .net_flow = net_flow,
      .breath_id = breath_id_,
  };

  dbg_sp.Set(desired_state.pressure_setpoint.value_or(kPa(0)).cmH2O());
  dbg_net_flow.Set(controller_state.net_flow.ml_per_sec());
  dbg_net_flow_uncorrected.Set(
      (sensor_readings.inflow - sensor_readings.outflow).ml_per_sec());
  dbg_volume.Set(controller_state.patient_volume.ml());
  dbg_volume_uncorrected.Set(uncorrected_flow_integrator_->GetVolume().ml());
  dbg_flow_correction.Set(flow_integrator_->FlowCorrection().ml_per_sec());

  return {actuators_state, controller_state};
}
