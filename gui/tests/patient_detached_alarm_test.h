#ifndef PATIENT_DETACHED_TEST_H_
#define PATIENT_DETACHED_TEST_H_

#include "breath_signals.h"
#include "chrono.h"
#include "network_protocol.pb.h"
#include "patient_detached_alarm.h"

#include <QCoreApplication>
#include <QString>
#include <QtTest>

class PatientDetachedAlarmTest : public QObject {
  Q_OBJECT
public:
  PatientDetachedAlarmTest() = default;
  ~PatientDetachedAlarmTest() = default;

private slots:
  void initTestCase() {}
  void cleanupTestCase() {}

  void testZeroFlowCorrection() {
    PatientDetachedAlarm alarm;
    alarm.Update(t(0), flow_correction(0), BreathSignals{});
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
  }

  void testSmallPositiveFlowCorrection() {
    PatientDetachedAlarm alarm;
    alarm.Update(t(0), flow_correction(100), BreathSignals{});
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
  }

  void testSmallNegativeFlowCorrection() {
    PatientDetachedAlarm alarm;
    alarm.Update(t(0), flow_correction(100), BreathSignals{});
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
  }

  void testLargePositiveFlowCorrection() {
    PatientDetachedAlarm alarm;
    // Large positive flow correction is not currently an alarm, though it
    // could certainly indicate that the device is misbehaving.
    alarm.Update(t(0), flow_correction(1000), BreathSignals{});
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
  }

  void testLargeNegativeFlowCorrection() {
    PatientDetachedAlarm alarm;
    alarm.Update(t(0), flow_correction(-1000), BreathSignals{});
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
  }

private:
  SteadyInstant t(int seconds) const {
    return base_ + DurationMs(1000 * seconds);
  }
  ControllerStatus flow_correction(float ml_per_sec) const {
    ControllerStatus res;
    res.sensor_readings.flow_correction_ml_per_min = ml_per_sec * 60;
    return res;
  }

  SteadyInstant base_ = SteadyClock::now();
};

#endif // PATIENT_DETACHED_TEST_H_
