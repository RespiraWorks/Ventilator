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

#include <math.h>

static constexpr Duration LoopPeriod = milliseconds(10);

/*static*/ Duration Controller::GetLoopPeriod() { return LoopPeriod; }

std::pair<ActuatorsState, ControllerState> Controller::Run(Time now, const VentParams &params,
                                                           const SensorReadings &sensor_readings) {
  VolumetricFlow uncorrected_net_flow =
      sensor_readings.air_inflow
      // + sensor_readings.oxygen_inflow \todo add this once it is well tested
      - sensor_readings.outflow;
  flow_integrator_->AddFlow(now, uncorrected_net_flow);
  uncorrected_flow_integrator_->AddFlow(now, uncorrected_net_flow);

  Volume patient_volume = flow_integrator_->GetVolume();
  VolumetricFlow net_flow = uncorrected_net_flow + flow_integrator_->FlowCorrection();

  BlowerSystemState desired_state =
      fsm_.DesiredState(now, params, {.patient_volume = patient_volume, .net_flow = net_flow});

  if (desired_state.is_end_of_breath) {
    // The "correct" volume at the breath boundary is 0.
    flow_integrator_->NoteExpectedVolume(ml(0));

    // Precision loss 64->32 bits ok: we only care about equality of these
    // values, not their absolute value, and the top 32 bits will change with
    // each new breath.
    // \todo micros since startup? Should this not just be breath_id_++ ?
    breath_id_ = static_cast<uint32_t>(now.microsSinceStartup());
  }
  dbg_breath_id_.set(breath_id_);

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
    psol_pid_.Reset();
	fio2_pid_.Reset();

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

    // At the moment we don't support oxygen mixing -- we deliver either pure
    // air or pure oxygen.  For any fio2 < 1, deliver air.
    if (params.fio2 < 1) {
      // Delivering pure air.
      psol_pid_.reset();

      // Calculate blower valve command using calculated gains
      float blower_valve = blower_valve_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                                     desired_state.pressure_setpoint->kPa());
	  float fio2_coupling_value = fio2_pid_.compute(now, sensor_readings.fio2, params.fio2);

      actuators_state = {
          .fio2_valve = blower_valve * fio2_coupling_value,
          // In normal mode, blower is always full power; pid controls pressure
          // by actuating the blower pinch valve.
          .blower_power = 1,
          .blower_valve = std::clamp(blower_valve + 0.05f, 0.0f, 1.0f),
          // coupled control: exhale valve tracks inhale valve command
          .exhale_valve = 1.0f - 0.55f * blower_valve - 0.4f,
      };
    } else {
      // Delivering pure oxygen.
      blower_valve_pid_.reset();

      float psol_valve = psol_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                           desired_state.pressure_setpoint->kPa());
      actuators_state = {
          // Force psol to stay very slightly open to avoid the discontinuity
          // caused by valve hysteresis at very low command.  The exhale valve
          // compensates for this intentional leakage by staying open when the
          // psol valve is closed.
          .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
          .blower_power = 0,
          .blower_valve = 0,
          .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
      };
    }

    // Start controlling pressure.
    ventilator_was_on_ = true;
  }

  ControllerState controller_state = {
      .pressure_setpoint = desired_state.pressure_setpoint.value_or(kPa(0)),
      .patient_volume = patient_volume,
      .net_flow = net_flow,
      .flow_correction = flow_integrator_->FlowCorrection(),
      .breath_id = breath_id_,
  };

  dbg_pc_setpoint_.set(desired_state.pressure_setpoint.value_or(kPa(0)).cmH2O());
  dbg_net_flow_.set(controller_state.net_flow.ml_per_sec());
  dbg_net_flow_uncorrected_.set(uncorrected_net_flow.ml_per_sec());
  dbg_volume_.set(controller_state.patient_volume.ml());
  dbg_volume_uncorrected_.set(uncorrected_flow_integrator_->GetVolume().ml());
  dbg_flow_correction_.set(flow_integrator_->FlowCorrection().ml_per_sec());

  // Handle DebugVars that force the actuators.
  auto set_force = [](const Debug::Variable::Float &var, auto &state) {
    float v = var.get();
    if (v >= 0 && v <= 1) {
      state = v;
    }
  };
  set_force(forced_blower_power_, actuators_state.blower_power);
  set_force(forced_blower_valve_pos_, actuators_state.blower_valve);
  set_force(forced_exhale_valve_pos_, actuators_state.exhale_valve);
  set_force(forced_psol_pos_, actuators_state.fio2_valve);

  return {actuators_state, controller_state};
}
