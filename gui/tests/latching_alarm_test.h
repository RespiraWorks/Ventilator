#ifndef LATCHING_ALARM_TEST_H_
#define LATCHING_ALARM_TEST_H_

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

#include <QCoreApplication>
#include <QtTest>

// A test alarm for the condition "pressure > 60".
class MaxPressureAlarm : public LatchingAlarm {
private:
  bool IsActive(SteadyInstant now, const ControllerStatus &status,
                const BreathSignals &breath_signals) override {
    (void)now;
    return (status.sensor_readings.patient_pressure_cm_h2o > 60.0);
  }
};

class LatchingAlarmTest : public QObject {
  Q_OBJECT
public:
  LatchingAlarmTest() = default;
  ~LatchingAlarmTest() = default;

private slots:
  void initTestCase() {}
  void cleanupTestCase() {}

  void testCommonCase() {
    BreathSignals bs;
    int i = 0;
    MaxPressureAlarm alarm;

    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Normal operation.
    alarm.Update(t(i++), pressure(25.0), bs);
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition activates.
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Condition stays on.
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Acknowledge the alarm.
    alarm.Acknowledge(t(i++));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition stays on, but alarm is still silenced.
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Silencing timeout elapses, audio back on.
    alarm.Update(t(i += 120), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
  }

  void testConditionBlipWhileBeeping() {
    BreathSignals bs;
    int i = 0;
    MaxPressureAlarm alarm;

    // Fire condition
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Condition goes down: visual goes down, audio stays.
    alarm.Update(t(i++), pressure(25.0), bs);
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Condition goes back up: visual goes back up, audio stays.
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
  }

  void testConditionBlipWhileSilenced() {
    BreathSignals bs;
    int i = 0;
    MaxPressureAlarm alarm;

    // Fire condition
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());

    // Silence the alarm
    alarm.Acknowledge(t(i++));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition goes down while silenced: visual goes down, and the blip
    // terminates the ACK.
    alarm.Update(t(i++), pressure(25.0), bs);
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());

    // Condition goes back up: visual goes back up, audio beeps again because
    // it is no longer silenced.
    alarm.Update(t(i++), pressure(100.0), bs);
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

#endif // LATCHING_ALARM_TEST_H_
