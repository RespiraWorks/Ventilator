#ifndef LATCHING_ALARM_H_
#define LATCHING_ALARM_H_

#include "breath_signals.h"
#include "chrono.h"
#include "network_protocol.pb.h"

#include <optional>

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
class LatchingAlarm {
private:
  virtual bool IsActive(SteadyInstant now, const ControllerStatus &status,
                        const BreathSignals &breath_signals) = 0;

public:
  virtual ~LatchingAlarm() = default;

  // Updates the state of the alarm and its signals according to current
  // sensor readings.
  void Update(SteadyInstant now, const ControllerStatus &status,
              const BreathSignals &breath_signals) {
    currently_active_ = IsActive(now, status, breath_signals);
    switch (audio_state_) {
    case AudioState::INACTIVE:
      audio_state_ =
          currently_active_ ? AudioState::BEEPING : AudioState::INACTIVE;
      break;
    case AudioState::BEEPING:
      audio_state_ = AudioState::BEEPING;
      break;
    case AudioState::SILENCED:
      if (now > silenced_until_) {
        silenced_until_ = std::nullopt;
        audio_state_ =
            currently_active_ ? AudioState::BEEPING : AudioState::INACTIVE;
      } else {
        audio_state_ = currently_active_ ? AudioState::SILENCED
                                         : (silenced_until_ = std::nullopt,
                                            AudioState::INACTIVE);
      }
      break;
    }
  }

  // Whether the audio signal should currently be active.
  // ACKNOWLEDGE is available if this returns true.
  bool IsAudioActive() const { return audio_state_ == AudioState::BEEPING; }

  // Whether the visual signal should currently be active.
  bool IsVisualActive() const { return currently_active_; }

  // ACKNOWLEDGEs the alarm, suppressing audio for 2 minutes.
  void Acknowledge(SteadyInstant now) {
    if (!IsAudioActive())
      return;
    audio_state_ = AudioState::SILENCED;
    silenced_until_ = now + DurationMs(120'000);
  }

private:
  // Whether on the last Update() call the alarm condition was met.
  bool currently_active_ = false;

  enum class AudioState { INACTIVE, BEEPING, SILENCED };

  AudioState audio_state_ = AudioState::INACTIVE;
  // Present only in SILENCED.
  std::optional<SteadyInstant> silenced_until_ = std::nullopt;
};

#endif // LATCHING_ALARM_H_
