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

#ifndef BLOWER_FSM_H
#define BLOWER_FSM_H

#include <optional>
#include <variant>

#include "network_protocol.pb.h"
#include "sensors.h"
#include "units.h"

// This module encapsulates the blower system's finite state machine (FSM).
//
// The controllable parts of the "blower system" are
//
//  - the blower fan, which generates air pressure,
//  - the inspiratory pinch valve, which sits between the fan and the patient,
//  - the expiratory pinch valve, which sits between the patient and the
//    outside world.
//
// The purpose of this module is to determine, at any point in time, the ideal
// state of the blower system:
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
  INSPIRATORY,
  EXPIRATORY,
};

// Sensor readings etc that are used by BlowerFsm to do e.g. breath detection.
struct BlowerFsmInputs {
  // Current patient volume, corrected for sensor zero-point drift.
  Volume patient_volume;
  // inflow - outflow, corrected for sensor zero-point drift.
  VolumetricFlow net_flow;
};

// Represents a state that the blower FSM wants us to achieve at a given point
// in time.
struct BlowerSystemState {
  // The setpoint pressure we're trying to achieve.
  //
  // pressure_setpoint == nullopt means "disable the system, shut down
  // immediately", whereas pressure_setpoint == 0 instructs the system to try
  // to achieve 0 pressure by e.g. closing valves.
  std::optional<Pressure> pressure_setpoint;

  // Should air be primarily flowing into the patient, or out of the patient?
  FlowDirection flow_direction;

  // Is this the first BlowerSystemState returned for a brand-new breath cycle?
  //
  // This is defaulted to false here and is handled by BlowerFsm, rather than
  // the individual FSM classes (OffFsm, PressureControlFsm, etc).
  bool is_new_breath = false;

  // Is this the first BlowerSystemState for a state different from Off state?
  // Used to reset some variables (volume integrators) to 0 when we start the
  // ventilator (transition from Off mode to any other mode).
  bool start_ventilation = false;
};

// Transition from PEEP to PIP pressure over this length of time.  Citation:
// https://respiraworks.slack.com/archives/C011CJQV4Q7/p1591763842312500?thread_ts=1591759016.310200&cid=C011CJQV4Q7
inline constexpr Duration RISE_TIME = milliseconds(100);

// A "breath finite state machine" where the blower is always off.
//
// All breath FSMs should implement the following "duck-typed API".
//
//  - <constructor>(Time now, const VentParams& params):
//    Constructs a new FSM for a single breath starting at the given time and
//    with the given params.  Those params don't change during the life of the
//    FSM.
//
//  - void Update(Time now, const BlowerFsmInputs& inputs):
//    Updates the state of the fsm based on time and values in inputs.
//
//  - BlowerSystemState DesiredState(): Gets the solenoid open/closed
//    state and the pressure that the fan should be trying to hit for the
//    current state of the fsm.
//
//  - bool Finished(): Has this breath FSM completed its work (namely, running
//    a single breath) ?
//    If so, it is ready to be replaced with a new one.
//
class OffFsm {
public:
  OffFsm() = default;
  explicit OffFsm(Time now, const VentParams &) {}
  void Update(Time now, const BlowerFsmInputs &inputs) {}
  BlowerSystemState DesiredState() {
    return {
        .pressure_setpoint = std::nullopt,
        // TODO: It doesn't make much sense to specify a flow direction when the
        // device is off and therefore there's no flow!  It might be better to
        // have a different BlowerSystemState struct for different categories of
        // modes: One for pressure modes, one for volume modes, one for flow
        // modes (high-flow nasal cannula), and one for the off mode.
        FlowDirection::EXPIRATORY,
    };
  }
  bool Finished() { return true; }
};

// "Breath finite state machine" for pressure control mode.
class PressureControlFsm {
public:
  explicit PressureControlFsm(Time now, const VentParams &params);
  void Update(Time now, const BlowerFsmInputs &inputs);
  BlowerSystemState DesiredState() const;
  bool Finished() const { return finished_; }

private:
  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  Pressure setpoint_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
  bool inspire_finished_ = false;
  bool finished_ = false;
};

// "Breath finite state machine" for pressure assist mode.
//
// This mode is used for a patient that is not fully sedated and can initiate
// breaths by himself, but once a breath has started, it is fully controlled
// by the ventilator: inspiration time is set by the ventilator.
// This mode enforce a minimum respiratory rate in case the patient does not
// initiate enough breaths.
// Similarly to pressure control, uses a square wave, but the duration of the
// exhale leg can vary depending on the patient breath initiation, meaning the
// breath can be finished (and the next one started) before the full breath
// duration has passed.
// In case the full breath duration passes, the fsm enforces a "mandatory
// breath" to make sure the patient's RR does not go below the desired one.
// Because the inhale is the first leg and only the exhale leg can vary in
// duration, the first breath in this mode is always a "mandatory breath".
class PressureAssistFsm {
public:
  explicit PressureAssistFsm(Time now, const VentParams &params);
  void Update(Time now, const BlowerFsmInputs &inputs);
  BlowerSystemState DesiredState() const;
  bool Finished() const { return finished_; }

private:
  bool PatientInspiring(const BlowerFsmInputs &inputs);

  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  Pressure setpoint_;
  Time start_time_;
  Time inspire_end_;
  Time expire_deadline_;

  // initialize threshold at insanely high flow to not trigger breath before
  // a proper threshold has been computed
  VolumetricFlow inspiratory_effort_threshold_ = cubic_m_per_sec(9);
  bool inspire_finished_ = false;
  bool finished_ = false;
};

class BlowerFsm {
public:
  // Gets the state that the the blower system should (ideally) deliver right
  // now.
  BlowerSystemState DesiredState(Time now, const VentParams &params,
                                 const BlowerFsmInputs &inputs);

private:
  std::variant<OffFsm, PressureControlFsm, PressureAssistFsm> fsm_;
};

#endif // BLOWER_FSM_H
