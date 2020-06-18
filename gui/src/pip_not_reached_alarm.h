#ifndef PIP_NOT_REACHED_ALARM_H_
#define PIP_NOT_REACHED_ALARM_H_

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

#include <QObject>
#include <QString>

// An alarm for the condition "PIP < threshold".
class PipNotReachedAlarm : public LatchingAlarm {
  Q_OBJECT

public:
  PipNotReachedAlarm() : LatchingAlarm(AlarmPriority::MEDIUM) {}

private:
  std::optional<QString>
  IsActive(SteadyInstant now, const ControllerStatus &status,
           const BreathSignals &breath_signals) override {
    (void)now;
    (void)status;
    auto pip = breath_signals.pip();
    if (pip.has_value() && *pip < threshold_cmh2o_) {
      return {QString("PIP not reached: measured %1, threshold %2")
                  .arg(QString::number(*pip, 'f', 1),
                       QString::number(threshold_cmh2o_, 'f', 1))};
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
  float threshold_cmh2o_ = 10;
};

#endif // PIP_NOT_REACHED_ALARM_H_
