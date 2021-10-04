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

#include "alarm_manager.h"

AlarmManager::AlarmManager() {
  for (auto *alarm : alarms_) {
    QObject::connect(alarm, &LatchingAlarm::updated, this, &AlarmManager::updated);
  }
}

void AlarmManager::Update(SteadyInstant now, const ControllerStatus &status,
                          const BreathSignals &breath_signals) {
  for (auto *alarm : alarms_) {
    alarm->Update(now, status, breath_signals);
  }
}

LatchingAlarm *AlarmManager::GetHighestPriorityActiveAlarm() {
  LatchingAlarm *res = nullptr;
  for (auto *alarm : alarms_) {
    if (res == nullptr || alarm->GetEffectiveAudioPriority() > res->GetEffectiveAudioPriority()) {
      res = alarm;
    }
  }
  return res;
}

LatchingAlarm *AlarmManager::GetHighestPrioritySilencedAlarm() {
  LatchingAlarm *res = nullptr;
  for (auto *alarm : alarms_) {
    if (!alarm->GetSilencedUntil().has_value()) continue;
    if (res == nullptr || alarm->GetNominalPriority() > res->GetNominalPriority()) {
      res = alarm;
    }
  }
  return res;
}

int AlarmManager::GetNumActiveAlarms() const {
  int res = 0;
  for (auto *alarm : alarms_) {
    if (alarm->IsAudioActive()) ++res;
  }
  return res;
}

int AlarmManager::GetNumSilencedAlarms() const {
  int res = 0;
  for (auto *alarm : alarms_) {
    if (alarm->GetSilencedUntil().has_value()) ++res;
  }
  return res;
}

Q_INVOKABLE void AlarmManager::acknowledgeHighestPriorityActiveAlarm() {
  GetHighestPriorityActiveAlarm()->Acknowledge(SteadyClock::now());
}

PipExceededAlarm *AlarmManager::get_pip_exceeded_alarm() { return &pip_exceeded_alarm_; }

PipNotReachedAlarm *AlarmManager::get_pip_not_reached_alarm() { return &pip_not_reached_alarm_; }

PatientDetachedAlarm *AlarmManager::get_patient_detached_alarm() {
  return &patient_detached_alarm_;
}
