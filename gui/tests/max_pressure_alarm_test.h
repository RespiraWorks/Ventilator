#ifndef MAX_PRESSURE_ALARM_TEST_H_
#define MAX_PRESSURE_ALARM_TEST_H_

#include "chrono.h"
#include "max_pressure_alarm.h"
#include "network_protocol.pb.h"

#include <QCoreApplication>
#include <QtTest>

class MaxPressureAlarmTest : public QObject {
  Q_OBJECT
public:
  MaxPressureAlarmTest() = default;
  ~MaxPressureAlarmTest() = default;

private slots:
  void initTestCase() {}
  void cleanupTestCase() {}

  void testCommonCase() {
    int i = 0;
    MaxPressureAlarm alarm;

    QCOMPARE(60, alarm.GetThresholdCmH2O());

    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Normal operation.
    alarm.Update(t(i++), pressure(25.0));
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition activates.
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Condition stays on.
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Acknowledge the alarm.
    alarm.Acknowledge(t(i++));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition stays on, but alarm is still silenced.
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Silencing timeout elapses, audio back on.
    alarm.Update(t(i += 120), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
  }

  void testConditionBlipWhileBeeping() {
    int i = 0;
    MaxPressureAlarm alarm;

    // Fire condition
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Condition goes down: visual goes down, audio stays.
    alarm.Update(t(i++), pressure(25.0));
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Condition goes back up: visual goes back up, audio stays.
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
  }

  void testConditionBlipWhileSilenced() {
    int i = 0;
    MaxPressureAlarm alarm;

    QCOMPARE(60, alarm.GetThresholdCmH2O());

    // Fire condition
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Silence the alarm
    alarm.Acknowledge(t(i++));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition goes down while silenced: visual goes down, and the blip
    // terminates the ACK.
    alarm.Update(t(i++), pressure(25.0));
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition goes back up: visual goes back up, audio beeps again because
    // it is no longer silenced.
    alarm.Update(t(i++), pressure(100.0));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
  }

private:
  SteadyInstant t(int seconds) const {
    return base_ + DurationMs(1000 * seconds);
  }
  ControllerStatus pressure(float p) const {
    ControllerStatus res;
    res.sensor_readings.patient_pressure_cm_h2o = p;
    return res;
  }

  SteadyInstant base_ = SteadyClock::now();
};

#endif // MAX_PRESSURE_ALARM_TEST_H_
