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

/// \TODO: this is only here for loop period for default trigger values. Get rid of this
#include "controller.h"

#include <algorithm>

using DebugFloat = Debug::Variable::Float;

// Note that since BreathDetection is instantiated for each breath, these debugvars are static and
// apply to any and all breath detection instances, to make them permanent and therefore traceable.
// This is fine because there can only ever be a single breath detection instance at any given time.

/// \TODO This should be configurable from the GUI.
static DebugFloat inhale_trigger{"bd_inhale_trigger", Debug::Variable::Access::ReadWrite, 200,
                                   "mL/s", "Breath detection inhale trigger"};

static DebugFloat exhale_trigger("bd_exhale_trigger", Debug::Variable::Access::ReadWrite, 300,
                                     "mL/s", "Breath detection exhale trigger");

// fast_alpha_ and slow_alpha_ were tuned for a control loop that runs at a particular frequency.
// In theory if the control loop gets slower, the alpha terms should get bigger, placing more weight
// on newer readings, and similarly if the control loop gets faster, the alpha terms should get
// smaller.  We've tried to encode this here, although it remains to be seen if it actually works.
static DebugFloat dbg_fast_alpha{"bd_fast_flow_avg_alpha", Debug::Variable::Access::ReadWrite,
                                 0.2f * (Controller::GetLoopPeriod() / milliseconds(10)), "",
                                 "alpha term for fast-updating exponentially-weighted average"
                                 " of flow used for breath detection"};
static DebugFloat dbg_slow_alpha{"bd_slow_flow_avg_alpha", Debug::Variable::Access::ReadWrite,
                                 0.01f * (Controller::GetLoopPeriod() / milliseconds(10)), "",
                                 "alpha term for slow-updating exponentially-weighted average"
                                 " of flow used for breath detection"};
static DebugFloat dbg_fast_flow_avg{"bd_fast_flow_avg", Debug::Variable::Access::ReadOnly, 0.0f,
                                    "mL/s", "fast-updating flow average for breath detection"};
static DebugFloat dbg_slow_flow_avg{"bd_slow_flow_avg", Debug::Variable::Access::ReadOnly, 0.0f,
                                    "mL/s", "fast-updating flow average for breath detection"};

BreathDetection::BreathDetection() {
  dbg_fast_flow_avg.set(0);
  dbg_slow_flow_avg.set(0);
};

void BreathDetection::update_averages(const BreathDetectionInputs &inputs) {
  // Calculate two exponentially-weighted averages of net flow: slow_flow_avg_ and fast_flow_avg_.
  //
  // The slow one has a smaller alpha term, so updates slower than the fast one.
  // You can think of the slow average as estimating "flow at dwell" and
  // the fast average as estimating "current flow".
  //
  // If the fast average exceeds the slow average by a threshold, we trigger a breath.
  float slow_alpha = dbg_slow_alpha.get();
  float fast_alpha = dbg_fast_alpha.get();

  slow_flow_avg_ = slow_alpha * inputs.net_flow + (1 - slow_alpha) * slow_flow_avg_;
  fast_flow_avg_ = fast_alpha * inputs.net_flow + (1 - fast_alpha) * fast_flow_avg_;

  dbg_slow_flow_avg.set(slow_flow_avg_.ml_per_sec());
  dbg_fast_flow_avg.set(fast_flow_avg_.ml_per_sec());
}


bool BreathDetection::PatientInspiring(const BreathDetectionInputs &inputs, bool at_dwell) {
  // Calculate averages only once flow is non-negative
  if (inputs.net_flow < ml_per_sec(0)) {
    return false;
  }

  update_averages(inputs);

  return at_dwell && fast_flow_avg_ - slow_flow_avg_ > ml_per_sec(inhale_trigger.get());
}

bool BreathDetection::PatientExhaling(const BreathDetectionInputs &inputs, bool at_dwell) {
  // Calculate averages only once flow is non-negative
  if (inputs.net_flow < ml_per_sec(0)) {
    return false;
  }

  update_averages(inputs);

  return slow_flow_avg_ > fast_flow_avg_ + ml_per_sec(exhale_trigger.get());
}

