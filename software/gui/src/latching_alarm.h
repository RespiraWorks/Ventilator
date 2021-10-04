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
#include <optional>

#include "breath_signals.h"
#include "chrono.h"
#include "network_protocol.pb.h"

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
// https://github.com/RespiraWorks/Ventilator/wiki/Alarm-Subsystem.
class LatchingAlarm : public QObject {
  Q_OBJECT

 private:
  // If the alarm condition is active, returns a human-readable string
  // describing it, otherwise nullopt.
  virtual std::optional<QString> IsActive(SteadyInstant now, const ControllerStatus &status,
                                          const BreathSignals &breath_signals) = 0;

 protected:
  explicit LatchingAlarm(AlarmPriority::Enum priority) : priority_(priority) {}

 public:
  virtual ~LatchingAlarm() = default;

  Q_PROPERTY(AlarmPriority::Enum nominalPriority READ GetNominalPriority CONSTANT)
  Q_PROPERTY(
      AlarmPriority::Enum effectiveVisualPriority READ GetEffectiveVisualPriority NOTIFY updated)
  Q_PROPERTY(
      AlarmPriority::Enum effectiveAudioPriority READ GetEffectiveAudioPriority NOTIFY updated)
  Q_PROPERTY(QString bannerText READ GetBannerText NOTIFY updated)
  Q_PROPERTY(int remainingSilenceMs READ GetRemainingSilenceMs NOTIFY updated)

  // Updates the state of the alarm and its signals according to current
  // sensor readings.
  void Update(SteadyInstant now, const ControllerStatus &status,
              const BreathSignals &breath_signals);

  AlarmPriority::Enum GetNominalPriority() const;

  AlarmPriority::Enum GetEffectiveVisualPriority() const;

  AlarmPriority::Enum GetEffectiveAudioPriority() const;

  // Whether the audio signal should currently be active.
  // ACKNOWLEDGE is available if this returns true.
  bool IsAudioActive() const;

  // Whether the visual signal should currently be active.
  bool IsVisualActive() const;

  QString GetBannerText() const;

  // ACKNOWLEDGEs the alarm, suppressing audio for 2 minutes.
  void Acknowledge(SteadyInstant now);

  std::optional<SteadyInstant> GetSilencedUntil() const;

  int GetRemainingSilenceMs();

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
