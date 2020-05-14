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

#include "hal.h"
#include "sensors.h"
#include "gtest/gtest.h"
#include <assert.h>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

// Maximum allowable delta between calculated sensor readings and the input
// pressure waveform [kPa]
static const float COMPARISON_TOLERANCE = 0.005f;

// Maximum allowable delta between calculated and actual volumetric flow
static const float COMPARISON_TOLERANCE_FLOW = 5.0e-5f;

// ADC of Nano is 10 bit, 5V VREF_P [ADC Counts / V]
static const float COUNTS_PER_VOLT = 1024.0f / 5.0f;

//@TODO: Finish writing more specific unit tests for this module

/*
 * @brief This method models the pressure to voltage transfer function of the
 * MPXV5004 series sensors.
 */
static Voltage MPXV5004_PressureToVoltage(Pressure pressure) {
  return volts(5 * (0.2f * pressure.kPa() + 0.2f));
}

// Simple helper function that takes in a voltage and returns the
// equivalent ADC counts that represent it
static void test_setAnalogPinToVolts(AnalogPin pin, Voltage v) {
  Hal.test_setAnalogPin(pin,
                        static_cast<int>(roundf(v.volts() * COUNTS_PER_VOLT)));
}

TEST(SensorTests, FullScaleReading) {
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
  test_setAnalogPinToVolts(AnalogPin::PATIENT_PRESSURE, voltage_at_0kPa);
  test_setAnalogPinToVolts(AnalogPin::INFLOW_PRESSURE_DIFF, voltage_at_0kPa);
  test_setAnalogPinToVolts(AnalogPin::OUTFLOW_PRESSURE_DIFF, voltage_at_0kPa);

  Sensors sensors;

  // Now to compare the pressure readings the sensor module is calculating
  // versus what the original pressure waveform was
  for (auto p : pressures) {
    SCOPED_TRACE("Pressure " + std::to_string(p.kPa()));
    test_setAnalogPinToVolts(AnalogPin::PATIENT_PRESSURE,
                             MPXV5004_PressureToVoltage(p));
    test_setAnalogPinToVolts(AnalogPin::INFLOW_PRESSURE_DIFF,
                             MPXV5004_PressureToVoltage(kPa(3.0)));
    test_setAnalogPinToVolts(AnalogPin::OUTFLOW_PRESSURE_DIFF,
                             MPXV5004_PressureToVoltage(kPa(1.0)));

    auto readings = sensors.GetSensorReadings();
    float pressurePatient = cmH2O(readings.pressure_cm_h2o).kPa();
    EXPECT_NEAR(pressurePatient, p.kPa(), COMPARISON_TOLERANCE);

    // Inhalation and exhalation should match because they are fed with the same
    // pressure waveform
    auto expected_flow = Sensors::PressureDeltaToFlow(kPa(3.0)) -
                         Sensors::PressureDeltaToFlow(kPa(1.0));
    EXPECT_NEAR(expected_flow.cubic_m_per_sec(),
                ml_per_min(readings.flow_ml_per_min).cubic_m_per_sec(),
                COMPARISON_TOLERANCE_FLOW);
  }
}

// These tests expect Venturi Diamters of 14 and 5.5 mm.
// If the Default PressureSensors::DEFAULT_VENTURI_PORT_DIAM and
// DEFAULT_VENTURI_CHOKE_DIAM are changed from this, the tests will fail unless
// you update the expected values accordingly.
TEST(SensorTests, TestPositiveVolumetricFlowCalculation) {
  EXPECT_NEAR(Sensors::PressureDeltaToFlow(kPa(1.0f)).cubic_m_per_sec(),
              9.52e-4f, COMPARISON_TOLERANCE_FLOW);
  EXPECT_NEAR(Sensors::PressureDeltaToFlow(kPa(-1.0f)).cubic_m_per_sec(),
              -9.52e-4f, COMPARISON_TOLERANCE_FLOW);
  EXPECT_NEAR(Sensors::PressureDeltaToFlow(kPa(0.0f)).cubic_m_per_sec(), 0.0f,
              COMPARISON_TOLERANCE_FLOW);
  EXPECT_NEAR(Sensors::PressureDeltaToFlow(kPa(1.0e-7f)).cubic_m_per_sec(),
              3.07e-7f, COMPARISON_TOLERANCE_FLOW);
  EXPECT_NEAR(Sensors::PressureDeltaToFlow(kPa(100.0f)).cubic_m_per_sec(),
              9.72e-3f, COMPARISON_TOLERANCE_FLOW);
  EXPECT_NEAR(Sensors::PressureDeltaToFlow(kPa(-100.0f)).cubic_m_per_sec(),
              -9.72e-3f, COMPARISON_TOLERANCE_FLOW);
}
