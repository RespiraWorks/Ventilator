/* Copyright 2020, RespiraWorks

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

/*
 * module contributors: verityRF
 *
 * The purpose of this module is to provide unit testing for the sensors
 * controller module.  Module is to be run on an x86 host and is not to be run
 * on an Arduino platform.
 */

#include "flow_integrator.h"
#include "hal.h"
#include "sensors.h"
#include "gtest/gtest.h"
#include <assert.h>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

// Maximum allowable delta between calculated sensor readings and the input.
static const Pressure kComparisonTolerancePressure{kPa(0.005f)};
static const VolumetricFlow kComparisonToleranceFlow{ml_per_sec(50)};
static const float kComparisonToleranceFIO2{0.001f};

#define EXPECT_PRESSURE_NEAR(a, b)                                             \
  EXPECT_NEAR((a).kPa(), (b).kPa(), kComparisonTolerancePressure.kPa())

#define EXPECT_FLOW_NEAR(a, b)                                                 \
  EXPECT_NEAR((a).ml_per_sec(), (b).ml_per_sec(),                              \
              kComparisonToleranceFlow.ml_per_sec())

//@TODO: Finish writing more specific unit tests for this module

/*
 * @brief This method models the pressure to voltage transfer function of the
 * MPXV5004 series sensors.  The raw voltage coming out of the sensor would be
 * 5 * (...), but our PCB scales it down to 3.3f * (...) so that the pressure
 * range (0-4kPa) is in the voltage range 0-3.3V.
 */
static Voltage MPXV5004_PressureToVoltage(Pressure pressure) {
  return volts(3.3f * (0.2f * pressure.kPa() + 0.2f));
}

// This method models the oxygen sensor. Voltage reads partial pressure of O2
// and needs ambient pressure to modulate output voltage.
// Sensitivity is 0.061 V/pt at 1 atm and assumed to vary linearly with pressure
// Offset is assumed to be 10 mV
// Analog amplifier with gain 50 is used on the PCB
static Voltage FIO2ToVoltage(float fio2, Pressure p_amb) {
  return volts(fio2 * p_amb.atm() * 0.06f + 0.01f) * 50.0f;
}

// Function that helps change the readings by setting the pressure sensor pins,
// advancing time and then getting sensors readings
static SensorReadings update_readings(Duration dt, Pressure patient_pressure,
                                      Pressure inflow_pressure,
                                      Pressure outflow_pressure, Pressure p_amb,
                                      float fio2, Sensors *sensors) {
  Hal.test_setAnalogPin(AnalogPin::kPatientPressure,
                        MPXV5004_PressureToVoltage(patient_pressure));
  Hal.test_setAnalogPin(AnalogPin::kInflowPressureDiff,
                        MPXV5004_PressureToVoltage(inflow_pressure));
  Hal.test_setAnalogPin(AnalogPin::kOutflowPressureDiff,
                        MPXV5004_PressureToVoltage(outflow_pressure));
  Hal.test_setAnalogPin(AnalogPin::kFIO2, FIO2ToVoltage(fio2, p_amb));
  Hal.delay(dt);
  return sensors->GetReadings();
}

TEST(SensorTests, FullScalePressureReading) {
  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges.
  std::vector<Pressure> pressures = {kPa(0.0f), kPa(0.5f), kPa(1.0f),
                                     kPa(1.5f), kPa(2.0f), kPa(2.5f),
                                     kPa(3.0f), kPa(3.5f), kPa(3.92f)};

  // Will pad the rest of the simulated analog signals with ambient pressure
  // readings (0 kPa) voltage equivalents
  Voltage voltage_at_0kPa = MPXV5004_PressureToVoltage(kPa(0)); //[V]

  // First set the simulated analog signals to an ambient 0 kPa corresponding
  // voltage during calibration
  Hal.test_setAnalogPin(AnalogPin::kPatientPressure, voltage_at_0kPa);
  Hal.test_setAnalogPin(AnalogPin::kInflowPressureDiff, voltage_at_0kPa);
  Hal.test_setAnalogPin(AnalogPin::kOutflowPressureDiff, voltage_at_0kPa);
  // Set fio2 signal to allow calibration
  Hal.test_setAnalogPin(AnalogPin::kFIO2, FIO2ToVoltage(0.21f, atm(1.0f)));

  Sensors sensors;
  sensors.Calibrate();

  // Now to compare the pressure readings the sensor module is calculating
  // versus what the original pressure waveform was
  for (auto p : pressures) {
    SCOPED_TRACE("Pressure " + std::to_string(p.kPa()));
    Hal.test_setAnalogPin(AnalogPin::kPatientPressure,
                          MPXV5004_PressureToVoltage(p));
    EXPECT_PRESSURE_NEAR(sensors.GetReadings().patient_pressure, p);
  }
}

TEST(SensorTests, FiO2Reading) {
  // calibrate O2 sensor at 21% fio2, 1atm
  Hal.test_setAnalogPin(AnalogPin::kFIO2, FIO2ToVoltage(0.21f, atm(1.0f)));
  // Set the other sensors to reasonable values to allow calibration
  Voltage voltage_at_0kPa = MPXV5004_PressureToVoltage(kPa(0));
  Hal.test_setAnalogPin(AnalogPin::kPatientPressure, voltage_at_0kPa);
  Hal.test_setAnalogPin(AnalogPin::kInflowPressureDiff, voltage_at_0kPa);
  Hal.test_setAnalogPin(AnalogPin::kOutflowPressureDiff, voltage_at_0kPa);

  Sensors sensors;
  sensors.Calibrate();

  // fio2 profile
  std::vector<float> fio2_settings = {0.0,  0.1f,  0.25f, 0.33f, 0.4f,
                                      0.6f, 0.66f, 0.75f, 0.9f,  1.0f};
  // sweep all fio2 settings and 1 atm pressure
  for (auto fio2 : fio2_settings) {
    SCOPED_TRACE("fio2 " + std::to_string(fio2));
    Hal.test_setAnalogPin(AnalogPin::kFIO2, FIO2ToVoltage(fio2, atm(1.0f)));
    EXPECT_NEAR(sensors.GetReadings().fio2, fio2, kComparisonToleranceFIO2);
  }
  // TODO: check the effect of ambient pressure once the system has a way to
  // know
  // ambient pressure (either through a sensor or through a user input?)
}

// These tests expect Venturi Diamters of 15.05mm and 5.5mm with a "discharge
// coefficient" of 0.97.  If VENTURI_PORT_DIAM or VENTURI_CHOKE_DIAM change,
// this test will fail until you update it.
//
// Expected values from https://www.wolframalpha.com/input/?i=Venturi+flowmeter
TEST(SensorTests, TestVolumetricFlowCalculation) {
  EXPECT_FLOW_NEAR(Sensors::PressureDeltaToFlow(kPa(1.0f)), ml_per_sec(939.6f));
  EXPECT_FLOW_NEAR(Sensors::PressureDeltaToFlow(kPa(-1.0f)),
                   ml_per_sec(-939.6f));
  EXPECT_FLOW_NEAR(Sensors::PressureDeltaToFlow(kPa(0.0f)), ml_per_sec(0));
  EXPECT_FLOW_NEAR(Sensors::PressureDeltaToFlow(kPa(1.0e-7f)),
                   ml_per_sec(0.2971f));
  EXPECT_FLOW_NEAR(Sensors::PressureDeltaToFlow(kPa(100.0f)), ml_per_sec(9396));
  EXPECT_FLOW_NEAR(Sensors::PressureDeltaToFlow(kPa(-100.0f)),
                   ml_per_sec(-9396));
}

TEST(SensorTests, TotalFlowCalculation) {
  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges with
  // less samples than for FullScaleReadings.
  std::vector<Pressure> pressures = {kPa(0.0f), kPa(0.5f), kPa(1.0f), kPa(2.0f),
                                     kPa(3.5f)};

  // First set the simulated analog signals to an ambient 0 kPa corresponding
  // voltage during calibration
  Voltage voltage_at_0kPa = MPXV5004_PressureToVoltage(kPa(0)); //[V]
  Hal.test_setAnalogPin(AnalogPin::kPatientPressure, voltage_at_0kPa);
  Hal.test_setAnalogPin(AnalogPin::kInflowPressureDiff, voltage_at_0kPa);
  Hal.test_setAnalogPin(AnalogPin::kOutflowPressureDiff, voltage_at_0kPa);
  // Set fio2 signal to allow calibration
  Hal.test_setAnalogPin(AnalogPin::kFIO2, FIO2ToVoltage(0.21f, atm(1.0f)));

  Sensors sensors;
  sensors.Calibrate();

  for (auto p_in : pressures) {
    for (auto p_out : pressures) {
      auto readings =
          update_readings(/*dt=*/seconds(0.0f), /*patient_pressure=*/kPa(0.0f),
                          p_in, p_out, atm(1.0f), /*fio2=*/0.21f, &sensors);
      EXPECT_FLOW_NEAR(readings.inflow, Sensors::PressureDeltaToFlow(p_in));
      EXPECT_FLOW_NEAR(readings.outflow, Sensors::PressureDeltaToFlow(p_out));
    }
  }
}

static constexpr Time base = microsSinceStartup(10'000'000);
static constexpr Duration sample_period = milliseconds(10);
Time ticks(int num_ticks) { return base + num_ticks * sample_period; }

TEST(SensorTests, Calibration) {
  // First set the simulated analog signals to randomly chosen signals
  // corresponding to conditions during calibration
  Pressure init_pressure = kPa(0.23f);
  Hal.test_setAnalogPin(AnalogPin::kPatientPressure,
                        MPXV5004_PressureToVoltage(init_pressure));
  Pressure init_inflow_delta = kPa(0.15f);
  Hal.test_setAnalogPin(AnalogPin::kInflowPressureDiff,
                        MPXV5004_PressureToVoltage(init_inflow_delta));
  Pressure init_outflow_delta = kPa(-0.13f);
  Hal.test_setAnalogPin(AnalogPin::kOutflowPressureDiff,
                        MPXV5004_PressureToVoltage(init_outflow_delta));
  float init_fio2 = 0.15f;
  Hal.test_setAnalogPin(AnalogPin::kFIO2, FIO2ToVoltage(init_fio2, atm(1.0f)));

  Sensors sensors;
  sensors.Calibrate();

  // get the sensor readings for the init signals, expect 0 (except for fio2,
  // which assumes 0.21 during calibration instead of 0)
  SensorReadings readings = sensors.GetReadings();

  EXPECT_PRESSURE_NEAR(readings.patient_pressure, kPa(0.0f));
  EXPECT_FLOW_NEAR(readings.inflow, ml_per_sec(0.0f));
  EXPECT_FLOW_NEAR(readings.outflow, ml_per_sec(0.0f));
  EXPECT_NEAR(readings.fio2, 0.21f, kComparisonToleranceFIO2);

  // set measured signals to 0 and expect -1*init values (plus 0.21 for fio2)
  readings = update_readings(/*dt=*/seconds(0), /*patient_pressure=*/kPa(0),
                             /*inflow_pressure=*/kPa(0),
                             /*outflow_pressure=*/kPa(0),
                             /*ambient pressure=*/atm(1.0f),
                             /*fio2=*/0.0f, &sensors);

  EXPECT_PRESSURE_NEAR(readings.patient_pressure,
                       cmH2O(-1 * init_pressure.cmH2O()));
  EXPECT_FLOW_NEAR(readings.inflow,
                   -1 * Sensors::PressureDeltaToFlow(init_inflow_delta));
  EXPECT_FLOW_NEAR(readings.outflow,
                   -1 * Sensors::PressureDeltaToFlow(init_outflow_delta));
  EXPECT_NEAR(readings.fio2, 0.21f - init_fio2, kComparisonToleranceFIO2);

  // set measured signals to some random values + init values and expect init
  // value to be removed from the readings (once again, except for fio2, which
  // removes init value and add 0.21)
  readings = update_readings(
      /*dt=*/seconds(0), /*patient_pressure=*/kPa(-0.5f) + init_pressure,
      /*inflow_pressure=*/kPa(1.1f) + init_inflow_delta,
      /*outflow_pressure=*/kPa(0.01f) + init_outflow_delta,
      /*ambient_pressure=*/atm(1.0f),
      /*fio2=*/0.25f + init_fio2, &sensors);

  EXPECT_PRESSURE_NEAR(readings.patient_pressure, kPa(-0.5f));
  EXPECT_FLOW_NEAR(readings.inflow, Sensors::PressureDeltaToFlow(kPa(1.1f)));
  EXPECT_FLOW_NEAR(readings.outflow, Sensors::PressureDeltaToFlow(kPa(0.01f)));
  EXPECT_NEAR(readings.fio2, 0.25f + 0.21f, kComparisonToleranceFIO2);
}
