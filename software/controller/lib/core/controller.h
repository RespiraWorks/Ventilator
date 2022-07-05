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
#include "flow_integrator.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "sensors.h"
#include "units.h"
#include "vars.h"
#include "ventilation_fsm.h"

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
  VentilationFsm fsm_;

  // TODO: These params need to be tuned.

  PID air_volume_pid_{"air_volume_",
                      " for AIR outer loop volume PID",
                      /*initial_kp=*/0.75f,
                      /*initial_ki=*/20.0f,
                      /*initial_kd=*/0.075f,
                      /*p_term=*/PID::TermApplication::OnError,
                      /*d_term=*/PID::TermApplication::OnMeasurement,
                      /*output_min=*/0.f,
                      /*output_max=*/1.0f};

  // Ziegler-Nichols Outer Loop Pressure values: ku = 0.9, Tu = 0.04
  PID air_pressure_pid_{"air_pressure_",
                        " for AIR outer loop pressure PID",
                        /*initial_kp=*/0.4f,
                        /*initial_ki=*/20.0f,
                        /*initial_kd=*/0.0f,
                        /*p_term=*/PID::TermApplication::OnError,
                        /*d_term=*/PID::TermApplication::OnMeasurement,
                        /*output_min=*/0.f,
                        /*output_max=*/1.0f};

  PID psol_pid_{"psol",
                " for O2 psol PID",
                /*initial_kp=*/0.04f,
                /*initial_ki=*/20.0f,
                /*initial_kd=*/0.0f,
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
                /*output_min=*/-1.0f,
                /*output_max=*/1.0f};

  PID air_flow_pid_{"air_flow_",
                    " for air flow PID",
                    /*kp=*/0.1f,
                    /*ki=*/20.0f,
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

  // Unchanging outputs - read from external debug program, never modified here.
  DbgUint32 dbg_loop_period_{"loop_period", DbgAccess::ReadOnly,
                             static_cast<uint32_t>(GetLoopPeriod().microseconds()), "\xB5s",
                             "Loop period"};

  // Outputs - read from external debug program, modified by the controller.
  DbgFloat dbg_pc_setpoint_{"pc_setpoint", DbgAccess::ReadOnly, 0.0f, "cmH2O",
                            "Pressure control set-point"};

  DbgFloat dbg_volume_setpoint_{"vol_setpoint", DbgAccess::ReadOnly, 0.0f, "m³",
                                "Volume control setpoint"};

  DbgFloat dbg_flow_setpoint_{"flow_setpoint", DbgAccess::ReadOnly, 0.0f, "L/s",
                              "Flow control setpoint"};

  // \todo: need a forced_fio2 variable
  DbgFloat dbg_fio2_setpoint_{"fio2_setpoint", DbgAccess::ReadOnly, 0.21f, "ratio",
                              "FiO2 setpoint [0.0, 1.0] as commanded by GUI"};

  DbgFloat dbg_air_flow_setpoint_{"air_flow_setpoint", DbgAccess::ReadOnly, 0.0f, "L/s",
                                  "Setpoint for air flow PID"};
  DbgFloat dbg_air_flow_lps_{"air_flow_measured", DbgAccess::ReadOnly, 0.0f, "L/s",
                             "Measured Air Flow"};

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
