#ifndef LATCHING_ALARM_H_
#define LATCHING_ALARM_H_

#include "breath_signals.h"
#include "chrono.h"
#include "network_protocol.pb.h"

#include <QObject>
#include <QString>

#include <optional>

class AlarmPriority {
  Q_GADGET

public:
  enum Enum {
    NONE,
    LOW,
    MEDIUM,
    HIGH,
  };

  Q_ENUM(Enum)
};

// An alarm with a latching audio signal that can be silenced.
// Has two signals: visual and audio.
// * visual signal is non-latching: it is active when the condition is active.
// * audio signal is latching, i.e. stays active until suppressed even if
//   condition is not active anymore.
// Audio can be suppressed for up to 2 minutes via Acknowledge().
// Suppression applies to the current occurrence, i.e. it expires either after
// 2 minutes or when the condition becomes inactive, whichever is earlier.
// The terminology is taken from
// https://github.com/RespiraWorks/VentilatorSoftware/wiki/Alarm-Subsystem.
class LatchingAlarm : public QObject {
  Q_OBJECT

private:
  // If the alarm condition is active, returns a human-readable string
  // describing it, otherwise nullopt.
  virtual std::optional<QString>
  IsActive(SteadyInstant now, const ControllerStatus &status,
           const BreathSignals &breath_signals) = 0;

protected:
  explicit LatchingAlarm(AlarmPriority::Enum priority) : priority_(priority) {}

public:
  virtual ~LatchingAlarm() = default;

  Q_PROPERTY(
      AlarmPriority::Enum nominalPriority READ GetNominalPriority CONSTANT)
  Q_PROPERTY(AlarmPriority::Enum effectiveVisualPriority READ
                 GetEffectiveVisualPriority NOTIFY updated)
  Q_PROPERTY(AlarmPriority::Enum effectiveAudioPriority READ
                 GetEffectiveAudioPriority NOTIFY updated)
  Q_PROPERTY(QString bannerText READ GetBannerText NOTIFY updated)
  Q_PROPERTY(int remainingSilenceMs READ GetRemainingSilenceMs NOTIFY updated)

  // Updates the state of the alarm and its signals according to current
  // sensor readings.
  void Update(SteadyInstant now, const ControllerStatus &status,
              const BreathSignals &breath_signals) {
    auto banner_text = IsActive(now, status, breath_signals);
    is_condition_active_ = banner_text.has_value();
    if (is_condition_active_) {
      banner_text_ = banner_text;
    }
    switch (audio_state_) {
    case AudioState::INACTIVE:
      audio_state_ =
          is_condition_active_ ? AudioState::BEEPING : AudioState::INACTIVE;
      break;
    case AudioState::BEEPING:
      audio_state_ = AudioState::BEEPING;
      break;
    case AudioState::SILENCED:
      if (now > silenced_until_) {
        // Silencing period elapsed.
        // If condition is active, we should go back to beeping, otherwise go
        // back to normal.
        silenced_until_ = std::nullopt;
        audio_state_ =
            is_condition_active_ ? AudioState::BEEPING : AudioState::INACTIVE;
      } else {
        // Silencing period is still in effect.
        // If condition is active, silencing continues.
        // If condition stops being active while silent, that clears the
        // silencing because it applied only to the current occurrence of the
        // condition.
        if (is_condition_active_) {
          audio_state_ = AudioState::SILENCED;
        } else {
          silenced_until_ = std::nullopt;
          banner_text_ = std::nullopt;
          audio_state_ = AudioState::INACTIVE;
        }
      }
      break;
    }
    updated();
  }

  AlarmPriority::Enum GetNominalPriority() const { return priority_; }

  AlarmPriority::Enum GetEffectiveVisualPriority() const {
    return IsVisualActive() ? GetNominalPriority() : AlarmPriority::NONE;
  }

  AlarmPriority::Enum GetEffectiveAudioPriority() const {
    return IsAudioActive() ? GetNominalPriority() : AlarmPriority::NONE;
  }

  // Whether the audio signal should currently be active.
  // ACKNOWLEDGE is available if this returns true.
  bool IsAudioActive() const { return audio_state_ == AudioState::BEEPING; }

  // Whether the visual signal should currently be active.
  bool IsVisualActive() const { return is_condition_active_; }

  QString GetBannerText() const { return banner_text_.value_or(""); }

  // ACKNOWLEDGEs the alarm, suppressing audio for 2 minutes.
  void Acknowledge(SteadyInstant now) {
    if (!IsAudioActive())
      return;
    audio_state_ = AudioState::SILENCED;
    silenced_until_ = now + DurationMs(120'000);
    updated();
  }

  std::optional<SteadyInstant> GetSilencedUntil() const {
    return silenced_until_;
  }

  int GetRemainingSilenceMs() {
    return silenced_until_.has_value()
               ? TimeAMinusB(*silenced_until_, SteadyClock::now()).count()
               : 0;
  }

signals:
  void updated();

private:
  bool is_condition_active_ = false;
  // Present if IsAudioActive().
  // Corresponds to the text returned by Update() last time the condition was
  // active.
  std::optional<QString> banner_text_;

  enum class AudioState { INACTIVE, BEEPING, SILENCED };

  const AlarmPriority::Enum priority_;
  AudioState audio_state_ = AudioState::INACTIVE;
  // Present only in SILENCED.
  std::optional<SteadyInstant> silenced_until_ = std::nullopt;
};

#endif // LATCHING_ALARM_H_
