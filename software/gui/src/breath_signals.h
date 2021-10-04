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

#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <optional>

#include "chrono.h"
#include "network_protocol.pb.h"

class BreathSignals {
 public:
  void Update(SteadyInstant now, const ControllerStatus &status);
  uint32_t num_breaths() const;
  std::optional<float> pip() const;
  std::optional<float> peep() const;
  std::optional<float> rr() const;

 private:
  uint32_t num_breaths_ = 0;

  std::optional<float> latest_pip_;
  std::optional<float> current_pip_;

  std::optional<float> latest_peep_;
  std::optional<float> current_peep_;

  uint64_t latest_breath_id_ = 0;

  static constexpr int MinRecentBreathStarts = 3;
  static constexpr int MaxRecentBreathStarts = 5;
  std::deque<SteadyInstant> recent_breath_starts_;
};
