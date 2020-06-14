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
  void test() {
    MaxPressureAlarm alarm;
    auto base = SteadyClock::now();
    auto t = [=](int seconds) { return base + DurationMs(1000 * seconds); };
    auto pressure = [](float p) -> ControllerStatus {
      return {.sensor_readings = {.patient_pressure_cm_h2o = p}};
    };

    QCOMPARE(60, alarm.GetThresholdCmH2O());

    auto now = t(0);
    QVERIFY(!alarm.IsAudioActive(now));
    QVERIFY(!alarm.IsVisualActive(now));
    QVERIFY(!alarm.CanAcknowledge());
    QVERIFY(!alarm.CanReset());

    now = t(1);
    // Go above pressure threshold.
    alarm.Update(now, pressure(100));
    QVERIFY(alarm.IsAudioActive(now));
    QVERIFY(alarm.IsVisualActive(now));
    // Allowed to acknowledge - audio is active.
    QVERIFY(alarm.CanAcknowledge());
    // Not allowed to reset because condition is currently met.
    QVERIFY(!alarm.CanReset());

    now = t(2);
    // Go below threshold - but the signals are latching so they should stay
    // active.
    alarm.Update(now, pressure(30));
    QVERIFY(alarm.IsAudioActive(now));
    QVERIFY(alarm.IsVisualActive(now));
    QVERIFY(alarm.CanAcknowledge());
    // Allowed to reset now, as the condition is not currently met.
    QVERIFY(alarm.CanReset());

    // ACKNOWLEDGE - should suppress audio for 2 minutes.
    alarm.Acknowledge(now);
    QVERIFY(!alarm.IsAudioActive(now));
    QVERIFY(!alarm.IsAudioActive(now + DurationMs(120'000)));
    QVERIFY(alarm.IsAudioActive(now + DurationMs(120'001)));
    // Can't acknowledge any more because it's already acknowledged.
    QVERIFY(!alarm.CanAcknowledge());
    // Can still reset because the signal is still latched.
    QVERIFY(alarm.CanReset());

    now = t(3);
    // Stay under threshold - audio stays off
    alarm.Update(now, pressure(25));
    QVERIFY(!alarm.IsAudioActive(now));
    // Still can't acknowledge.
    QVERIFY(!alarm.CanAcknowledge());

    // Wait until the ACK expires, stay under threshold.
    // Audio should become active and acknowledgeable.
    now = t(125);
    alarm.Update(now, pressure(25));
    QVERIFY(alarm.IsAudioActive(now));
    QVERIFY(alarm.CanAcknowledge());

    // Reset the alarm - both signals go down.
    QVERIFY(alarm.CanReset());
    alarm.Reset(now);
    QVERIFY(!alarm.IsAudioActive(now));
    QVERIFY(!alarm.IsVisualActive(now));

    // Activate again.
    now = t(126);
    alarm.Update(now, pressure(100));
    now = t(127);
    alarm.Update(now, pressure(50));
    QVERIFY(alarm.IsAudioActive(now));
    QVERIFY(alarm.IsVisualActive(now));
    QVERIFY(alarm.CanAcknowledge());
    QVERIFY(alarm.CanReset());
  }
};

#endif // MAX_PRESSURE_ALARM_TEST_H_
