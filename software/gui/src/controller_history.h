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

#pragma once

#include <deque>
#include <tuple>
#include <vector>

#include "chrono.h"
#include "network_protocol.pb.h"

// Maintains a history of recent ControllerStatus-es sufficient
// for rendering the UI.
//
// Non-thread-safe, needs external synchronization.
class ControllerHistory {
 public:
  // Initializes the history to keep a window of given duration -
  // meaning, if the oldest point is more than this much older than
  // the point being added, it gets kicked out.
  //
  // "granularity" signals how many points to keep: if a new point is less than
  // this much later than the latest point, it doesn't get added.
  ControllerHistory(DurationMs window, DurationMs granularity);

  // Appends a ControllerStatus obtained at a given time point in GUI time.
  // We cannot use the controller's uptime, because if controller restarts,
  // uptime will appear to go backwards.
  // For a similar reason we also must use specifically a steady clock
  // (clock that never goes backwards) - as opposed to, say, the system clock.
  bool Append(SteadyInstant gui_now, const ControllerStatus &status);

  int Size() const;

  std::vector<std::tuple<SteadyInstant, ControllerStatus>> GetHistory() const;

  ControllerStatus GetLastStatus() const;

 private:
  DurationMs window_;
  DurationMs granularity_;
  std::deque<std::tuple<SteadyInstant, ControllerStatus>> history_;
};
