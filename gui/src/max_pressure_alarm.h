#ifndef MAX_PRESSURE_ALARM_H_
#define MAX_PRESSURE_ALARM_H_

#include "chrono.h"
#include "network_protocol.pb.h"

#include <optional>

// An alarm for the condition "pressure > threshold".
// Has two signals: visual and audio.
// The signals are latching, i.e. they stay active until suppressed even if
// pressure went back below threshold.
// The audio signal can be suppressed for 2 minutes via Acknowledge().
// The visual signal can be cleared via Reset().
// The terminology is taken from
// https://github.com/RespiraWorks/VentilatorSoftware/wiki/Alarm-Subsystem.
//
// This class is deliberately single-purpose for now. As we develop more alarms,
// we'll need to introduce some base classes and ways of handling alarms
// generically.
class MaxPressureAlarm {
public:
  // Updates the state of the alarm and its signals according to current
  // sensor readings.
  void Update(SteadyInstant now, const ControllerStatus &status) {
    currently_active_ =
        (status.sensor_readings.patient_pressure_cm_h2o > threshold_cmh2o_);
    if (currently_active_ && !latched_since_.has_value()) {
      latched_since_ = now;
    }
    if (audio_suppressed_until_.has_value() && now > *audio_suppressed_until_) {
      audio_suppressed_until_ = std::nullopt;
    }
  }

  // Whether the audio signal should currently be active.
  bool IsAudioActive(SteadyInstant now) {
    return latched_since_.has_value() &&
           now > audio_suppressed_until_.value_or(SteadyInstant::min());
  }

  // Whether the visual signal should currently be active.
  bool IsVisualActive([[maybe_unused]] SteadyInstant now) {
    return latched_since_.has_value();
  }

  // Whether the ACKNOWLEDGE action should currently be available.
  bool CanAcknowledge() {
    return latched_since_.has_value() && !audio_suppressed_until_.has_value();
  }
  // ACKNOWLEDGEs the alarm, suppressing audio for 2 minutes.
  void Acknowledge(SteadyInstant now) {
    if (!CanAcknowledge())
      return;
    audio_suppressed_until_ = now + DurationMs(120'000);
  }

  // Whether the RESET action should currently be available.
  bool CanReset() { return latched_since_.has_value() && !currently_active_; }
  // RESETs the alarm, clearing both audio and visual signals until
  // the next time the condition fires.
  void Reset([[maybe_unused]] SteadyInstant now) {
    if (!CanReset())
      return;
    latched_since_ = std::nullopt;
    audio_suppressed_until_ = std::nullopt;
  }

  float GetThresholdCmH2O() const { return threshold_cmh2o_; }
  void SetThresholdCmH2O(float threshold_cmh2o) {
    threshold_cmh2o_ = threshold_cmh2o;
  }

private:
  float threshold_cmh2o_ = 60;

  // Whether on the last Update() call the alarm condition was met.
  bool currently_active_ = false;

  // Present if the alarm is currently signaling; if so, tells when the signal
  // began.
  // Becomes present when currently_active_ transitions from false to true.
  // Becomes absent via Reset().
  std::optional<SteadyInstant> latched_since_ = std::nullopt;

  // Present if the alarm is currently signaling AND the audio signal is
  // currently suppressed.
  // Becomes present via Acknowledge().
  // Becomes absent when either of these things happens:
  // - Reset() is called
  // - Update() is called with a time later than this.
  std::optional<SteadyInstant> audio_suppressed_until_ = std::nullopt;
};

#endif // MAX_PRESSURE_ALARM_H_
