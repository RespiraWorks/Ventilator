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

#include <QDebug>
#include <QObject>
#include <QString>

#include "chrono.h"
#include "latching_alarm.h"
#include "patient_detached_alarm.h"
#include "pip_exceeded_alarm.h"
#include "pip_not_reached_alarm.h"

class AlarmManager : public QObject {
  Q_OBJECT
 public:
  AlarmManager();

  void Update(SteadyInstant now, const ControllerStatus &status,
              const BreathSignals &breath_signals);

  Q_PROPERTY(
      LatchingAlarm *highestPriorityActiveAlarm READ GetHighestPriorityActiveAlarm NOTIFY updated)
  Q_PROPERTY(LatchingAlarm *highestPrioritySilencedAlarm READ GetHighestPrioritySilencedAlarm NOTIFY
                 updated)
  Q_PROPERTY(int numActiveAlarms READ GetNumActiveAlarms NOTIFY updated)
  Q_PROPERTY(int numSilencedAlarms READ GetNumSilencedAlarms NOTIFY updated)

  LatchingAlarm *GetHighestPriorityActiveAlarm();

  LatchingAlarm *GetHighestPrioritySilencedAlarm();

  int GetNumActiveAlarms() const;

  int GetNumSilencedAlarms() const;

  Q_INVOKABLE void acknowledgeHighestPriorityActiveAlarm();

  Q_PROPERTY(PipExceededAlarm *pipExceededAlarm READ get_pip_exceeded_alarm CONSTANT)
  Q_PROPERTY(PipNotReachedAlarm *pipNotReachedAlarm READ get_pip_not_reached_alarm CONSTANT)
  Q_PROPERTY(PatientDetachedAlarm *patientDetachedAlarm READ get_patient_detached_alarm CONSTANT)

  PipExceededAlarm *get_pip_exceeded_alarm();
  PipNotReachedAlarm *get_pip_not_reached_alarm();
  PatientDetachedAlarm *get_patient_detached_alarm();

 signals:
  void updated();

 private:
  PipExceededAlarm pip_exceeded_alarm_;
  PipNotReachedAlarm pip_not_reached_alarm_;
  PatientDetachedAlarm patient_detached_alarm_;
  std::vector<LatchingAlarm *> alarms_{&pip_exceeded_alarm_, &pip_not_reached_alarm_,
                                       &patient_detached_alarm_};
};
