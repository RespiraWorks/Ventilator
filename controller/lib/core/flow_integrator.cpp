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

#include "flow_integrator.h"

// TODO: VOLUME_INTEGRAL_INTERVAL was not chosen carefully.
static constexpr Duration VOLUME_INTEGRAL_INTERVAL = milliseconds(5);

// Tuning params for PID that calculates error in our flow measurements.
// Calculated using the
// https://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method The constants in
// the constructor are for the "classic PID" control type, which seemed to work
// well.
//
// FLOW_PID_OUTPUT_MAX is a sanity check, preventing us from using absurdly
// high flow corrections.  The value below may strike you as absurdly high, but
// we've seen corrections of 40ml/s be necessary, and 100ml/s gives a safety
// margin.
//
// Even 40ml/s may seem way higher than should ever occur, but it's actually
// easy to get an error this high at zero flow.  Since we have two sensors,
// we'd need an error of +/-20ml in each.  At zero flow, this corresponds to
// significantly less than 1 Pa of pressure, and just two or three bits in our
// 12-bit ADC!  See
// https://docs.google.com/spreadsheets/d/1G9Kb-ImlluK8MOx-ce2rlHUBnTOtAFQvKjjs1bEhlpM
//
// In theory the story is better at higher flows (because the venturi response
// is the square root of pressure), and we may run a bias flow through our
// device to put us into a regime where we're less sensitive to error
// (equivalently, where the venturi response has a lower slope).  But we still
// want to handle the low-flow case as best we can.
static constexpr float FLOW_PID_KU = 0.20f;
static constexpr float FLOW_PID_TU = 5;
static constexpr VolumetricFlow FLOW_PID_OUTPUT_MAX = ml_per_sec(100);

// TODO: Make PID parameters configurable.

FlowIntegrator::FlowIntegrator()
    : flow_correction_pid_(
          /*kp=*/0.6f * FLOW_PID_KU,
          /*ki=*/1.2f * FLOW_PID_KU / FLOW_PID_TU,
          /*kd=*/3 * FLOW_PID_KU * FLOW_PID_TU / 40,
          // ProportionalTerm::ON_ERROR and DifferentialTerm::ON_MEASUREMENT
          // makes for your "standard" PID.  TODO: perhaps these shouldn't even
          // be options.
          ProportionalTerm::ON_ERROR,        //
          DifferentialTerm::ON_MEASUREMENT,  //
          -FLOW_PID_OUTPUT_MAX.ml_per_sec(), //
          FLOW_PID_OUTPUT_MAX.ml_per_sec()) {}

void FlowIntegrator::AddFlow(Time now, VolumetricFlow uncorrected_flow) {
  VolumetricFlow flow = uncorrected_flow + flow_correction_;
  Duration delta = now - last_flow_measurement_time_;
  if (delta >= VOLUME_INTEGRAL_INTERVAL) {
    volume_ += delta * (last_flow_ + flow) / 2.0f;
    last_flow_measurement_time_ = now;
    last_flow_ = flow;
  }
}

void FlowIntegrator::NoteExpectedVolume(Volume v) {
  flow_correction_ = ml_per_sec(flow_correction_pid_.Compute(
      last_flow_measurement_time_, volume_.ml(), v.ml()));
}
