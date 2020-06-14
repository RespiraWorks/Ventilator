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
    return {expire_pressure_, FlowDirection::EXPIRATORY};
  }
  return {setpoint_, FlowDirection::INSPIRATORY};
}

PressureAssistFsm::PressureAssistFsm(Time now, const VentParams &params)
    : inspire_pressure_(cmH2O(static_cast<float>(params.pip_cm_h2o))),
      expire_pressure_(cmH2O(static_cast<float>(params.peep_cm_h2o))),
      start_time_(now), inspire_end_(start_time_ + InspireDuration(params)),
      expire_deadline_(inspire_end_ + ExpireDuration(params)) {}

void PressureAssistFsm::Update(Time now, const BlowerFsmInputs &inputs) {
  if (now >= inspire_end_) {
    inspire_finished_ = true;
  }
  if (now >= expire_deadline_ || PatientInspiring(inputs)) {
    finished_ = true;
  }
}

BlowerSystemState PressureAssistFsm::DesiredState() const {
  if (inspire_finished_) {
    return {expire_pressure_, FlowDirection::EXPIRATORY};
  }
  return {inspire_pressure_, FlowDirection::INSPIRATORY};
}

// TODO don't rely on fsm inner states to make this usable in any fsm
bool PressureAssistFsm::PatientInspiring(const BlowerFsmInputs &inputs) {
  // TODO: change this when we get better understanding of inspiratory effort
  // detection strategy, right now this is just set to trigger for any flow
  // that is higher than twice the flowmeter's flow noise :
  //   flow = flow_inhale (+/- noise) - flow_exhale (+/- noise).
  // According to data from Ingmar test lung
  // (https://drive.google.com/open?id=11_A8KEDdBa5l7rqUegfKsj71rug1l7Sx),
  // this should work OK as a first approximation.
  if (inspire_finished_ && inputs.net_flow >= ml_per_min(0) &&
      inspiratory_effort_threshold_ >= cubic_m_per_sec(9)) {
    // Not sure how to go about this: when we enter this breath fsm, either we
    // were Off and flow should be 0, which means this is OK: the first
    // positive flow we see is either noise or the blower starting to provide
    // PEEP we can take that as an  "almost zero" reference flow, or we are at
    // PIP, I assume the flow will be negative until we reach PEEP, where it
    // will approximately be 0 and we may assume to be back to the first case.
    inspiratory_effort_threshold_ = inputs.net_flow + ml_per_min(12000.0f);
    // Note 12000 ml/min looks like a lot but it is actually 200 ml/sec which
    // is in the high end of our sensor's sensitivity for relatively low flows
    // that are expected during exhale, especially when summing two sensors.
    // I can see two ways to improve this sensitivity:
    //   a: provide a denoised flow in SensorReadings, at the cost of reducing
    //      response time
    //   b: provide separate inflow and outflow, at the cost of a more
    //      complicated (and not yet determined) breath detection algorithm.
  }

  return inspire_finished_ && inputs.net_flow > inspiratory_effort_threshold_;
}

BlowerSystemState BlowerFsm::DesiredState(Time now, const VentParams &params,
                                          const BlowerFsmInputs &inputs) {
  // Immediately turn off the ventilator if params.mode == OFF; otherwise,
  // wait until the end of a cycle before implementing the mode change.
  bool is_new_breath = false;

  std::visit([&](auto &fsm) { return fsm.Update(now, inputs); }, fsm_);

  if ((params.mode == VentMode_OFF && !std::holds_alternative<OffFsm>(fsm_)) ||
      std::visit([&](auto &fsm) { return fsm.Finished(); }, fsm_)) {
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
    case VentMode_PRESSURE_ASSIST:
      fsm_.emplace<PressureAssistFsm>(now, params);
      break;
    }
  }

  BlowerSystemState s =
      std::visit([&](auto &fsm) { return fsm.DesiredState(); }, fsm_);
  s.is_new_breath = is_new_breath;
  return s;
}
