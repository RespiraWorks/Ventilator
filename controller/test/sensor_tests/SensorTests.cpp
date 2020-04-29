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
 * MPXV5004 series sensors. Caller must ensure the output buffer has the
 * correct minimum length.
 *
 * @param *pressureIn pointer to input pressure waveform to process
 * @param *voltageOut pointer to the output voltage buffer
 * @param count is the length of the input buffer
 */
static void MPXV5004_TransferFn(const float *pressureIn, float *voltageOut,
                                int count) {
  assert(pressureIn != nullptr && voltageOut != nullptr);
  for (int i = 0; i < count; i++) {
    voltageOut[i] = 5 * (0.2f * pressureIn[i] + 0.2f);
  }
}

// Simple helper function that takes in a voltage and returns the
// equivalent ADC counts that represent it
static void test_setAnalogPinToVolts(AnalogPin pin, float volts) {
  Hal.test_setAnalogPin(pin, static_cast<int>(roundf(volts * COUNTS_PER_VOLT)));
}

TEST(SensorTests, FullScaleReading) {
  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges. Each
  // value is repeated twice, so that the test neatly corresponds to the
  // 2 default average sample counts that the sensor module defaults to.
  // Values are in kPa.
  float pressures[] = {0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.5f, 1.5f,  2.0f,
                       2.0f, 2.5f, 2.5f, 3.0f, 3.0f, 3.5f, 3.5f, 3.92f, 3.92f};
  const int NUM_ELEMENTS = sizeof(pressures) / sizeof(pressures[0]);

  // Convert these pressure waveforms into the voltage equivalents using the
  // appropriate sensor transfer functions
  float sensorVoltages[NUM_ELEMENTS]; //[V]
  MPXV5004_TransferFn(pressures, sensorVoltages, NUM_ELEMENTS);

  // Will pad the rest of the simulated analog signals with ambient pressure
  // readings (0 kPa) voltage equivalents
  float ambientPressure = 0;                    //[kPa]
  float sensorVoltage_0kPa = 0;                 //[V]
  MPXV5004_TransferFn(&ambientPressure, &sensorVoltage_0kPa, 1);

  // First set the simulated analog signals to an ambient 0 kPa corresponding
  // voltage during calibration
  test_setAnalogPinToVolts(AnalogPin::PATIENT_PRESSURE, sensorVoltage_0kPa);
  test_setAnalogPinToVolts(AnalogPin::INFLOW_PRESSURE_DIFF, sensorVoltage_0kPa);
  test_setAnalogPinToVolts(AnalogPin::OUTFLOW_PRESSURE_DIFF,
                           sensorVoltage_0kPa);

  sensors_init(); // the sensors are also calibrated

  // Now to compare the pressure readings the sensor module is calculating
  // versus what the original pressure waveform was
  for (int i = 0; i < NUM_ELEMENTS; i += 2) {
    SCOPED_TRACE("iteration " + std::to_string(i));
    test_setAnalogPinToVolts(AnalogPin::PATIENT_PRESSURE, sensorVoltages[i]);
    test_setAnalogPinToVolts(AnalogPin::INFLOW_PRESSURE_DIFF,
                             sensorVoltages[i]);
    test_setAnalogPinToVolts(AnalogPin::OUTFLOW_PRESSURE_DIFF,
                             sensorVoltages[i]);

    float pressurePatient = get_patient_pressure().kPa();
    EXPECT_NEAR(pressurePatient, pressures[i], COMPARISON_TOLERANCE);

    float inflow = get_volumetric_inflow().cubic_m_per_sec();
    float outflow = get_volumetric_outflow().cubic_m_per_sec();
    // Inhalation and exhalation should match because they are fed with the same
    // pressure waveform
    EXPECT_EQ(inflow, outflow);
    EXPECT_NEAR(inflow,
                pressure_delta_to_flow(kPa(pressures[i])).cubic_m_per_sec(),
                COMPARISON_TOLERANCE);
  }
}

// These tests expect Venturi Diamters of 14 and 5.5 mm.
// If the Default PressureSensors::DEFAULT_VENTURI_PORT_DIAM and
// DEFAULT_VENTURI_CHOKE_DIAM are changed from this, the tests will fail unless
// you update the expected values accordingly.
TEST(SensorTests, TestPositiveVolumetricFlowCalculation) {
  sensors_init();
  float volumFlow = pressure_delta_to_flow(kPa(1.0f)).cubic_m_per_sec();
  // 1 kPa differential pressure should result in 9.52e-4 [m^3/s] of Q
  EXPECT_NEAR(volumFlow, 9.52e-4f, COMPARISON_TOLERANCE_FLOW);
}

TEST(SensorTests, TestNegativeVolumetricFlowCalculation) {
  sensors_init();
  float volumFlow = pressure_delta_to_flow(kPa(-1.0f)).cubic_m_per_sec();
  // -1 kPa differential pressure should result in -9.52e-4 [m^3/s] of Q
  EXPECT_NEAR(volumFlow, -9.52e-4f, COMPARISON_TOLERANCE_FLOW);
}

TEST(SensorTests, TestZeroVolumetricFlowCalculation) {
  sensors_init();
  float volumFlow = pressure_delta_to_flow(kPa(0.0f)).cubic_m_per_sec();
  // 0 kPa differential pressure should result in 0 [m^3/s] of Q
  EXPECT_NEAR(volumFlow, 0.0f, COMPARISON_TOLERANCE_FLOW);
}

TEST(SensorTests, TestNearZeroVolumetricFlowCalculation) {
  sensors_init();
  float volumFlow = pressure_delta_to_flow(kPa(1.0e-7f)).cubic_m_per_sec();
  // 1e-7 kPa differential pressure should result in 3.07e-7 [m^3/s] of Q
  EXPECT_NEAR(volumFlow, 3.07e-7f, COMPARISON_TOLERANCE_FLOW);
}

TEST(SensorTests, TestLargeVolumetricFlowCalculation) {
  sensors_init();
  float volumFlow = pressure_delta_to_flow(kPa(100.0f)).cubic_m_per_sec();
  // 100 kPa differential pressure should result in 9.72e-3 [m^3/s] of Q
  EXPECT_NEAR(volumFlow, 9.72e-3f, COMPARISON_TOLERANCE_FLOW);
}

TEST(SensorTests, TestSmallVolumetricFlowCalculation) {
  sensors_init();
  float volumFlow = pressure_delta_to_flow(kPa(-100.0f)).cubic_m_per_sec();
  // -100 kPa differential pressure should result in -9.72e-3 [m^3/s] of Q
  EXPECT_NEAR(volumFlow, -9.72e-3f, COMPARISON_TOLERANCE_FLOW);
}
