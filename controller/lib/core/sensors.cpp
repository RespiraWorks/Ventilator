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

 module contributors: verityRF, lee-matthews, Edwin Chiu
 The purpose of this module is to allow calibrated readings from the different
pressure sensors in the ventilator design. It is designed to be used with the
Arduino Nano and the MPXV5004GP and MPXV7002DP pressure sensors.
*/

#include "hal.h"

#include "blower.h"
#include "sensors.h"

// TODO(jkff): Move this to hal.h
#ifndef TEST_MODE
//[0]: patient pressure sensor pin; [1]: inhalation diff pressure sensor pin;
//[2]: exhalation diff pressure sensor pin
static const int SENSOR_PINS[] = {A0, A1, A2};
#else
//[0]: patient pressure sensor pin; [1]: inhalation diff pressure sensor pin;
//[2]: exhalation diff pressure sensor pin
static const int SENSOR_PINS[] = {0, 1, 2};
#endif

//@TODO: Replace floats with equivalent integer only operations to save program
// space/time

// min/max possible reading from MPXV5004GP [kPa]
static const float P_VAL_MIN = 0.0f;
static const float P_VAL_MAX = 3.92f;

// min/max possible reading from MPXV7002DP [kPa]
static const float DP_VAL_MIN = -2.0f;
static const float DP_VAL_MAX = 2.0f;

// Arduino Nano ADC is 10 bit, default 5V Vref_P (~4.9 mV
// per count) [V];
static const float ADC_LSB = 5.0f / 1024.0f;

// zero calibration values for [0]: patient pressure sensor;[1]:
// inhalation diff pressure sensor;[2]: exhalation diff pressure
// sensor; [ADC Counts]
static int sensorZeroVals[] = {0, 0, 0};

// number of samples to perform averaging over during sensor zeroization
static int zeroingAvgSize = 4;
// number of samples to perform averaging over during calibrated sensor reads
static int sensorAvgSize = 2;

void set_zero_avg_samples(int numAvgSamples) {
  if (numAvgSamples < 1 || numAvgSamples > 32) {
    return;
  }
  zeroingAvgSize = numAvgSamples;
}

int get_zero_avg_samples() { return zeroingAvgSize; }

void set_sensor_avg_samples(int numAvgSamples) {
  if (numAvgSamples < 1 || numAvgSamples > 32) {
    return;
  }
  sensorAvgSize = numAvgSamples;
}

int get_sensor_avg_samples() { return sensorAvgSize; }

/*
 * @brief This method gets the zero pressure readings from the specified sensor
 * and averages them according to zeroingAvgSize. Called from zero_sensors(),
 * not for external use as zero_sensors() ensures correct system configuration.
 *
 * @param pressureSensor the sensor from the pressureSensor enum that a reading
 * is desired from
 *
 * @return The averaged sensor zero reading in ADC counts for the specified
 * pressureSensor enum member
 */
static int get_raw_sensor_zero_reading(enum pressureSensors pressureSensor) {
  int runningSum = 0;
  for (int i = 0; i < zeroingAvgSize; i++) {
    runningSum += Hal.analogRead((int)pressureSensor);
  }
  // disregarding remainder because that's in the noise floor anyway
  return runningSum / zeroingAvgSize;
}

void zero_sensors() {
  blower_disable();
  Hal.delay(100); // some arbitrary time to wait for the pressure of the system
                  // to equalize at all points
  sensorZeroVals[(int)pressureSensors::PATIENT] =
      get_raw_sensor_zero_reading(pressureSensors::PATIENT);
  sensorZeroVals[(int)pressureSensors::INHALATION] =
      get_raw_sensor_zero_reading(pressureSensors::INHALATION);
  sensorZeroVals[(int)pressureSensors::EXHALATION] =
      get_raw_sensor_zero_reading(pressureSensors::EXHALATION);
}

void sensors_init() {
  // must wait at least 20 ms from Power-On-Reset for pressure sensors to
  // warm-up
  Hal.delay(20);
  zero_sensors();
}

//@TODO: Add alarms if sensor value is out of expected range?
float get_pressure_reading(enum pressureSensors pressureSensor) {
  int runningSum = 0;
  for (int i = 0; i < sensorAvgSize; i++) {
    runningSum += (Hal.analogRead((int)pressureSensor) -
                   sensorZeroVals[(int)pressureSensor]);
  }
  // disregarding remainder because that's in the noise floor anyway
  runningSum /= sensorAvgSize;
  // Sensitivity of all pressure sensors is 1 V/kPa; no division needed.
  return ((float)runningSum * ADC_LSB);
}
