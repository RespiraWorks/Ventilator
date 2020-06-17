#ifndef MAX_PRESSURE_ALARM_H_
#define MAX_PRESSURE_ALARM_H_

#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

#include <optional>

// An alarm for the condition "pressure > threshold".
class MaxPressureAlarm : public LatchingAlarm {
private:
  bool IsActive(SteadyInstant now, const ControllerStatus &status) override {
    (void)now;
    return (status.sensor_readings.patient_pressure_cm_h2o > threshold_cmh2o_);
  }

public:
  float GetThresholdCmH2O() const { return threshold_cmh2o_; }
  void SetThresholdCmH2O(float threshold_cmh2o) {
    threshold_cmh2o_ = threshold_cmh2o;
  }

private:
  float threshold_cmh2o_ = 60;
};

#endif // MAX_PRESSURE_ALARM_H_
