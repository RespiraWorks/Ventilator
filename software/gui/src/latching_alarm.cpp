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

#include "latching_alarm.h"

// Updates the state of the alarm and its signals according to current
// sensor readings.
void LatchingAlarm::Update(SteadyInstant now, const ControllerStatus &status,
                           const BreathSignals &breath_signals) {
  auto banner_text = IsActive(now, status, breath_signals);
  is_condition_active_ = banner_text.has_value();
  if (is_condition_active_) {
    banner_text_ = banner_text;
  }
  switch (audio_state_) {
    case AudioState::INACTIVE:
      audio_state_ = is_condition_active_ ? AudioState::BEEPING : AudioState::INACTIVE;
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
        audio_state_ = is_condition_active_ ? AudioState::BEEPING : AudioState::INACTIVE;
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

AlarmPriority::Enum LatchingAlarm::GetNominalPriority() const { return priority_; }

AlarmPriority::Enum LatchingAlarm::GetEffectiveVisualPriority() const {
  return IsVisualActive() ? GetNominalPriority() : AlarmPriority::NONE;
}

AlarmPriority::Enum LatchingAlarm::GetEffectiveAudioPriority() const {
  return IsAudioActive() ? GetNominalPriority() : AlarmPriority::NONE;
}

// Whether the audio signal should currently be active.
// ACKNOWLEDGE is available if this returns true.
bool LatchingAlarm::IsAudioActive() const { return audio_state_ == AudioState::BEEPING; }

// Whether the visual signal should currently be active.
bool LatchingAlarm::IsVisualActive() const { return is_condition_active_; }

QString LatchingAlarm::GetBannerText() const { return banner_text_.value_or(""); }

// ACKNOWLEDGEs the alarm, suppressing audio for 2 minutes.
void LatchingAlarm::Acknowledge(SteadyInstant now) {
  if (!IsAudioActive()) return;
  audio_state_ = AudioState::SILENCED;
  silenced_until_ = now + DurationMs(120'000);
  updated();
}

std::optional<SteadyInstant> LatchingAlarm::GetSilencedUntil() const { return silenced_until_; }

int LatchingAlarm::GetRemainingSilenceMs() {
  return silenced_until_.has_value() ? TimeAMinusB(*silenced_until_, SteadyClock::now()).count()
                                     : 0;
}
