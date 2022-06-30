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

#include "breath_detection.h"

#include <algorithm>

/// \TODO: This should be configurable from the GUI.
static Debug::Variable::Float dbg_bd_flow_trigger{
    "flow_trigger", Debug::Variable::Access::ReadWrite, 200, "mL/s", "flow trigger"};

BreathDetection::BreathDetection(const Duration loop_period, const char *mode)
    :  // fast_avg_alpha_ and slow_avg_alpha_ were tuned for a control loop that
       // runs at a particular frequency.
       //
       // In theory if the control loop gets slower, the alpha terms should get
       // bigger, placing more weight on newer readings, and similarly if the control
       // loop gets faster, the alpha terms should get smaller.  We've tried to encode
       // this here, although it remains to be seen if it actually works.
      fast_alpha_("fast_flow_avg_alpha", Debug::Variable::Access::ReadWrite,
                  0.2f * (loop_period / milliseconds(10)), "",
                  "alpha term for fast-updating "
                  "exponentially-weighted average of flow"),
      slow_alpha_("slow_flow_avg_alpha", Debug::Variable::Access::ReadWrite,
                  0.01f * (loop_period / milliseconds(10)), "",
                  "alpha term for slow-updating "
                  "exponentially-weighted average of flow") {
  fast_alpha_.prepend_name(mode);
  slow_alpha_.prepend_name(mode);
  fast_flow_avg_.prepend_name(mode);
  slow_flow_avg_.prepend_name(mode);
};

bool BreathDetection::PatientInspiring(const BreathDetectionInputs &inputs, bool at_dwell) {
  if (inputs.net_flow < ml_per_sec(0)) {
    return false;
  }

  // Once flow is non-negative, start calculating two
  // exponentially-weighted averages of net flow: slow_flow_avg_ and fast_flow_avg_.
  //
  // The slow one has a smaller alpha term, so updates slower than the fast one.
  // You can think of the slow average as estimating "flow at dwell" and
  // the fast average as estimating "current flow".
  //
  // If the fast average exceeds the slow average by a threshold, we trigger a breath.
  float slow_alpha = slow_alpha_.get();
  float fast_alpha = fast_alpha_.get();

  // Use VolumetricFlow to make the code worry about units for us in the calculations
  VolumetricFlow slow_average{ml_per_sec(slow_flow_avg_.get())};
  VolumetricFlow fast_average{ml_per_sec(fast_flow_avg_.get())};

  slow_average = slow_alpha * inputs.net_flow + (1 - slow_alpha) * slow_average;
  fast_average = fast_alpha * inputs.net_flow + (1 - fast_alpha) * fast_average;

  slow_flow_avg_.set(slow_average.ml_per_sec());
  fast_flow_avg_.set(fast_average.ml_per_sec());

  return at_dwell && fast_average - slow_average > ml_per_sec(dbg_bd_flow_trigger.get());
}
