#ifndef PIP_EXCEEDED_ALARM_H_
#define PIP_EXCEEDED_ALARM_H_

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

#include <QtCore/QObject>

// An alarm for the condition "PIP > threshold".
class PipExceededAlarm : public LatchingAlarm {
  Q_OBJECT

private:
  bool IsActive(SteadyInstant now, const ControllerStatus &status,
                const BreathSignals &breath_signals) override {
    (void)now;
    (void)status;
    return breath_signals.pip().value_or(0) > threshold_cmh2o_;
  }

public:
  float GetThresholdCmH2O() const { return threshold_cmh2o_; }
  void SetThresholdCmH2O(float threshold_cmh2o) {
    threshold_cmh2o_ = threshold_cmh2o;
    updated();
  }

private:
  float threshold_cmh2o_ = 60;
};

#endif // PIP_EXCEEDED_ALARM_H_
