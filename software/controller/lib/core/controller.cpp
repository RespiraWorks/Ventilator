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

#include <algorithm>
#include <cmath>

#include "system_constants.h"

Duration Controller::GetLoopPeriod() { return MainLoopPeriod; }

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

  VentilationSystemState desired_state =
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
  dbg_fio2_setpoint_.set(params.fio2);
  dbg_air_flow_setpoint_.set(sensor_readings.air_inflow.liters_per_sec());

  ActuatorsState actuators_state;

  switch (params.mode) {
    case VentMode_OFF:
      break;
    case VentMode_PRESSURE_CONTROL:
    case VentMode_PRESSURE_ASSIST:
    case VentMode_PRESSURE_REG_VC:
    case VentMode_PRESSURE_SUPPORT:
    case VentMode_SPONTANEOUS_BREATHS:
    case VentMode_PC_SIMV:
    case VentMode_BIPAP:
      if (desired_state.pressure_setpoint == std::nullopt) {
        // System disabled.  Disable blower, close inspiratory pinch valve, and
        // open expiratory pinch valve.  This way if someone is hooked up, they can
        // breathe through the expiratory branch, and they can't contaminate the
        // inspiratory branch.
        //
        // If the pinch valves are not yet homed, this will home them and then move
        // them to the desired positions.
        air_pressure_pid_.reset();
        psol_pid_.reset();
        fio2_pid_.reset();
        air_flow_pid_.reset();

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

        // Delivering air + oxygen mixes from 21 to 59%.
        if (params.fio2 < 0.6) {
          psol_pid_.reset();

          // Successive Loop Closure: pressure on outer loop, flow on inner loop
          // Pressure command >> Flow command >> Actuator command
          float flow_cmd = air_pressure_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                                     desired_state.pressure_setpoint->kPa());

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(), flow_cmd);

          // Fio2 controller with feed-forward
          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          actuators_state = {
              .fio2_valve = std::clamp(
                  sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value, 0.0f, 1.0f),
              // In normal mode, blower is always full power; pid controls pressure
              // by actuating the blower pinch valve.
              .blower_power = 1.f,
              .blower_valve =
                  std::clamp(blower_valve + 0.05f, 0.0f,
                             1.0f),  // always keep the valve a little open for better response
              // coupled control: exhale valve tracks flow command
              .exhale_valve = std::clamp(1.0f - 0.55f * flow_cmd - 0.4f, 0.f, 1.f),
          };
        } else {
          // Delivering air + oxygen mixes from 60 to 100%
          air_pressure_pid_.reset();

          // Single loop on PSOL for now, because PSOL flow sensor not implemented
          // yet
          float psol_valve = psol_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                               desired_state.pressure_setpoint->kPa());

          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                    psol_valve * (1 - fio2_coupling_value));

          actuators_state = {
              // Force psol to stay very slightly open to avoid the discontinuity
              // caused by valve hysteresis at very low command.  The exhale valve
              // compensates for this intentional leakage by staying open when the
              // psol valve is closed.
              .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
              .blower_power = 1.0f,
              .blower_valve = std::clamp(blower_valve, 0.0f, 1.0f),
              .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
          };
        }
        // Start controlling pressure.
        ventilator_was_on_ = true;
      }
      break;
    case VentMode_HIGH_FLOW_NASAL_CANNULA:
      if (desired_state.flow_setpoint == std::nullopt) {
        // System disabled.  Disable blower, close inspiratory pinch valve, and
        // open expiratory pinch valve.  This way if someone is hooked up, they can
        // breathe through the expiratory branch, and they can't contaminate the
        // inspiratory branch.
        //
        // If the pinch valves are not yet homed, this will home them and then move
        // them to the desired positions.
        air_pressure_pid_.reset();
        psol_pid_.reset();
        fio2_pid_.reset();
        air_flow_pid_.reset();

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

        // Delivering air + oxygen mixes from 21 to 59%.
        if (params.fio2 < 0.6) {
          psol_pid_.reset();

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                    desired_state.flow_setpoint->liters_per_sec());

          // Fio2 controller with feed-forward
          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          actuators_state = {
              .fio2_valve = std::clamp(
                  sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value, 0.0f, 1.0f),
              // In normal mode, blower is always full power; pid controls pressure
              // by actuating the blower pinch valve.
              .blower_power = 1.f,
              .blower_valve =
                  std::clamp(blower_valve + 0.05f, 0.0f,
                             1.0f),  // always keep the valve a little open for better response
              // coupled control: exhale valve tracks flow command
              .exhale_valve = std::clamp(blower_valve + 0.05f, 0.f, 1.f),
          };
        } else {
          // Delivering air + oxygen mixes from 60 to 100%
          air_flow_pid_.reset();

          // Single loop on PSOL for now, because PSOL flow sensor not implemented
          // yet
          float psol_valve = psol_pid_.compute(now, net_flow.liters_per_sec(),
                                               desired_state.flow_setpoint->liters_per_sec());

          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                    psol_valve * (1 - fio2_coupling_value));

          actuators_state = {
              // Force psol to stay very slightly open to avoid the discontinuity
              // caused by valve hysteresis at very low command.  The exhale valve
              // compensates for this intentional leakage by staying open when the
              // psol valve is closed.
              .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
              .blower_power = 1.0f,
              .blower_valve = std::clamp(blower_valve, 0.0f, 1.0f),
              .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
          };
        }
        // Start controlling pressure.
        ventilator_was_on_ = true;
      }
      break;
    case VentMode_VOLUME_CONTROL:
    case VentMode_VOLUME_ASSIST:
      if (desired_state.volume_setpoint == std::nullopt) {
        // System disabled.  Disable blower, close inspiratory pinch valve, and
        // open expiratory pinch valve.  This way if someone is hooked up, they can
        // breathe through the expiratory branch, and they can't contaminate the
        // inspiratory branch.
        //
        // If the pinch valves are not yet homed, this will home them and then move
        // them to the desired positions.
        air_volume_pid_.reset();
        air_pressure_pid_.reset();
        psol_pid_.reset();
        fio2_pid_.reset();
        air_flow_pid_.reset();

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

        // Delivering air + oxygen mixes from 21 to 59%.
        if (params.fio2 < 0.6) {
          float flow_cmd;

          psol_pid_.reset();

          if (!desired_state.is_in_exhale) {
            air_pressure_pid_.reset();
            // Successive Loop Closure: volume on outer loop, flow on inner loop
            // Volume command >> Flow command >> Actuator command
            flow_cmd = air_volume_pid_.compute(now, patient_volume.ml(),
                                               desired_state.volume_setpoint->ml());
          } else {
            air_volume_pid_.reset();
            // Successive Loop Closure: pressure on outer loop, flow on inner loop
            // Pressure command >> Flow command >> Actuator command
            flow_cmd = air_pressure_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                                 desired_state.pressure_setpoint->kPa());
          }

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(), flow_cmd);

          // Fio2 controller with feed-forward
          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          if (!desired_state.is_in_exhale) {
            actuators_state = {
                .fio2_valve =
                    std::clamp(sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value,
                               0.0f, 1.0f),
                // In normal mode, blower is always full power; pid controls pressure
                // by actuating the blower pinch valve.
                .blower_power = 1.f,
                .blower_valve =
                    std::clamp(blower_valve, 0.0f,
                               1.0f),  // always keep the valve a little open for better response
                // coupled control: exhale valve tracks flow command
                .exhale_valve = std::clamp(1.0f - 0.60f * blower_valve, 0.f, 1.f),
            };
          } else {
            actuators_state = {
                .fio2_valve =
                    std::clamp(sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value,
                               0.0f, 1.0f),
                // In normal mode, blower is always full power; pid controls pressure
                // by actuating the blower pinch valve.
                .blower_power = 1.f,
                .blower_valve =
                    std::clamp(blower_valve + 0.05f, 0.0f,
                               1.0f),  // always keep the valve a little open for better response
                // coupled control: exhale valve tracks flow command
                .exhale_valve = std::clamp(1.0f - 0.55f * flow_cmd - 0.4f, 0.f, 1.f),
            };
          }
        } else {
          float psol_valve;

          // Delivering air + oxygen mixes from 60 to 100%
          air_pressure_pid_.reset();
          air_volume_pid_.reset();

          // Single loop on PSOL for now, because PSOL flow sensor not implemented
          // yet
          if (!desired_state.is_in_exhale) {
            psol_valve =
                psol_pid_.compute(now, patient_volume.ml(), desired_state.volume_setpoint->ml());
          } else {
            psol_valve = psol_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                           desired_state.pressure_setpoint->kPa());
          }

          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                    psol_valve * (1 - fio2_coupling_value));

          actuators_state = {
              // Force psol to stay very slightly open to avoid the discontinuity
              // caused by valve hysteresis at very low command.  The exhale valve
              // compensates for this intentional leakage by staying open when the
              // psol valve is closed.
              .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
              .blower_power = 1.0f,
              .blower_valve = std::clamp(blower_valve, 0.0f, 1.0f),
              .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
          };
        }
        // Start controlling pressure.
        ventilator_was_on_ = true;
      }
      break;
    case VentMode_CPAP:
      if (desired_state.flow_setpoint == std::nullopt) {
        // System disabled.  Disable blower, close inspiratory pinch valve, and
        // open expiratory pinch valve.  This way if someone is hooked up, they can
        // breathe through the expiratory branch, and they can't contaminate the
        // inspiratory branch.
        //
        // If the pinch valves are not yet homed, this will home them and then move
        // them to the desired positions.
        air_pressure_pid_.reset();
        psol_pid_.reset();
        fio2_pid_.reset();
        air_flow_pid_.reset();

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

        // Delivering air + oxygen mixes from 21 to 59%.
        if (params.fio2 < 0.6) {
          float blower_valve;
          float flow_cmd;

          psol_pid_.reset();

          if (!desired_state.is_in_exhale) {
            air_pressure_pid_.reset();
            // Successive Loop Closure: volume on outer loop, flow on inner loop
            // Volume command >> Flow command >> Actuator command
            blower_valve = air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                                 desired_state.flow_setpoint->liters_per_sec());
          } else {
            // Successive Loop Closure: pressure on outer loop, flow on inner loop
            // Pressure command >> Flow command >> Actuator command
            flow_cmd = air_pressure_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                                 desired_state.pressure_setpoint->kPa());
            blower_valve =
                air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(), flow_cmd);
          }
          // Fio2 controller with feed-forward
          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          if (!desired_state.is_in_exhale) {
            actuators_state = {
                .fio2_valve =
                    std::clamp(sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value,
                               0.0f, 1.0f),
                // In normal mode, blower is always full power; pid controls pressure
                // by actuating the blower pinch valve.
                .blower_power = 1.f,
                .blower_valve =
                    std::clamp(blower_valve + 0.05f, 0.0f,
                               1.0f),  // always keep the valve a little open for better response
                // coupled control: exhale valve tracks flow command
                .exhale_valve = std::clamp(blower_valve + 0.05f, 0.f, 1.f),
            };
          } else {
            actuators_state = {
                .fio2_valve =
                    std::clamp(sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value,
                               0.0f, 1.0f),
                // In normal mode, blower is always full power; pid controls pressure
                // by actuating the blower pinch valve.
                .blower_power = 1.f,
                .blower_valve =
                    std::clamp(blower_valve + 0.05f, 0.0f,
                               1.0f),  // always keep the valve a little open for better response
                // coupled control: exhale valve tracks flow command
                .exhale_valve = std::clamp(1.0f - 0.55f * flow_cmd - 0.4f, 0.f, 1.f),
            };
          }
        } else {
          float psol_valve;

          // Delivering air + oxygen mixes from 60 to 100%
          air_flow_pid_.reset();
          air_pressure_pid_.reset();

          // Single loop on PSOL for now, because PSOL flow sensor not implemented
          // yet
          if (!desired_state.is_in_exhale) {
            psol_valve = psol_pid_.compute(now, sensor_readings.oxygen_inflow.liters_per_sec(),
                                           desired_state.flow_setpoint->liters_per_sec());
          } else {
            psol_valve = psol_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                           desired_state.pressure_setpoint->kPa());
          }

          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                    psol_valve * (1 - fio2_coupling_value));

          actuators_state = {
              // Force psol to stay very slightly open to avoid the discontinuity
              // caused by valve hysteresis at very low command.  The exhale valve
              // compensates for this intentional leakage by staying open when the
              // psol valve is closed.
              .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
              .blower_power = 1.0f,
              .blower_valve = std::clamp(blower_valve, 0.0f, 1.0f),
              .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
          };
        }
        // Start controlling pressure.
        ventilator_was_on_ = true;
      }
      break;
    case VentMode_VC_SIMV:
      if (desired_state.volume_setpoint == std::nullopt) {
        // System disabled.  Disable blower, close inspiratory pinch valve, and
        // open expiratory pinch valve.  This way if someone is hooked up, they can
        // breathe through the expiratory branch, and they can't contaminate the
        // inspiratory branch.
        //
        // If the pinch valves are not yet homed, this will home them and then move
        // them to the desired positions.
        air_volume_pid_.reset();
        air_pressure_pid_.reset();
        psol_pid_.reset();
        fio2_pid_.reset();
        air_flow_pid_.reset();

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

        // Delivering air + oxygen mixes from 21 to 59%.
        if (params.fio2 < 0.6) {
          float flow_cmd;

          psol_pid_.reset();

          if (!desired_state.is_in_exhale) {
            air_pressure_pid_.reset();
            // Successive Loop Closure: volume on outer loop, flow on inner loop
            // Volume command >> Flow command >> Actuator command
            flow_cmd = air_volume_pid_.compute(now, patient_volume.ml(),
                                               desired_state.volume_setpoint->ml());
          } else {
            air_volume_pid_.reset();
            // Successive Loop Closure: pressure on outer loop, flow on inner loop
            // Pressure command >> Flow command >> Actuator command
            flow_cmd = air_pressure_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                                 desired_state.pressure_setpoint->kPa());
          }

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(), flow_cmd);

          // Fio2 controller with feed-forward
          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          if (!desired_state.is_in_exhale) {
            actuators_state = {
                .fio2_valve =
                    std::clamp(sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value,
                               0.0f, 1.0f),
                // In normal mode, blower is always full power; pid controls pressure
                // by actuating the blower pinch valve.
                .blower_power = 1.f,
                .blower_valve =
                    std::clamp(blower_valve, 0.0f,
                               1.0f),  // always keep the valve a little open for better response
                // coupled control: exhale valve tracks flow command
                .exhale_valve = std::clamp(1.0f - 0.60f * blower_valve, 0.f, 1.f),
            };
          } else {
            actuators_state = {
                .fio2_valve =
                    std::clamp(sensor_readings.oxygen_inflow.liters_per_sec() * fio2_coupling_value,
                               0.0f, 1.0f),
                // In normal mode, blower is always full power; pid controls pressure
                // by actuating the blower pinch valve.
                .blower_power = 1.f,
                .blower_valve =
                    std::clamp(blower_valve + 0.05f, 0.0f,
                               1.0f),  // always keep the valve a little open for better response
                // coupled control: exhale valve tracks flow command
                .exhale_valve = std::clamp(1.0f - 0.55f * flow_cmd - 0.4f, 0.f, 1.f),
            };
          }
        } else {
          float psol_valve;

          // Delivering air + oxygen mixes from 60 to 100%
          air_pressure_pid_.reset();
          air_volume_pid_.reset();

          // Single loop on PSOL for now, because PSOL flow sensor not implemented
          // yet
          if (!desired_state.is_in_exhale) {
            psol_valve =
                psol_pid_.compute(now, patient_volume.ml(), desired_state.volume_setpoint->ml());
          } else {
            psol_valve = psol_pid_.compute(now, sensor_readings.patient_pressure.kPa(),
                                           desired_state.pressure_setpoint->kPa());
          }

          float fio2_coupling_value =
              std::clamp(params.fio2 + fio2_pid_.compute(now, sensor_readings.fio2, params.fio2),
                         0.0f, 1.0f);  // just a little bit of feed-forward

          float blower_valve =
              air_flow_pid_.compute(now, sensor_readings.air_inflow.liters_per_sec(),
                                    psol_valve * (1 - fio2_coupling_value));

          actuators_state = {
              // Force psol to stay very slightly open to avoid the discontinuity
              // caused by valve hysteresis at very low command.  The exhale valve
              // compensates for this intentional leakage by staying open when the
              // psol valve is closed.
              .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
              .blower_power = 1.0f,
              .blower_valve = std::clamp(blower_valve, 0.0f, 1.0f),
              .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
          };
        }
        // Start controlling pressure.
        ventilator_was_on_ = true;
      }
      break;
  }

  ControllerState controller_state = {
      .pressure_setpoint = desired_state.pressure_setpoint.value_or(kPa(0)),
      .patient_volume = patient_volume,
      .net_flow = net_flow,
      .flow_correction = flow_integrator_->FlowCorrection(),
      .breath_id = breath_id_,
  };

  dbg_pc_setpoint_.set(desired_state.pressure_setpoint.value_or(kPa(0)).cmH2O());
  dbg_volume_setpoint_.set(desired_state.volume_setpoint.value_or(cubic_m(0)).ml());
  dbg_flow_setpoint_.set(desired_state.flow_setpoint.value_or(cubic_m_per_sec(0)).liters_per_sec());
  dbg_net_flow_.set(controller_state.net_flow.ml_per_sec());
  dbg_net_flow_uncorrected_.set(uncorrected_net_flow.ml_per_sec());
  dbg_volume_.set(controller_state.patient_volume.ml());
  dbg_volume_uncorrected_.set(uncorrected_flow_integrator_->GetVolume().ml());
  dbg_flow_correction_.set(flow_integrator_->FlowCorrection().ml_per_sec());

  return {actuators_state, controller_state};
}
