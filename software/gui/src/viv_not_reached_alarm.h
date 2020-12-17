#ifndef VIV_NOT_REACHED_ALARM_H_
#define VIV_NOT_REACHED_ALARM_H_

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

#include <cmath>

#include <QObject>
#include <QString>

// An alarm for the condition "VIV < threshold".
class VivNotReachedAlarm : public LatchingAlarm {
  Q_OBJECT

public:
  VivNotReachedAlarm() : LatchingAlarm(AlarmPriority::MEDIUM) {}

private:
  std::optional<QString>
  IsActive(SteadyInstant now, const ControllerStatus &status,
           const BreathSignals &breath_signals) override {
    (void)now;
    (void)status;
    if (breath_signals.num_breaths() < 4)
      return std::nullopt;
    if( breath_signals.currentmode() != 4 || breath_signals.currentmode() != 6  )
            return std::nullopt;
    auto viv = breath_signals.viv();// .pip();
    if (viv.has_value() && std::ceil(*viv) < threshold_ml_) {
      return {QString("VIV not reached (%1 < %2)")
                  .arg(QString::number(*viv, 'f', 0),
                       QString::number(threshold_ml_, 'f', 0))};
    }
    return std::nullopt;
  }

public:
  float GetThresholdml() const { return threshold_ml_; }
  void SetThresholdml(float threshold_ml) {
    threshold_ml_ = threshold_ml;
    updated();
  }

private:
  float threshold_ml_ = 20;
};

#endif // PIP_NOT_REACHED_ALARM_H_
