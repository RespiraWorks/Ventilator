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

#include <QObject>
#include <QString>
#include <cmath>

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

// An alarm for the condition "PIP < threshold".
class PipNotReachedAlarm : public LatchingAlarm {
  Q_OBJECT

 public:
  PipNotReachedAlarm() : LatchingAlarm(AlarmPriority::MEDIUM) {}

 private:
  std::optional<QString> IsActive(SteadyInstant now, const ControllerStatus &status,
                                  const BreathSignals &breath_signals) override {
    (void)now;
    (void)status;
    if (breath_signals.num_breaths() < 3) return std::nullopt;
    auto pip = breath_signals.pip();
    if (pip.has_value() && std::ceil(*pip) < threshold_cmh2o_) {
      return {QString("PIP not reached (%1 < %2)")
                  .arg(QString::number(*pip, 'f', 0), QString::number(threshold_cmh2o_, 'f', 0))};
    }
    return std::nullopt;
  }

 public:
  float GetThresholdCmH2O() const { return threshold_cmh2o_; }
  void SetThresholdCmH2O(float threshold_cmh2o) {
    threshold_cmh2o_ = threshold_cmh2o;
    updated();
  }

 private:
  float threshold_cmh2o_{10};
};
