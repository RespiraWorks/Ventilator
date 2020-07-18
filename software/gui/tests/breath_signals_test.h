#ifndef BREATH_SIGNALS_TEST_H_
#define BREATH_SIGNALS_TEST_H_

#include "breath_signals.h"
#include "network_protocol.pb.h"

#include <QCoreApplication>
#include <QtTest>

class BreathSignalsTest : public QObject {
  Q_OBJECT
public:
  BreathSignalsTest() = default;
  ~BreathSignalsTest() = default;

private slots:
  void initTestCase() {}
  void cleanupTestCase() {}

  void testPipAndPeep() {
    SteadyInstant now = SteadyClock::now();
    auto pressure = [](uint64_t breath_id, float p) -> ControllerStatus {
      ControllerStatus res;
      res.sensor_readings.breath_id = breath_id;
      res.sensor_readings.patient_pressure_cm_h2o = p;
      return res;
    };

    BreathSignals b;
    QVERIFY(!b.pip().has_value());
    QVERIFY(!b.peep().has_value());

    b.Update(now, pressure(1, 5.0f));
    b.Update(now, pressure(1, 7.0f));
    b.Update(now, pressure(1, 6.0f));
    QCOMPARE(b.pip().value_or(0), 0);
    QCOMPARE(b.peep().value_or(0), 0);

    // New breath started - previous breath's values become available.
    b.Update(now, pressure(2, 4.0f));
    QCOMPARE(b.pip().value_or(0), 7.0);
    QCOMPARE(b.peep().value_or(0), 5.0);
    b.Update(now, pressure(2, 6.0f));
    b.Update(now, pressure(2, 5.0f));

    // Another breath started - previous breath's values become available.
    b.Update(now, pressure(3, 4.0f));
    QCOMPARE(b.pip().value_or(0), 6.0);
    QCOMPARE(b.peep().value_or(0), 4.0);
  }

  void testRr() {
    auto breath = [](uint64_t breath_id) -> ControllerStatus {
      return {.sensor_readings = {.breath_id = breath_id}};
    };
    SteadyInstant now = SteadyClock::now();
    auto ms = [=](int millis) { return now + DurationMs(millis); };

    BreathSignals b;
    QVERIFY(!b.rr().has_value());

    // Breath 1: 0-1000, 2: 1000-2000, 3: 2000-4000, 4: 4000-6000
    b.Update(ms(0), breath(1));
    b.Update(ms(100), breath(1));
    b.Update(ms(200), breath(1));
    QVERIFY(!b.rr().has_value());

    b.Update(ms(1000), breath(2));
    QVERIFY(!b.rr().has_value());

    // 3 breaths have started, enough to have signal.
    // The signal is 2 full breaths in 2 seconds = RR of 60.
    b.Update(ms(2000), breath(3));
    b.Update(ms(2500), breath(3));
    QCOMPARE(60.0, b.rr().value_or(0.0));

    // 4 breaths started (3 full breaths) in 4 seconds => RR = 45
    b.Update(ms(4000), breath(4));
    QCOMPARE(45.0, b.rr().value_or(0.0));

    // 5 breaths started (4 full breaths) in 6 seconds => RR = 40
    b.Update(ms(6000), breath(5));
    QCOMPARE(40.0, b.rr().value_or(0.0));

    // Earliest breath at 0 is evicted, next is at 1000
    // 5 breaths started (4 full breaths) in 7 seconds => RR = 48
    b.Update(ms(8000), breath(6));
    QCOMPARE(4 * 60.0f / 7, b.rr().value_or(0.0));

    // Earliest breath at 1000 is evicted, next is at 2000
    // 5 breaths started (4 full breaths) in 8 seconds => RR = 30
    b.Update(ms(10000), breath(7));
    QCOMPARE(30, b.rr().value_or(0.0));
  }
};

#endif // BREATH_SIGNALS_TEST_H_
