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
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <string>

// Maximum allowable delta between calculated sensor readings and the input
// pressure waveform [kPa]
static const float COMPARISON_TOLERANCE = 0.005f;

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
static void MPXV5004_TransferFn(float *pressureIn, float *voltageOut,
                                int count) {
  assert(pressureIn != NULL && voltageOut != NULL);
  for (int i = 0; i < count; i++) {
    voltageOut[i] = 5 * (0.2f * pressureIn[i] + 0.2f);
  }
}

/*
 * @brief This method models the pressure to voltage transfer function of the
 * MPXV7002 series sensors. Caller must ensure the output buffer has the
 * correct minimum length.
 *
 * @param *pressureIn pointer to input pressure waveform to process
 * @param *voltageOut pointer to the output voltage buffer
 * @param count is the length of the input buffer
 */
static void MPXV7002_TransferFn(float *pressureIn, float *voltageOut,
                                int count) {
  assert(pressureIn != NULL && voltageOut != NULL);
  for (int i = 0; i < count; i++) {
    voltageOut[i] = 5 * (0.2f * pressureIn[i] + 0.5f);
  }
}

// Simple helper function that takes in a voltage and returns the
// equivalent ADC counts that represent it
static void test_setAnalogPinToVolts(AnalogPinId pin, float volts) {
  Hal.test_setAnalogPin(pin, static_cast<int>(roundf(volts * COUNTS_PER_VOLT)));
}

TEST(SensorTests, FullScaleReading) {
  // length of the differentialFlowPressures array
  const int NUM_DIFF_ELEMENTS = 20;
  // length of the patientPressures Array
  const int NUM_PATIENT_ELEMENTS = 18;
  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges. Each
  // value is repeated twice, so that the test neatly corresponds to the
  // 2 default average sample counts that the sensor module defaults to.
  // Values are in kPa.
  float differentialFlowPressures[NUM_DIFF_ELEMENTS] = {
      0.0f, 0.0f, -2.0f, -2.0f, -1.5f, -1.5f, -1.0f, -1.0f, -0.5f, -0.5f,
      0.0f, 0.0f, 0.5f,  0.5f,  1.0f,  1.0f,  1.5f,  1.5f,  2.0f,  2.0f};
  float patientPressures[NUM_PATIENT_ELEMENTS] = {
      0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.5f, 1.5f,  2.0f,
      2.0f, 2.5f, 2.5f, 3.0f, 3.0f, 3.5f, 3.5f, 3.92f, 3.92f};
  float differentialFlowSensorVoltages[NUM_DIFF_ELEMENTS]; //[V]
  float patientSensorVoltages[NUM_PATIENT_ELEMENTS];       //[V]
  // Convert these pressure waveforms into the voltage equivalents using the
  // appropriate sensor transfer functions
  MPXV7002_TransferFn(differentialFlowPressures, differentialFlowSensorVoltages,
                      NUM_DIFF_ELEMENTS);
  MPXV5004_TransferFn(patientPressures, patientSensorVoltages,
                      NUM_PATIENT_ELEMENTS);

  // Will pad the rest of the simulated analog signals with ambient pressure
  // readings (0 kPa) voltage equivalents
  float ambientPressure = 0;                    //[kPa]
  float differentialFlowSensorVoltage_0kPa = 0; //[V]
  float patientFlowSensorVoltage_0kPa = 0;      //[V]

  MPXV7002_TransferFn(&ambientPressure, &differentialFlowSensorVoltage_0kPa, 1);
  MPXV5004_TransferFn(&ambientPressure, &patientFlowSensorVoltage_0kPa, 1);

  // First set the simulated analog signals to an ambient 0 kPa corresponding
  // voltage during calibration
  test_setAnalogPinToVolts(PressureSensors::PATIENT_PIN,
                           patientFlowSensorVoltage_0kPa);
  test_setAnalogPinToVolts(PressureSensors::INHALATION_PIN,
                           differentialFlowSensorVoltage_0kPa);
  test_setAnalogPinToVolts(PressureSensors::EXHALATION_PIN,
                           differentialFlowSensorVoltage_0kPa);

  sensors_init(); // the sensors are also calibrated

  // Now to compare the pressure readings the sensor module is calculating
  // versus what the original pressure waveform was
  for (int i = 0; i < NUM_DIFF_ELEMENTS; i += 2) {
    test_setAnalogPinToVolts(PressureSensors::INHALATION_PIN,
                             differentialFlowSensorVoltages[i]);
    test_setAnalogPinToVolts(PressureSensors::EXHALATION_PIN,
                             differentialFlowSensorVoltages[i]);
    float pressureInhalation =
        get_pressure_reading(PressureSensors::INHALATION_PIN);
    float pressureExhalation =
        get_pressure_reading(PressureSensors::EXHALATION_PIN);
    // Inhalation and exhalation should match because they are fed with the same
    // pressure waveform
    EXPECT_EQ(pressureInhalation, pressureExhalation)
        << "Differential Sensor Calculated Inhale/Exhale at index " << i;
    // Calculate deviance from expected. Using only inhalation because we know
    // it is equal to exhalation by now.
    EXPECT_NEAR(pressureInhalation, differentialFlowPressures[i],
                COMPARISON_TOLERANCE)
        << "Differential Sensor Calculated Value at index " << i;
  }

  for (int i = 0; i < NUM_PATIENT_ELEMENTS; i += 2) {
    test_setAnalogPinToVolts(PressureSensors::PATIENT_PIN,
                             patientSensorVoltages[i]);
    float pressurePatient = get_pressure_reading(PressureSensors::PATIENT_PIN);
    EXPECT_NEAR(pressurePatient, patientPressures[i], COMPARISON_TOLERANCE)
        << "Patient Sensor at index" << i;
  }
}
