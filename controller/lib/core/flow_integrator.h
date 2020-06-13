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

#include "hal.h"
#include "pid.h"
#include "units.h"

// Integrates flow over time to calculate volume.
//
// Also computes a constant correction to the flow that tries to account for
// slow-changing error in the flow, based on certain points in time when we
// know what the volume "should be".
class FlowIntegrator {
public:
  FlowIntegrator();

  // Updates the tidal volume given this flow.
  //
  // FlowIntegrator will correct this flow according to FlowCorrection() --
  // don't pre-correct it yourself!
  void AddFlow(Time now, VolumetricFlow uncorrected_flow);

  Volume GetVolume() const { return volume_; }

  // FlowIntegrator adds this value to measured flow when computing volume, in
  // an attempt to drive flow to the "correct" volume (as given by
  // NoteExpectedVolume).
  //
  // Code that wants "corrected flow" (almost everything) should use measured
  // flow plus this value.
  VolumetricFlow FlowCorrection() const { return flow_correction_; }

  // Informs FlowIntegrator what the tidal volume "ought to have been" as of
  // the last time AddFlow was called.
  //
  // FlowIntegrator will then adjust its flow offset to try account for this in
  // the future.
  //
  // TODO: How will this work with pressure assist mode?  In pressure control
  // mode, the ventilator breathes on a precise schedule, so we know exactly
  // when TV "should be" 0.  But in pressure assist mode, the patient triggers
  // some breaths themselves, and it takes us some time to detect inspiratory
  // effort.  By the point that we've detected effort, some flow has occurred,
  // so the correct TV is not 0.  But then, what *is* it?
  //
  // A possible brute-force solution would be to keep history of the past N
  // tidal volumes.
  void NoteExpectedVolume(Volume v);

private:
  // Our flow sensors are subject to roughly two kinds of error:
  //
  //  - high-frequency error (i.e. "noise"), which we handle by reading the ADC
  //    many times ("oversampling") in HAL, and
  //
  //  - low-frequency error, i.e. zero-point drift, meaning that the flow we
  //    observe is equal to true flow plus an error, and that error changes
  //    slowly relative to how quickly we can read the sensor.
  //
  // We use a PID controller invoked at each call to NoteExpectedVolume to
  // estimate the zero-point drift.  If the patient is breathing normally,
  // tidal volume should be 0 between breath.  The job of the PID is to output
  // a flow offset so that the measured flow plus the offset takes tidal volume
  // to its "correct" value at the next breath.
  //
  //  - PID input: Tidal volume in ml right between two breaths.
  //  - PID setpoint: Desired volume between breaths, i.e. 0 ml.
  //  - PID output: A flow in ml/sec that should be added to every measured
  //    flow.
  //
  // TODO: When the ventilator blower is disabled and re-enabled, presumably we
  // should clear some or all of these values!
  PID flow_correction_pid_;
  VolumetricFlow flow_correction_ = cubic_m_per_sec(0);

  Time last_flow_measurement_time_ = Hal.now();
  Volume volume_ = ml(0);
  VolumetricFlow last_flow_ = cubic_m_per_sec(0);
};
