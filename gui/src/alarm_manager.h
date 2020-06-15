#ifndef ALARM_MANAGER_H_
#define ALARM_MANAGER_H_

#include <QDebug>
#include <QObject>
#include <QString>

class Alarm : public QObject {
  Q_OBJECT
  Q_PROPERTY(AlarmPriority priority READ GetPriority NOTIFY PriorityChanged)
  Q_PROPERTY(QString text READ GetText NOTIFY TextChanged)
  Q_PROPERTY(bool isValid READ GetIsValid NOTIFY IsValidChanged)

public:
  enum AlarmPriority { None, Low, Medium, High };
  Q_ENUM(AlarmPriority)
  explicit Alarm(QObject *parent = nullptr,
                 AlarmPriority priority = AlarmPriority::None)
      : QObject(parent), priority_(priority) {}
  AlarmPriority GetPriority() const { return priority_; }

  bool GetIsValid() const { return priority_ != AlarmPriority::None; }
  QString GetText() const { return text_; }
signals:
  void PriorityChanged();
  void TextChanged();
  void IsValidChanged();

private:
  AlarmPriority priority_ = AlarmPriority::None;
  QString text_ = "This is a test";
};

class AlarmManager : public QObject {
  Q_OBJECT
public:
  explicit AlarmManager(QObject *parent = nullptr) : QObject(parent) {}

  Q_PROPERTY(Alarm *highestPriorityAlarm READ GetHighestPriorityAlarm NOTIFY
                 HighestPriorityAlarmChanged)
  Q_PROPERTY(int alarmsCount READ GetAlarmsCount NOTIFY AlarmsCountChanged)

  Q_INVOKABLE Alarm *GetHighestPriorityAlarm() {
    return highest_priority_alarm_;
  }
  int GetAlarmsCount() const { return alarms_.size(); }

  Q_INVOKABLE void ackownledge() {
    highest_priority_alarm_ = new Alarm(nullptr, Alarm::None);
    emit HighestPriorityAlarmChanged();
  }

signals:
  void HighestPriorityAlarmChanged();
  void AlarmsCountChanged();

private:
  Alarm *highest_priority_alarm_ = new Alarm(nullptr, Alarm::Low);
  QVector<Alarm *> alarms_;
};

#endif // ALARM_MANAGER_H_
