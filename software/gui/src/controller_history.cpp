/* Copyright 2020-2021, RespiraWorks

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

#include "controller_history.h"

ControllerHistory::ControllerHistory(DurationMs window, DurationMs granularity)
    : window_(window), granularity_(granularity) {}

bool ControllerHistory::Append(SteadyInstant gui_now, const ControllerStatus &status) {
  if (!history_.empty() && gui_now - std::get<SteadyInstant>(history_.back()) < granularity_) {
    return false;
  }
  history_.push_back({gui_now, status});
  // Kick out points that are too old.
  while (!history_.empty() && gui_now - std::get<SteadyInstant>(history_.front()) > window_) {
    history_.pop_front();
  }
  return true;
}

int ControllerHistory::Size() const { return history_.size(); }

std::vector<std::tuple<SteadyInstant, ControllerStatus>> ControllerHistory::GetHistory() const {
  return {history_.begin(), history_.end()};
}

ControllerStatus ControllerHistory::GetLastStatus() const {
  if (history_.empty()) {
    return ControllerStatus_init_zero;
  }
  return std::get<1>(history_.back());
}
