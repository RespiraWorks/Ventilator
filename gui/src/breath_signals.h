#ifndef BREATH_SIGNALS_H_
#define BREATH_SIGNALS_H_

#include <stdint.h>

#include "chrono.h"
#include "network_protocol.pb.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <optional>

class BreathSignals {
public:
  void Update(SteadyInstant now, const ControllerStatus &status) {
    float pressure = status.sensor_readings.patient_pressure_cm_h2o;
    uint64_t breath_id = status.sensor_readings.breath_id;

    if (breath_id != latest_breath_id_) {
      latest_pip_ = current_pip_;
      current_pip_ = pressure;

      latest_peep_ = current_peep_;
      current_peep_ = pressure;

      latest_breath_id_ = status.sensor_readings.breath_id;

      recent_breath_starts_.push_back(now);
      if (recent_breath_starts_.size() > kMaxRecentBreathStarts) {
        recent_breath_starts_.pop_front();
      }
      return;
    }

    current_pip_ = std::max(
        current_pip_.value_or(std::numeric_limits<float>::min()), pressure);
    current_peep_ = std::min(
        current_peep_.value_or(std::numeric_limits<float>::max()), pressure);
  }

  std::optional<float> pip() const { return latest_pip_; }
  std::optional<float> peep() const { return latest_peep_; }
  std::optional<float> rr() const {
    if (recent_breath_starts_.size() < kMinRecentBreathStarts) {
      return std::nullopt;
    }
    SteadyInstant newest = recent_breath_starts_.back();
    SteadyInstant oldest = recent_breath_starts_.front();
    float ms_per_breath =
        static_cast<float>(TimeAMinusB(newest, oldest).count()) /
        static_cast<float>(recent_breath_starts_.size() - 1);
    return 60000.0 / ms_per_breath;
  }

private:
  std::optional<float> latest_pip_;
  std::optional<float> current_pip_;

  std::optional<float> latest_peep_;
  std::optional<float> current_peep_;

  uint64_t latest_breath_id_ = 0;

  static constexpr int kMinRecentBreathStarts = 3;
  static constexpr int kMaxRecentBreathStarts = 5;
  std::deque<SteadyInstant> recent_breath_starts_;
};

#endif // BREATH_SIGNALS_H_
