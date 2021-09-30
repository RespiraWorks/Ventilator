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
  void Update(SteadyInstant now, const ControllerStatus &status) {
    float pressure = status.sensor_readings.patient_pressure_cm_h2o;
    uint64_t breath_id = status.sensor_readings.breath_id;

    if (breath_id != latest_breath_id_) {
      ++num_breaths_;
      latest_breath_id_ = status.sensor_readings.breath_id;

      latest_pip_ = current_pip_;
      current_pip_ = pressure;

      latest_peep_ = current_peep_;
      current_peep_ = pressure;

      recent_breath_starts_.push_back(now);
      if (recent_breath_starts_.size() > MaxRecentBreathStarts) {
        recent_breath_starts_.pop_front();
      }
      return;
    }

    current_pip_ = std::max(current_pip_.value_or(std::numeric_limits<float>::min()), pressure);
    current_peep_ = std::min(current_peep_.value_or(std::numeric_limits<float>::max()), pressure);
  }

  uint32_t num_breaths() const { return num_breaths_; }
  std::optional<float> pip() const { return latest_pip_; }
  std::optional<float> peep() const { return latest_peep_; }
  std::optional<float> rr() const {
    if (recent_breath_starts_.size() < MinRecentBreathStarts) {
      return std::nullopt;
    }
    SteadyInstant newest = recent_breath_starts_.back();
    SteadyInstant oldest = recent_breath_starts_.front();
    float ms_per_breath = static_cast<float>(TimeAMinusB(newest, oldest).count()) /
                          static_cast<float>(recent_breath_starts_.size() - 1);
    return 60000.0 / ms_per_breath;
  }

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
