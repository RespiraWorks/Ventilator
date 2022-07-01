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

#include "units.h"
#include "vars.h"

// Sensor readings etc that are used by breath detection.
struct BreathDetectionInputs {
  // Current patient volume, corrected for sensor zero-point drift.
  Volume patient_volume;
  // inflow - outflow, corrected for sensor zero-point drift.
  VolumetricFlow net_flow;
};

// Provides a class that is used to detecting inspiratory effort.
class BreathDetection {
 public:
  BreathDetection();
  // This method tells the system if a patient is inspiring and must be called
  // only during exhale phase. Parameters:
  // - net_flow, to allow inspire detection
  // - at_dwell, telling BreathDetection that the dwell plateau has been reached
  // TODO: automatically detect dwell plateau from flow
  bool PatientInspiring(const BreathDetectionInputs &inputs, bool at_dwell);

 private:
  // During breath we maintain two exponentially-weighted averages of flow, one
  // which updates quickly (fast_flow_avg_), and one which updates slowly (slow_flow_avg_).
  //
  // When fast_flow_avg_ exceeds slow_flow_avg_ by a threshold, we trigger a breath.
  //
  // More discussion of this algorithm:
  // https://respiraworks.slack.com/archives/C011CJQV4Q7/p1592417313120400
  VolumetricFlow fast_flow_avg_{ml_per_sec(0)};
  VolumetricFlow slow_flow_avg_{ml_per_sec(0)};
};
