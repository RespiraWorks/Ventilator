/* Copyright 2020-2022, RespiraWorks
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
#include <variant>

#include "breath_detection.h"
#include "network_protocol.pb.h"
#include "units.h"

// This module encapsulates the ventilation system's finite state machine (FSM).
//
// The controllable parts of the "ventilation system" are
//
//  - the blower fan, which generates air pressure,
//  - the inspiratory pinch valve, which sits between the fan and the patient,
//  - the expiratory pinch valve, which sits between the patient and the
//    outside world.
//
// The purpose of this module is to determine, at any point in time, the ideal
// state of the ventilation system:
//
//  - the pressure that ideally should exist in the system, and
//  - whether the primary flow of air should be outside -> in (patient
//    inhaling) or inside -> out (patient exhaling).
//
// Once this module has determined the ideal state, it's the responsibility of
// the PID inside the Controller module to choose blower power and valve
// settings that achieve the state.
//
// The main() loop queries DesiredState() on each iteration, which delegates to
// a "breath FSM" (e.g. PressureControlFsm), which is responsible for *one
// breath* with a fixed mode and VentParams.  When that breath ends, we create a
// new inner FSM with (potentially) new params.
//
// Decision that params should only change at breath boundaries:
// https://respiraworks.slack.com/archives/CV4MTUJHF/p1588001011133500

enum class FlowDirection {
  Inspiratory,
  Expiratory,
};

// Represents a state that the ventilation FSM wants us to achieve at a given point
// in time.
struct VentilationSystemState {
  // The setpoint pressure we're trying to achieve.
  //
  // pressure_setpoint == nullopt means "disable the system, shut down
  // immediately", whereas pressure_setpoint == 0 instructs the system to try
  // to achieve 0 pressure by e.g. closing valves.
  std::optional<Pressure> pressure_setpoint;
  // volume setpoint used in volume control/assist modes
  std::optional<Volume> volume_setpoint;
  // volumetric flow setpoint used in HFNC, CPAP & BIPAP
  std::optional<VolumetricFlow> flow_setpoint;

  // Should air be primarily flowing into the patient, or out of the patient?
  FlowDirection flow_direction;

  // Max/min pressure that the ventilation will try to achieve over this breath.
  //
  // These may not be equal to the VentParams.pip/peep that you pass in to
  // VentilationFsm::DesiredState, because VentilationFsm's parameters change only at
  // breath boundaries.
  Pressure pip;
  Pressure peep;
  Pressure psup;
  Pressure pstep;

  // inspire volume used in volume control/assist modes
  Volume viv;
  bool is_in_exhale = false;
  // Is this the last VentilationSystemState returned at the end of the breath cycle?
  bool is_end_of_breath = false;
};

// A "breath finite state machine" where the ventilation system is always off.
//
// All breath FSMs should implement the following "duck-typed API".
//
//  - <constructor>(Time now, const VentParams& params):
//    Constructs a new FSM for a single breath starting at the given time and
//    with the given params.  Those params don't change during the life of the
//    FSM.
//
//  - VentilationSystemState DesiredState(): Gets the solenoid open/closed
//    state and the pressure that the fan should be trying to hit for the
//    current state of the fsm.
//
class OffFsm {
 public:
  OffFsm() = default;
  explicit OffFsm(Time now, const VentParams &) {}
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs) {
    return {
        .pressure_setpoint = std::nullopt,
        .volume_setpoint = std::nullopt,
        // TODO: It doesn't make much sense to specify a flow direction when the
        // device is off and therefore there's no flow!  It might be better to
        // have a different VentilationSystemState struct for different categories of
        // modes: One for pressure modes, one for volume modes, one for flow
        // modes (high-flow nasal cannula), and one for the off mode.
        //
        // Same applies to the is_end_of_breath flag: it doesn't really pertain
        // to the off state but hardcode it to false by convention.
        .flow_direction = FlowDirection::Expiratory,
        .pip = cmH2O(0.0f),
        .peep = cmH2O(0.0f),
        .viv = ml(0.0f),
        .is_in_exhale = false,
        .is_end_of_breath = false,
    };
  }
};

// "Breath finite state machine" for pressure control mode.
class PressureControlFsm {
 public:
  explicit PressureControlFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

 private:
  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
};

// "Breath finite state machine" for pressure assist mode.
//
// This mode is used for a patient that is not fully sedated and can initiate
// breaths themselves, but once a breath has started, it is fully controlled by
// the ventilator.  In other words, expiration time is controlled by the
// patient, and inspiration time is controlled by the ventilator.
//
// This mode enforces a minimum respiratory rate in case the patient does not
// initiate a breath quickly enough.
class PressureAssistFsm {
 public:
  explicit PressureAssistFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

 private:
  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_deadline_;
  BreathDetection inspire_detection_{};
  Duration inspire_duration;
};

// "Breath finite state machine" for High Flow Nasal Cannula mode
class HFNCFsm {
public:
  explicit HFNCFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const VolumetricFlow needed_flow_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
};

// "Breath finite state machine" for volume control mode.
class VolumeControlFsm {
public:
  explicit VolumeControlFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Volume inspire_volume_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
};

// "Breath finite state machine" for CPAP mode
class CPAPFsm {
public:
  explicit CPAPFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const VolumetricFlow needed_flow_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
};

// "Breath finite state machine" for volume assist mode.
//
class VolumeAssistFsm {
public:
  explicit VolumeAssistFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Volume inspire_volume_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  BreathDetection inspire_detection_{};
};

// "Breath finite state machine" for pressure support mode.
//
class PressureSupportFsm {
public:
  explicit PressureSupportFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Pressure psupp_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  Duration expire_duration;
  BreathDetection inspire_detection_{};
};

// "Breath finite state machine" for PC_SIMV mode.
//
class SIMVPCFsm {
public:
  explicit SIMVPCFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  const Pressure psupp_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  Duration expire_duration;
  BreathDetection inspire_detection_{};
};

// "Breath finite state machine" for VC_SIMV mode.
//
class SIMVVCFsm {
public:
  explicit SIMVVCFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Volume inspire_volume_;
  const Pressure expire_pressure_;
  const Pressure psupp_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  Duration expire_duration;
  bool pressure_support;
  BreathDetection inspire_detection_{};
};

// "Breath finite state machine" for BIPAP mode.
//
class BIPAPFsm {
public:
  explicit BIPAPFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  const Pressure psupp_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  Duration expire_duration;
  BreathDetection inspire_detection_{};
};

// "Breath finite state machine" for PRVC mode.
//
class PRVCFsm {
public:
  explicit PRVCFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  const Pressure pstep_;
  const Volume inspire_volume_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  Duration expire_duration;
};

// "Breath finite state machine" for SPV mode.
//
class SPVFsm {
public:
  explicit SPVFsm(Time now, const VentParams &params);
  VentilationSystemState DesiredState(Time now, const BreathDetectionInputs &inputs);

private:
  const Pressure psupp_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  Duration inspire_duration;
  Duration expire_duration;
  BreathDetection inspire_detection_{};
};

class VentilationFsm {
 public:
  // Gets the state that the system should (ideally) deliver right now.
  VentilationSystemState DesiredState(Time now, const VentParams &params,
                                      const BreathDetectionInputs &inputs);

 private:
  std::variant<
    OffFsm,
    PressureControlFsm,
    PressureAssistFsm,
    HFNCFsm,
    VolumeControlFsm,
    CPAPFsm,
    VolumeAssistFsm,
    PressureSupportFsm,
    SIMVPCFsm,
    SIMVVCFsm,
    BIPAPFsm,
    PRVCFsm,
    SPVFsm
    > fsm_;
};
