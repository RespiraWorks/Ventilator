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

#include <QCoreApplication>
#include <QString>
#include <QtTest>

#include "breath_signals.h"
#include "chrono.h"
#include "latching_alarm.h"
#include "network_protocol.pb.h"

// A test alarm for the condition "pressure > 60".
class MaxPressureAlarm : public LatchingAlarm {
 public:
  MaxPressureAlarm() : LatchingAlarm(AlarmPriority::HIGH) {}

 private:
  std::optional<QString> IsActive(SteadyInstant now, const ControllerStatus &status,
                                  const BreathSignals &breath_signals) override {
    (void)now;
    (void)breath_signals;
    return (status.sensor_readings.patient_pressure_cm_h2o > 60.0)
               ? std::make_optional<QString>(
                     QString("active at %1").arg(status.sensor_readings.patient_pressure_cm_h2o))
               : std::nullopt;
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
    // Banner text appears and is available until the alarm is acknowledged AND
    // its occurrence ends - which doesn't happen in this test.
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Condition stays on.
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Acknowledge the alarm.
    alarm.Acknowledge(t(i++));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Condition stays on, but alarm is still silenced.
    alarm.Update(t(i++), pressure(95.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
    // Text should stay up to date with the data.
    QCOMPARE("active at 95", alarm.GetBannerText());

    // Silencing timeout elapses, audio back on.
    alarm.Update(t(i += 120), pressure(95.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    QCOMPARE("active at 95", alarm.GetBannerText());
  }

  void testConditionBlipWhileBeeping() {
    BreathSignals bs;
    int i = 0;
    MaxPressureAlarm alarm;

    // Fire condition
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Condition goes down: visual goes down, audio stays.
    alarm.Update(t(i++), pressure(25.0), bs);
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    // Text remains from the last time the condition was active
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Condition goes back up: visual goes back up, audio stays.
    alarm.Update(t(i++), pressure(95.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    QCOMPARE("active at 95", alarm.GetBannerText());
  }

  void testConditionBlipWhileSilenced() {
    BreathSignals bs;
    int i = 0;
    MaxPressureAlarm alarm;

    // Fire condition
    alarm.Update(t(i++), pressure(100.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Silence the alarm
    alarm.Acknowledge(t(i++));
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
    QCOMPARE("active at 100", alarm.GetBannerText());

    // Condition goes down while silenced: visual goes down, and the blip
    // terminates the ACK.
    alarm.Update(t(i++), pressure(25.0), bs);
    QVERIFY(!alarm.IsVisualActive());
    QVERIFY(!alarm.IsAudioActive());
    QCOMPARE("", alarm.GetBannerText());

    // Condition goes back up: visual goes back up, audio beeps again because
    // it is no longer silenced.
    alarm.Update(t(i++), pressure(95.0), bs);
    QVERIFY(alarm.IsVisualActive());
    QVERIFY(alarm.IsAudioActive());
    QCOMPARE("active at 95", alarm.GetBannerText());
  }

 private:
  SteadyInstant t(int seconds) const { return base_ + DurationMs(1000 * seconds); }
  ControllerStatus pressure(float p) const {
    ControllerStatus res;
    res.sensor_readings.patient_pressure_cm_h2o = p;
    return res;
  }

  SteadyInstant base_ = SteadyClock::now();
};
