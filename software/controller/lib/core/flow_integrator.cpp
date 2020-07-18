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
#include "vars.h"

// TODO: VOLUME_INTEGRAL_INTERVAL was not chosen carefully.
static constexpr Duration VOLUME_INTEGRAL_INTERVAL = milliseconds(5);

FlowIntegrator::FlowIntegrator() = default;

void FlowIntegrator::AddFlow(Time now, VolumetricFlow uncorrected_flow) {
  VolumetricFlow flow = uncorrected_flow + flow_correction_;

  // First time?  In that case, leave the volume at 0 for now, just record the
  // data point.
  if (last_flow_measurement_time_ == std::nullopt) {
    last_note_time_ = now;
    last_note_vol_ = ml(0);
    last_flow_measurement_time_ = now;
    last_flow_ = flow;
    return;
  }

  Duration time_since_last_note = now - *last_flow_measurement_time_;
  if (time_since_last_note >= VOLUME_INTEGRAL_INTERVAL) {
    volume_ += time_since_last_note * (last_flow_ + flow) / 2.0f;
    last_flow_measurement_time_ = now;
    last_flow_ = flow;
  }
}

void FlowIntegrator::NoteExpectedVolume(Volume v) {
  // We need at least one call to AddFlow before we can do anything.
  if (last_note_time_ == std::nullopt ||
      last_flow_measurement_time_ == std::nullopt ||
      *last_note_time_ >= *last_flow_measurement_time_) {
    return;
  }

  // Compute the new flow correction as follows.
  //
  // Consider three points in time:
  //
  // - t0, start time of previous breath (technically, time of previous call to
  //   NoteExpectedVolume),
  // - t1, now, end of previous breath / start of current breath, and
  // - t2, end of current breath (a point in time in the future).
  //
  // Let the duration t1 - t0 time_since_last_note.  It's our assumption/hope
  // that NoteExpectedVolume is called regularly, so we assume that t2 - t1
  // will be equal to t1 - t0.
  //
  // We calculate the amount that uncorrected volume has drifted as `dv`, and
  // we assume that `dv` at time t1 is a good approximation for dv at t2.
  //
  // Using this, we compute a flow correction that will compensate for dv over
  // the next breath, making corrected volume at t2 hopefully be very close
  // to the desired volume v passed to this function.
  //
  // NOTE: You might be tempted to apply some hysteresis to dv, e.g. using an
  // exponentially-weighted average.  Experimenting with the data checked in
  // under sample-data suggests this doesn't help.  dv changes by as much as
  // 15ml/s between breaths, but at least from my eyeballing of the data, using
  // multiple historical values of dv makes our prediction of the next dv
  // worse.
  //
  // TODO: That said, this algorithm handles coughing, stacked breaths, etc
  // very badly, treating basically anything other than a completely uniform
  // breath as flow error, and aggressively "correcting" it.
  //
  // TODO: Similarly, if the time until the next breath happens to be much
  // longer than time_since_last_note, we may seriously overcorrect the volume.

  // Compute the time delta between now (last_flow_measurement_time_) and the
  // last call to NoteExpectedVolume.
  Duration time_since_last_note =
      *last_flow_measurement_time_ - *last_note_time_;

  // Compute what our volume would have been if we hadn't applied any flow
  // correction during the last call to NoteExpectedVolume.
  Volume v_uncorrected = volume_ - flow_correction_ * time_since_last_note;

  // Compute the difference in volume between now and the previous breath
  // we would have seen if we hadn't applied correction.
  Volume dv = v_uncorrected - last_note_vol_;

  // Assume that dv will remain constant.  Then we can compute the expected
  // volume after the next breath.
  Volume v_next = volume_ + dv;

  // Finally, choose a flow correction that drives v_next to v, assuming
  // time_since_last_note remains constant.
  flow_correction_ = (v - v_next) / time_since_last_note;

  last_note_time_ = last_flow_measurement_time_;
  last_note_vol_ = volume_;
}
