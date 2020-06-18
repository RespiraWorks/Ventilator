#ifndef ALARM_MANAGER_H_
#define ALARM_MANAGER_H_

#include "chrono.h"
#include "latching_alarm.h"
#include "pip_exceeded_alarm.h"

#include <QDebug>
#include <QObject>
#include <QString>

class AlarmManager : public QObject {
  Q_OBJECT
public:
  AlarmManager() {
    QObject::connect(&pip_exceeded_alarm_, &LatchingAlarm::updated, this,
                     &AlarmManager::updated);
  }

  void Update(SteadyInstant now, const ControllerStatus &status,
              const BreathSignals &breath_signals) {
    pip_exceeded_alarm_.Update(now, status, breath_signals);
  }

  Q_PROPERTY(LatchingAlarm *highestPriorityAlarm READ GetHighestPriorityAlarm
                 NOTIFY updated)
  Q_PROPERTY(int numActiveAlarms READ GetNumActiveAlarms NOTIFY updated)

  LatchingAlarm *GetHighestPriorityAlarm() { return &pip_exceeded_alarm_; }

  int GetNumActiveAlarms() const {
    return pip_exceeded_alarm_.IsAudioActive() ? 1 : 0;
  }
  Q_INVOKABLE void acknowledgeHighestPriorityAlarm() {
    GetHighestPriorityAlarm()->Acknowledge(SteadyClock::now());
  }

  Q_PROPERTY(
      PipExceededAlarm *pipExceededAlarm READ get_pip_exceeded_alarm CONSTANT)

  PipExceededAlarm *get_pip_exceeded_alarm() { return &pip_exceeded_alarm_; }

signals:
  void updated();

private:
  PipExceededAlarm pip_exceeded_alarm_;
};

#endif // ALARM_MANAGER_H_
