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

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"
#include "units.h"

// An alarm for the condition "patient is not connected".
class PatientDetachedAlarm : public LatchingAlarm {
  Q_OBJECT

 public:
  // TODO: This is a medium-priority alarm in the requirements, but are we sure
  // that's correct?  Seems kind of strange; if the patient isn't connected,
  // isn't that kind of a big deal?
  PatientDetachedAlarm() : LatchingAlarm(AlarmPriority::HIGH) {}

 private:
  std::optional<QString> IsActive(SteadyInstant /*now*/, const ControllerStatus &status,
                                  const BreathSignals & /*breath_signals*/) override {
    VolumetricFlow flow_correction =
        ml_per_min(status.sensor_readings().flow_correction_ml_per_min());
    if (flow_correction <= threshold_) {
      return {QString("Patient detached from device")};
    }
    return std::nullopt;
  }

 private:
  // I observe that disconnecting the test lung results in a flow correction of
  // about -1600ml/s even with PIP/PEEP set to 5cmH2O.  -500ml/s is
  // conservative, but still well below what we'd expect to see in normal
  // operation.
  static inline constexpr VolumetricFlow threshold_{ml_per_sec(-500)};
};
