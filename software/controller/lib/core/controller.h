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

#pragma once

#include <optional>
#include <utility>

#include "actuators.h"
#include "blower_fsm.h"
#include "flow_integrator.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "sensors.h"
#include "units.h"
#include "vars.h"

// TODO: This name is too close to the ControllerStatus proto.
struct ControllerState {
  // Patient pressure the controller wants to achieve.
  Pressure pressure_setpoint;

  // Patient volume and net flow (inflow minus outflow) are properties of
  // ControllerState, not SensorReadings, because flow and volume measurement
  // require error corrections that can only happen at the controller level.
  //
  // The "correct" volume after a normal breath is 0, and it's the controller
  // that knows when breaths occur.  We use this info to drive flow (and
  // therefore volume) to 0 at each breath boundary.
  Volume patient_volume;
  VolumetricFlow net_flow;

  // Offset that was added to raw measured flow when computing net_flow.  If
  // this is a large negative value, it may indicate a leak in the system.
  VolumetricFlow flow_correction;

  // Identifies the current breath among all breaths handled since controller
  // startup.
  uint64_t breath_id{0};
};

// This class is here to allow integration of our controller into Modelica
// software and run closed-loop tests in a simulated physical environment
class Controller {
 public:
  Controller() = default;

  static Duration GetLoopPeriod();

  std::pair<ActuatorsState, ControllerState> Run(Time now, const VentParams &params,
                                                 const SensorReadings &sensor_readings);

 private:
  uint32_t breath_id_{0};
  BlowerFsm fsm_;

  // TODO: These params need to be tuned.
  PID blower_valve_pid_{"blower_valve_",
                        " for blower valve PID",
                        /*kp=*/0.04f,
                        /*ki=*/10.0f,
                        /*kd=*/0.0f,
                        /*p_term=*/PID::TermApplication::OnError,
                        /*d_term=*/PID::TermApplication::OnMeasurement,
                        /*output_min=*/0.f,
                        /*output_max=*/1.0f};

  PID psol_pid_{"psol_",
                " for O2 psol PID",
                /*kp=*/0.04f,
                /*ki=*/20.0f,
                /*kd=*/0.0f,
                /*p_term=*/PID::TermApplication::OnError,
                /*d_term=*/PID::TermApplication::OnMeasurement,
                /*output_min=*/0.f,
                /*output_max=*/1.0f};

  PID fio2_pid_{"fio2_",
                " for FIO2 PID",
      /*kp=*/0.001f,
      /*ki=*/0.1f,
      /*kd=*/0.0f,
      /*p_term=*/PID::TermApplication::OnError,
      /*d_term=*/PID::TermApplication::OnMeasurement,
      /*output_min=*/0.f,
      /*output_max=*/1.0f};

  // These objects accumulate flow to calculate volume.
  //
  // For debugging, we accumulate flow with and without error correction.  See
  // FlowIntegrator definition for description of errors.
  //
  // These are never nullopt; we use std::optional to let us clear/reset these
  // objects.
  std::optional<FlowIntegrator> flow_integrator_ = FlowIntegrator();
  std::optional<FlowIntegrator> uncorrected_flow_integrator_ = FlowIntegrator();

  // This state tells the controller whether the vent was already On when Run()
  // was last called, and allows resetting integrators when transitioning from
  // Off state to On state.
  bool ventilator_was_on_{false};

  // Debug variables
  using DbgFloat = Debug::Variable::Float;
  using DbgUint32 = Debug::Variable::UInt32;
  using DbgAccess = Debug::Variable::Access;

  DbgFloat forced_blower_power_{
      "forced_blower_power", DbgAccess::ReadWrite, -1.f, "ratio",
      "Force the blower fan to a particular power [0,1].  Specify a value "
      "outside this range to let the controller control it."};
  DbgFloat forced_exhale_valve_pos_{
      "forced_exhale_valve_pos", DbgAccess::ReadWrite, -1.f, "ratio",
      "Force the exhale valve to a particular position [0,1].  Specify a value "
      "outside this range to let the controller control it."};
  DbgFloat forced_blower_valve_pos_{
      "forced_blower_valve_pos", DbgAccess::ReadWrite, -1.f, "ratio",
      "Force the blower valve to a particular position [0,1].  Specify a value "
      "outside this range to let the controller control it."};
  DbgFloat forced_psol_pos_{
      "forced_psol_pos", DbgAccess::ReadWrite, -1.f, "ratio",
      "Force the O2 psol to a particular position [0,1].  (Note that psol.cpp "
      "scales this further; see psol_pwm_closed and psol_pwm_open.)  Specify a "
      "value outside this range to let the controller control the psol."};

  // Unchanging outputs - read from external debug program, never modified here.
  DbgUint32 dbg_loop_period_{"loop_period", DbgAccess::ReadOnly,
                             static_cast<uint32_t>(GetLoopPeriod().microseconds()), "\xB5s",
                             "Loop period"};

  // Outputs - read from external debug program, modified by the controller.
  DbgFloat dbg_pc_setpoint_{"pc_setpoint", DbgAccess::ReadOnly, 0.0f, "cmH2O",
                            "Pressure control set-point"};
  // \todo: need a forced_fio2 variable
  DbgFloat dbg_fio2_setpoint_{"fio2_setpoint", DbgAccess::ReadOnly, 0.21f, "ratio",
                              "FiO2 setpoint [0.0, 1.0] as commanded by GUI"};

  DbgFloat dbg_net_flow_{"net_flow", DbgAccess::ReadOnly, 0.0f, "mL/s", "Net flow rate"};
  DbgFloat dbg_net_flow_uncorrected_{"net_flow_uncorrected", DbgAccess::ReadOnly, 0.0f, "mL/s",
                                     "Net flow rate w/o correction"};
  DbgFloat dbg_flow_correction_{"flow_correction", DbgAccess::ReadOnly, 0.0f, "mL/s",
                                "Correction to flow"};
  DbgFloat dbg_volume_{"volume", DbgAccess::ReadOnly, 0.0f, "mL", "Patient volume"};
  DbgFloat dbg_volume_uncorrected_{"uncorrected_volume", DbgAccess::ReadOnly, 0.0f, "mL",
                                   "Patient volume w/o correction"};

  DbgUint32 dbg_breath_id_{"breath_id", DbgAccess::ReadOnly, 0, "", "ID of the current breath"};
};
