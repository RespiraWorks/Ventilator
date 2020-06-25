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

#include "breath_detection.h"
#include "controller.h"
#include "vars.h"
#include <algorithm>

// dbg_bd_* are breath detector configuration vars.
//
// These are read but never modified here.

// TODO: This should be configurable from the GUI.
static DebugFloat dbg_bd_flow_trigger("bd_flow_trigger",
                                      "breath detection flow trigger (ml/s)",
                                      200);

// pa_fast_avg_alpha and pa_slow_avg_alpha were tuned for a control loop that
// runs at a particular frequency.
//
// In theory if the control loop gets slower, the alpha terms should get
// bigger, placing more weight on newer readings, and similarly if the control
// loop gets faster, the alpha terms should get smaller.  We've tried to encode
// this here, although it remains to be seen if it actually works.
static DebugFloat dbg_bd_fast_avg_alpha(
    "bd_fast_avg_alpha",
    "alpha term in breath detection's fast-updating "
    "exponentially-weighted average of flow",
    0.2f * (Controller::GetLoopPeriod() / milliseconds(10)));
static DebugFloat dbg_bd_slow_avg_alpha(
    "bd_slow_avg_alpha",
    "alpha term in breath detection's slow-updating "
    "exponentially-weighted average of flow",
    0.01f * (Controller::GetLoopPeriod() / milliseconds(10)));

static DebugFloat dbg_fast_flow_avg("fast_flow_avg",
                                    "fast-updating flow average (ml/s)", 0.f);
static DebugFloat dbg_slow_flow_avg("slow_flow_avg",
                                    "slow-updating flow average (ml/s)", 0.f);

BreathDetection::BreathDetection() {
  dbg_slow_flow_avg.Set(0.f);
  dbg_fast_flow_avg.Set(0.f);
};

bool BreathDetection::PatientInspiring(const BreathDetectionInputs &inputs,
                                       bool at_dwell) {
  if (inputs.net_flow < ml_per_sec(0)) {
    return false;
  }

  // Once flow is non-negative, start calculating two
  // exponentially-weighted averages of net flow: slow_avg_flow_ and
  // fast_avg_flow_.
  //
  // The slow one has a smaller alpha term, so updates slower than the fast
  // one.  You can think of the slow average as estimating "flow at dwell" and
  // the fast average as estimating "current flow".
  //
  // If the fast average exceeds the slow average by a threshold, we trigger a
  // breath.
  float slow_alpha = dbg_bd_slow_avg_alpha.Get();
  float fast_alpha = dbg_bd_fast_avg_alpha.Get();
  // TODO: This could be encapsulated in an exponentially-weighted-average
  // class.
  slow_avg_flow_ = slow_alpha * inputs.net_flow +
                   (1 - slow_alpha) * slow_avg_flow_.value_or(inputs.net_flow);
  fast_avg_flow_ = fast_alpha * inputs.net_flow +
                   (1 - fast_alpha) * fast_avg_flow_.value_or(inputs.net_flow);

  dbg_slow_flow_avg.Set(slow_avg_flow_->ml_per_min());
  dbg_fast_flow_avg.Set(fast_avg_flow_->ml_per_min());

  return at_dwell &&
         *fast_avg_flow_ >
             *slow_avg_flow_ + ml_per_sec(dbg_bd_flow_trigger.Get());
}
