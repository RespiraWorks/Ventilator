#ifndef ALARM_MANAGER_H_
#define ALARM_MANAGER_H_

#include "chrono.h"
#include "latching_alarm.h"
#include "pip_exceeded_alarm.h"
#include "pip_not_reached_alarm.h"

#include <QDebug>
#include <QObject>
#include <QString>

class AlarmManager : public QObject {
  Q_OBJECT
public:
  AlarmManager() {
    for (auto *alarm : alarms_) {
      QObject::connect(alarm, &LatchingAlarm::updated, this,
                       &AlarmManager::updated);
    }
  }

  void Update(SteadyInstant now, const ControllerStatus &status,
              const BreathSignals &breath_signals) {
    for (auto *alarm : alarms_) {
      alarm->Update(now, status, breath_signals);
    }
  }

  Q_PROPERTY(LatchingAlarm *highestPriorityActiveAlarm READ
                 GetHighestPriorityActiveAlarm NOTIFY updated)
  Q_PROPERTY(LatchingAlarm *highestPrioritySilencedAlarm READ
                 GetHighestPrioritySilencedAlarm NOTIFY updated)
  Q_PROPERTY(int numActiveAlarms READ GetNumActiveAlarms NOTIFY updated)
  Q_PROPERTY(int numSilencedAlarms READ GetNumSilencedAlarms NOTIFY updated)

  LatchingAlarm *GetHighestPriorityActiveAlarm() {
    LatchingAlarm *res = nullptr;
    for (auto *alarm : alarms_) {
      if (res == nullptr || alarm->GetEffectiveAudioPriority() >
                                res->GetEffectiveAudioPriority()) {
        res = alarm;
      }
    }
    return res;
  }

  LatchingAlarm *GetHighestPrioritySilencedAlarm() {
    LatchingAlarm *res = nullptr;
    for (auto *alarm : alarms_) {
      if (!alarm->GetSilencedUntil().has_value())
        continue;
      if (res == nullptr ||
          alarm->GetNominalPriority() > res->GetNominalPriority()) {
        res = alarm;
      }
    }
    return res;
  }

  int GetNumActiveAlarms() const {
    int res = 0;
    for (auto *alarm : alarms_) {
      if (alarm->IsAudioActive())
        ++res;
    }
    return res;
  }

  int GetNumSilencedAlarms() const {
    int res = 0;
    for (auto *alarm : alarms_) {
      if (alarm->GetSilencedUntil().has_value())
        ++res;
    }
    return res;
  }

  Q_INVOKABLE void acknowledgeHighestPriorityActiveAlarm() {
    GetHighestPriorityActiveAlarm()->Acknowledge(SteadyClock::now());
  }

  Q_PROPERTY(
      PipExceededAlarm *pipExceededAlarm READ get_pip_exceeded_alarm CONSTANT)
  Q_PROPERTY(PipNotReachedAlarm *pipNotReachedAlarm READ
                 get_pip_not_reached_alarm CONSTANT)

  PipExceededAlarm *get_pip_exceeded_alarm() { return &pip_exceeded_alarm_; }
  PipNotReachedAlarm *get_pip_not_reached_alarm() {
    return &pip_not_reached_alarm_;
  }

signals:
  void updated();

private:
  PipExceededAlarm pip_exceeded_alarm_;
  PipNotReachedAlarm pip_not_reached_alarm_;
  std::vector<LatchingAlarm *> alarms_{&pip_exceeded_alarm_,
                                       &pip_not_reached_alarm_};
};

#endif // ALARM_MANAGER_H_
