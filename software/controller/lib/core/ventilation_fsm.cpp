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

#include "ventilation_fsm.h"

#include <algorithm>

#include "system_constants.h"
#include "vars.h"

// dbg_pa_* are pressure assist configuration vars.
//
// These are read but never modified here.

// This is a pressure assist tuning parameter, read but never modified here
// TODO: Is 250ms right?  Or can it be a fixed value at all; should it depend
// on the RR or something?
static Debug::Variable::Float dbg_pa_min_expire_ms(
    "pa_min_expire_ms", Debug::Variable::Access::ReadWrite, 250, "ms",
    "minimum amount of time after ventilator exits PIP "
    "before we're eligible to trigger a breath");

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
static Duration InspireDuration(const VentParams &params) {
  float t = 60.f / static_cast<float>(params.breaths_per_min);  // secs per breath
  float r = params.inspiratory_expiratory_ratio;                // I:E
  return seconds(t * r / (1 + r));
}
static Duration ExpireDuration(const VentParams &params) {
  float t = 60.f / static_cast<float>(params.breaths_per_min);  // secs per breath
  float r = params.inspiratory_expiratory_ratio;                // I:E
  return seconds(t / (1 + r));
}

PressureControlFsm::PressureControlFsm(Time now, const VentParams &params)
    : inspire_pressure_(cmH2O(static_cast<float>(params.pip_cm_h2o))),
      expire_pressure_(cmH2O(static_cast<float>(params.peep_cm_h2o))),
      start_time_(now),
      inspire_end_(start_time_ + InspireDuration(params)),
      expire_end_(inspire_end_ + ExpireDuration(params)) {}

VentilationSystemState PressureControlFsm::DesiredState(Time now,
                                                        const BreathDetectionInputs &inputs) {
  if (now < inspire_end_) {
    // Go from expire_pressure_ to inspire_pressure_ over a duration of
    // RiseTime.  Then for the rest of the inspire time, hold at
    // inspire_pressure_.
    static_assert(RiseTime > milliseconds(0));
    float rise_frac = std::min(1.f, (now - start_time_) / RiseTime);
    return {
        .pressure_setpoint = expire_pressure_ + (inspire_pressure_ - expire_pressure_) * rise_frac,
        .flow_direction = FlowDirection::Inspiratory,
        .pip = inspire_pressure_,
        .peep = expire_pressure_,
        .is_end_of_breath = false,
    };
  }
  // expiratory part of the cycle
  return {
      .pressure_setpoint = expire_pressure_,
      .flow_direction = FlowDirection::Expiratory,
      .pip = inspire_pressure_,
      .peep = expire_pressure_,
      .is_end_of_breath = (now >= expire_end_),
  };
}

PressureAssistFsm::PressureAssistFsm(Time now, const VentParams &params)
    : inspire_pressure_(cmH2O(static_cast<float>(params.pip_cm_h2o))),
      expire_pressure_(cmH2O(static_cast<float>(params.peep_cm_h2o))),
      start_time_(now),
      inspire_end_(start_time_ + InspireDuration(params)),
      expire_deadline_(inspire_end_ + ExpireDuration(params)) {}

VentilationSystemState PressureAssistFsm::DesiredState(Time now,
                                                       const BreathDetectionInputs &inputs) {
  if (now < inspire_end_) {
    // Go from expire_pressure_ to inspire_pressure_ over a duration of
    // RiseTime.  Then for the rest of the inspire time, hold at
    // inspire_pressure_.
    static_assert(RiseTime > milliseconds(0));
    float rise_frac = std::min(1.f, (now - start_time_) / RiseTime);
    return {
        .pressure_setpoint = expire_pressure_ + (inspire_pressure_ - expire_pressure_) * rise_frac,
        .flow_direction = FlowDirection::Inspiratory,
        .pip = inspire_pressure_,
        .peep = expire_pressure_,
        .is_end_of_breath = false,
    };
  }
  // expiratory part of the cycle
  return {
      .pressure_setpoint = expire_pressure_,
      .flow_direction = FlowDirection::Expiratory,
      .pip = inspire_pressure_,
      .peep = expire_pressure_,
      .is_end_of_breath =
          (now >= expire_deadline_ ||
           inspire_detection_.PatientInspiring(
               inputs,
               /*at_dwell=*/now > inspire_end_ + milliseconds(dbg_pa_min_expire_ms.get()))),
  };
}

VentilationSystemState VentilationFsm::DesiredState(Time now, const VentParams &params,
                                                    const BreathDetectionInputs &inputs) {
  VentilationSystemState s =
      std::visit([&](auto &fsm) { return fsm.DesiredState(now, inputs); }, fsm_);

  // Before returning the state just obtained, we check if a mode change is
  // needed. If the ventilator is being switched on, recompute the desired
  // state.
  //
  // Implement the mode change if at least one of the following is true:
  // (1) current mode is off and params.mode is not OFF:
  //     immediately turn on the ventilator and recompute desired state
  // (2) params.mode == OFF: immediately turn off the ventilator
  // (3) just-obtained desired state `s` indicates that this is the end of
  //     the breath cycle: create an FSM for the new breath in accordance
  //     with the desired mode in params.mode
  bool switching_on = (std::holds_alternative<OffFsm>(fsm_) && params.mode != VentMode_OFF);
  bool switching_off = (params.mode == VentMode_OFF);

  if (switching_on || switching_off || s.is_end_of_breath) {
    switch (params.mode) {
      case VentMode_OFF:
        fsm_.emplace<OffFsm>(now, params);
        break;
      case VentMode_PRESSURE_CONTROL:
        fsm_.emplace<PressureControlFsm>(now, params);
        break;
      case VentMode_PRESSURE_ASSIST:
        fsm_.emplace<PressureAssistFsm>(now, params);
        break;
      case VentMode_HIGH_FLOW_NASAL_CANNULA:
        // TODO: Implement me. For now, keep mode unchanged.
        break;
    }
  }
  if (switching_on) {
    s = std::visit([&](auto &fsm) { return fsm.DesiredState(now, inputs); }, fsm_);
  }
  return s;
}
