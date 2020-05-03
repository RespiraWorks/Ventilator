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

#include "blower_fsm.h"

#include "hal.h"
#include "new.h"
#include "units.h"

// This module is responsible for generating an ideal pressure graph over time.
// It's then up to the PID module to realize this pressure, as closely as it
// can.
//
// The main() loop queries blower_fsm_desired_state() on each iteration.
// get_setpoint() delegates to a "breath FSM" (e.g. PressureControlFsm), which
// is responsible for *one breath* with a fixed mode and VentParams.  When that
// breath ends, we create a new inner FSM with (potentially) new params.
//
// Decision that params should only change at breath boundaries:
// https://respiraworks.slack.com/archives/CV4MTUJHF/p1588001011133500

namespace {

// A "breath finite state machine" where the blower is always off.
//
// All breath FSMs should implement the following "duck-typed API".
//
//  - <constructor>(const VentParams& params): Constructs a new FSM for a
//    single breath, with the given params.  Those params don't change during
//    the life of the FSM.
//
//  - BlowerSystemState desired_state(): Gets the solenoid open/closed state
//    and the pressure that the fan should be trying to hit at this point in
//    time.
//
//  - bool finished(): Has this breath FSM completed its work (namely, running
//    a single breath)?  If so, it is ready to be replaced with a new one.
//
class OffFsm {
public:
  OffFsm() = default;
  explicit OffFsm(const VentParams &) {}
  BlowerSystemState desired_state() {
    return {.blower_enabled = false, kPa(0), ValveState::OPEN};
  }
  bool finished() { return true; }
};

// "Breath finite state machine" for pressure control mode.
//
// Currently our pressure-control mode drives a simple square wave: We go from
// PIP pressure on inhale to PEEP pressure on exhale.  Edwin currently
// (2020-04-29) believes that the physical limitations of the system (i.e. the
// fact that the blower can't spin up instantaneously) will lead this to being
// an acceptable waveform, although it remains to be seen.
class PressureControlFsm {
public:
  explicit PressureControlFsm(const VentParams &params)
      : inspire_pressure_(cmH2O(params.pip_cm_h2o)),
        expire_pressure_(cmH2O(params.peep_cm_h2o)), start_time_(Hal.now()),
        inspire_end_(start_time_ + inspire_duration(params)),
        expire_end_(inspire_end_ + expire_duration(params)) {}

  BlowerSystemState desired_state() {
    if (Hal.now() < inspire_end_) {
      return {.blower_enabled = true, inspire_pressure_, ValveState::CLOSED};
    }
    return {.blower_enabled = true, expire_pressure_, ValveState::OPEN};
  }

  bool finished() { return Hal.now() > expire_end_; }

private:
  // Given t = secs_per_breath and r = I:E ratio, calculate inspiration and
  // expiration durations (I and E).
  //
  //   t = I + E
  //   r = I / E
  //
  // implies
  //
  //  I = t * r / (1 + r)
  //  E =     t / (1 + r)
  //
  // https://www.wolframalpha.com/input/?i=solve+t+%3D+x+%2B+y+and+r+%3D+x%2Fy+for+x%2Cy
  static Duration inspire_duration(const VentParams &params) {
    float t = 60.f / params.breaths_per_min;       // secs per breath
    float r = params.inspiratory_expiratory_ratio; // I:E
    return seconds(t * r / (1 + r));
  }
  static Duration expire_duration(const VentParams &params) {
    float t = 60.f / params.breaths_per_min;       // secs per breath
    float r = params.inspiratory_expiratory_ratio; // I:E
    return seconds(t / (1 + r));
  }

  const Pressure inspire_pressure_;
  const Pressure expire_pressure_;
  Time start_time_;
  Time inspire_end_;
  Time expire_end_;
};

// A discriminated union of "breath finite state machines".
//
// TODO(jlebar): Replace with std::variant once we have an STL.  I tried to
// hack together my own variant class, but it was ultimately disappointing.
class FsmUnion {
public:
  FsmUnion() : mode_(), u_{OffFsm()} {}

  // Replaces the existing breath FSM with a new one, of the given mode and
  // params.
  void new_breath(VentMode m, const VentParams &params) {
    switch (mode_) {
    case VentMode_OFF:
      u_.off.~OffFsm();
      break;
    case VentMode_PRESSURE_CONTROL:
      u_.pressure_control.~PressureControlFsm();
      break;
    }

    mode_ = m;
    switch (mode_) {
    case VentMode_OFF:
      new (&u_.off) OffFsm(params);
      break;
    case VentMode_PRESSURE_CONTROL:
      new (&u_.pressure_control) PressureControlFsm(params);
      break;
    }
  }

  // Gets the state the FSM would like us to (try to) achieve.
  BlowerSystemState desired_state() {
    switch (mode_) {
    case VentMode_OFF:
      return u_.off.desired_state();
    case VentMode_PRESSURE_CONTROL:
      return u_.pressure_control.desired_state();
    }
    // All cases covered above (and gcc checks this).
    __builtin_unreachable();
  }

  // Returns whether or not the current FSM is done with its one breath.
  bool finished() {
    switch (mode_) {
    case VentMode_OFF:
      return u_.off.finished();
    case VentMode_PRESSURE_CONTROL:
      return u_.pressure_control.finished();
    }
    // All cases covered above (and gcc checks this).
    __builtin_unreachable();
  }

private:
  VentMode mode_;
  union U {
    OffFsm off;
    PressureControlFsm pressure_control;
  };
  U u_;
};

FsmUnion fsm;

} // anonymous namespace

void blower_fsm_init() {}

BlowerSystemState blower_fsm_desired_state(const VentParams &params) {
  // Immediately turn off the ventilator if params.mode == OFF; otherwise, wait
  // until the end of a cycle before implementing the mode change.
  if (params.mode == VentMode_OFF || fsm.finished()) {
    fsm.new_breath(params.mode, params);
  }

  return fsm.desired_state();
}
