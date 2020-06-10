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
#include <algorithm>

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
  float t =
      60.f / static_cast<float>(params.breaths_per_min); // secs per breath
  float r = params.inspiratory_expiratory_ratio;         // I:E
  return seconds(t * r / (1 + r));
}
static Duration expire_duration(const VentParams &params) {
  float t =
      60.f / static_cast<float>(params.breaths_per_min); // secs per breath
  float r = params.inspiratory_expiratory_ratio;         // I:E
  return seconds(t / (1 + r));
}

PressureControlFsm::PressureControlFsm(Time now, const VentParams &params)
    : inspire_pressure_(cmH2O(static_cast<float>(params.pip_cm_h2o))),
      expire_pressure_(cmH2O(static_cast<float>(params.peep_cm_h2o))),
      start_time_(now), inspire_end_(start_time_ + inspire_duration(params)),
      expire_end_(inspire_end_ + expire_duration(params)) {}

BlowerSystemState PressureControlFsm::desired_state(Time now) {
  if (now < inspire_end_) {
    // Go from expire_pressure_ to inspire_pressure_ over a duration of
    // RISE_TIME.  Then for the rest of the inspire time, hold at
    // inspire_pressure_.
    static_assert(RISE_TIME > milliseconds(0));
    float rise_frac = std::min(1.f, (now - start_time_) / RISE_TIME);
    Pressure setpoint =
        expire_pressure_ + (inspire_pressure_ - expire_pressure_) * rise_frac;

    return {.blower_enabled = true, setpoint, ValveState::CLOSED};
  }
  return {.blower_enabled = true, expire_pressure_, ValveState::OPEN};
}

BlowerSystemState BlowerFsm::DesiredState(Time now, const VentParams &params) {
  // Immediately turn off the ventilator if params.mode == OFF; otherwise,
  // wait until the end of a cycle before implementing the mode change.
  bool is_new_breath = false;
  if ((params.mode == VentMode_OFF && !std::holds_alternative<OffFsm>(fsm_)) ||
      std::visit([&](auto &fsm) { return fsm.finished(now); }, fsm_)) {
    // Set is_new_breath to true even when the ventilator transitions from on
    // to off.  It's a little arbitrary, but for the most part, is_new_breath
    // is used to mark breath boundaries rather than simply signal the
    // beginning of a breath, and it would be weird if the last breath before
    // turning off the ventilator appeared to continue indefinitely.
    is_new_breath = true;
    switch (params.mode) {
    case VentMode_OFF:
      fsm_.emplace<OffFsm>(now, params);
      break;
    case VentMode_PRESSURE_CONTROL:
      fsm_.emplace<PressureControlFsm>(now, params);
      break;
    }
  }

  BlowerSystemState s =
      std::visit([&](auto &fsm) { return fsm.desired_state(now); }, fsm_);
  s.is_new_breath = is_new_breath;
  return s;
}
