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
#include "controller.h"
#include "vars.h"
#include <algorithm>

// TODO(jlebar): Refactor this so that instead of is_new_breath, we have
// is_end_of_breath.  Then we can merge Update and DesiredState(), getting rid
// of a lot of mutable state in the FSMs.

// dbg_pa_* are pressure assist configuration vars.
//
// These are read but never modified here.

// TODO: This should be configurable from the GUI.
static DebugFloat dbg_pa_flow_trigger("pa_flow_trigger",
                                      "pressure assist flow trigger (ml/s)",
                                      200);

// TODO: Is 250ms right?  Or can it be a fixed value at all; should it depend
// on the RR or something?
static DebugFloat
    dbg_pa_min_expire_ms("pa_min_expire_ms",
                         "minimum amount of time after ventilator exits PIP "
                         "before we're eligible to trigger a breath",
                         250);

// pa_fast_avg_alpha and pa_slow_avg_alpha were tuned for a control loop that
// runs at a particular frequency.
//
// In theory if the control loop gets slower, the alpha terms should get
// bigger, placing more weight on newer readings, and similarly if the control
// loop gets faster, the alpha terms should get smaller.  We've tried to encode
// this here, although it remains to be seen if it actually works.
static DebugFloat dbg_pa_fast_avg_alpha(
    "pa_fast_avg_alpha",
    "alpha term in pressure assist mode's fast-updating "
    "exponentially-weighted average of flow",
    0.2f * (Controller::GetLoopPeriod() / milliseconds(10)));
static DebugFloat dbg_pa_slow_avg_alpha(
    "pa_slow_avg_alpha",
    "alpha term in pressure assist mode's slow-updating "
    "exponentially-weighted average of flow",
    0.01f * (Controller::GetLoopPeriod() / milliseconds(10)));

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
  float t =
      60.f / static_cast<float>(params.breaths_per_min); // secs per breath
  float r = params.inspiratory_expiratory_ratio;         // I:E
  return seconds(t * r / (1 + r));
}
static Duration ExpireDuration(const VentParams &params) {
  float t =
      60.f / static_cast<float>(params.breaths_per_min); // secs per breath
  float r = params.inspiratory_expiratory_ratio;         // I:E
  return seconds(t / (1 + r));
}

PressureControlFsm::PressureControlFsm(Time now, const VentParams &params)
    : inspire_pressure_(cmH2O(static_cast<float>(params.pip_cm_h2o))),
      expire_pressure_(cmH2O(static_cast<float>(params.peep_cm_h2o))),
      setpoint_(expire_pressure_), start_time_(now),
      inspire_end_(start_time_ + InspireDuration(params)),
      expire_end_(inspire_end_ + ExpireDuration(params)) {}

void PressureControlFsm::Update(Time now, const BlowerFsmInputs &inputs) {
  if (now >= inspire_end_) {
    inspire_finished_ = true;
  } else {
    // Go from expire_pressure_ to inspire_pressure_ over a duration of
    // RISE_TIME.  Then for the rest of the inspire time, hold at
    // inspire_pressure_.
    static_assert(RISE_TIME > milliseconds(0));
    float rise_frac = std::min(1.f, (now - start_time_) / RISE_TIME);
    setpoint_ =
        expire_pressure_ + (inspire_pressure_ - expire_pressure_) * rise_frac;
  }
  if (now >= expire_end_) {
    finished_ = true;
  }
}

BlowerSystemState PressureControlFsm::DesiredState() const {
  if (inspire_finished_) {
    return {
        .pressure_setpoint = expire_pressure_,
        .flow_direction = FlowDirection::EXPIRATORY,
        .pip = inspire_pressure_,
        .peep = expire_pressure_,
    };
  }
  return {
      .pressure_setpoint = setpoint_,
      .flow_direction = FlowDirection::INSPIRATORY,
      .pip = inspire_pressure_,
      .peep = expire_pressure_,
  };
}

PressureAssistFsm::PressureAssistFsm(Time now, const VentParams &params)
    : inspire_pressure_(cmH2O(static_cast<float>(params.pip_cm_h2o))),
      expire_pressure_(cmH2O(static_cast<float>(params.peep_cm_h2o))),
      setpoint_(expire_pressure_), start_time_(now),
      inspire_end_(start_time_ + InspireDuration(params)),
      expire_deadline_(inspire_end_ + ExpireDuration(params)) {}

void PressureAssistFsm::Update(Time now, const BlowerFsmInputs &inputs) {
  if (now >= inspire_end_) {
    inspire_finished_ = true;
  } else {
    // Go from expire_pressure_ to inspire_pressure_ over a duration of
    // RISE_TIME.  Then for the rest of the inspire time, hold at
    // inspire_pressure_.
    static_assert(RISE_TIME > milliseconds(0));
    float rise_frac = std::min(1.f, (now - start_time_) / RISE_TIME);
    setpoint_ =
        expire_pressure_ + (inspire_pressure_ - expire_pressure_) * rise_frac;
  }
  if (now >= expire_deadline_ || PatientInspiring(now, inputs)) {
    finished_ = true;
  }
}

BlowerSystemState PressureAssistFsm::DesiredState() const {
  if (inspire_finished_) {
    return {
        .pressure_setpoint = expire_pressure_,
        .flow_direction = FlowDirection::EXPIRATORY,
        .pip = inspire_pressure_,
        .peep = expire_pressure_,
    };
  }
  return {
      .pressure_setpoint = setpoint_,
      .flow_direction = FlowDirection::INSPIRATORY,
      .pip = inspire_pressure_,
      .peep = expire_pressure_,
  };
}

// TODO don't rely on fsm inner states to make this usable in any fsm
bool PressureAssistFsm::PatientInspiring(Time now,
                                         const BlowerFsmInputs &inputs) {
  if (!inspire_finished_ || inputs.net_flow < ml_per_sec(0)) {
    return false;
  }

  // Once we're done inspiring and flow is non-negative, start calculating two
  // exponentially-weighted averages of net flow: slow_avg_flow_ and
  // fast_avg_flow_.
  //
  // The slow one has a smaller alpha term, so updates slower than the fast
  // one.  You can think of the slow average as estimating "flow at dwell" and
  // the fast average as estimating "current flow".
  //
  // If the fast average exceeds the slow average by a threshold, we trigger a
  // breath.
  float slow_alpha = dbg_pa_slow_avg_alpha.Get();
  float fast_alpha = dbg_pa_fast_avg_alpha.Get();

  // TODO: This could be encapsulated in an exponentially-weighted-average
  // class.
  slow_avg_flow_ = slow_alpha * inputs.net_flow +
                   (1 - slow_alpha) * slow_avg_flow_.value_or(inputs.net_flow);
  fast_avg_flow_ = fast_alpha * inputs.net_flow +
                   (1 - fast_alpha) * fast_avg_flow_.value_or(inputs.net_flow);

  return now >= inspire_end_ + milliseconds(dbg_pa_min_expire_ms.Get()) &&
         *fast_avg_flow_ >
             *slow_avg_flow_ + ml_per_sec(dbg_pa_flow_trigger.Get());
}

BlowerSystemState BlowerFsm::DesiredState(Time now, const VentParams &params,
                                          const BlowerFsmInputs &inputs) {
  // Immediately turn off the ventilator if params.mode == OFF; otherwise,
  // wait until the end of a cycle before implementing the mode change.
  bool is_new_breath = false;

  std::visit([&](auto &fsm) { return fsm.Update(now, inputs); }, fsm_);

  if (params.mode == VentMode_OFF ||
      std::visit([&](auto &fsm) { return fsm.Finished(); }, fsm_)) {
    // For the purpose of is_new_breath, treat multiple occurrences of OffFsm
    // as a "single breath".
    is_new_breath = !std::holds_alternative<OffFsm>(fsm_);

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

  BlowerSystemState s =
      std::visit([&](auto &fsm) { return fsm.DesiredState(); }, fsm_);
  s.is_new_breath = is_new_breath;
  return s;
}
