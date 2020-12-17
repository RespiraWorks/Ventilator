#ifndef VIV_EXCEEDED_ALARM_H_
#define VIV_EXCEEDED_ALARM_H_

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

#include <cmath>

#include <QObject>
#include <QString>

// An alarm for the condition "VIV > threshold".
class VivExceededAlarm : public LatchingAlarm {
  Q_OBJECT

public:
  VivExceededAlarm() : LatchingAlarm(AlarmPriority::HIGH) {}

private:
  std::optional<QString>
  IsActive(SteadyInstant now, const ControllerStatus &status,
           const BreathSignals &breath_signals) override {


    (void)now;
    (void)status;
    if (breath_signals.num_breaths() < 4)
      return std::nullopt;
     ;
    if( breath_signals.currentmode() != 4 || breath_signals.currentmode() != 6  )
        return std::nullopt;

    float viv = breath_signals.viv().value_or(0);
    if (std::floor(viv) > threshold_ml_) {
      return {QString("VIV exceeded (%1 > %2)")
                  .arg(QString::number(viv, 'f', 0),
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
  float threshold_ml_ = 50;
};

#endif // VIV_EXCEEDED_ALARM_H_
