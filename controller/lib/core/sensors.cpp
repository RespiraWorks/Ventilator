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

 module contributors: verityRF, jlebar, lee-matthews, Edwin Chiu
 The purpose of this module is to allow calibrated readings from the different
pressure sensors in the ventilator design. It is designed to be used with the
Arduino Nano and the MPXV5004GP and MPXV7002DP pressure sensors.
*/

#include "hal.h"

#include "blower.h"
#include "math.h"
#include "sensors.h"

//@TODO: Potential Caution: Density of air slightly varies over temperature and
// altitude - need mechanism to adjust based on delivery? Constant involving
// density of air. Density assumed at 15 deg. Celsius and 1 atm of pressure.
static const float ROOT_OF_TWO_OVER_DENSITY_OF_AIR =
    sqrtf(2.0f / 1.225f); //[sqrt(meters^3/kg)]

// Arduino Nano ADC is 10 bit, default 5V Vref_P (~4.9 mV
// per count) [V];
static const float ADC_LSB = 5.0f / 1024.0f;

// zero calibration values for the different pressure sensors.
static int sensorZeroVals[static_cast<int>(AnalogPinId::COUNT)] = {0};

// number of samples to perform averaging over during sensor zeroization
static int zeroingAvgSize = 4;
// number of samples to perform averaging over during calibrated sensor reads
static int sensorAvgSize = 2;

// PortArea must be larger than the ChokeArea [meters^2]
static float venturiPortArea = PressureSensors::DEFAULT_VENTURI_PORT_DIAM;
static float venturiChokeArea = PressureSensors::DEFAULT_VENTURI_CHOKE_DIAM;
//[meters^4]
static float venturiAreaProduct = venturiPortArea * venturiChokeArea;
// Equivalent to 1/sqrt(A1^2 - A2^2) guaranteed never to have a negative
// radicand [1/meters^2]
static float bernoulliAreaDivisor =
    1.0f / sqrtf(static_cast<float>(pow(venturiPortArea, 2)) -
                 static_cast<float>(pow(venturiChokeArea, 2)));

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

static float diameterOfCircleToArea(float diameter) {
  return static_cast<float>(M_PI *
                            (static_cast<float>(pow(diameter, 2)) / 4.0f));
}

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
static int get_raw_sensor_zero_reading(AnalogPinId pinId) {
  int runningSum = 0;
  for (int i = 0; i < zeroingAvgSize; i++) {
    runningSum += Hal.analogRead(pinId);
  }
  // disregarding remainder because that's in the noise floor anyway
  return runningSum / zeroingAvgSize;
}

void zero_sensors() {
  blower_disable();
  Hal.delay(100); // some arbitrary time to wait for the pressure of the system
                  // to equalize at all points
  sensorZeroVals[static_cast<int>(PressureSensors::PATIENT_PIN)] =
      get_raw_sensor_zero_reading(PressureSensors::PATIENT_PIN);
  sensorZeroVals[static_cast<int>(PressureSensors::INHALATION_PIN)] =
      get_raw_sensor_zero_reading(PressureSensors::INHALATION_PIN);
  sensorZeroVals[static_cast<int>(PressureSensors::EXHALATION_PIN)] =
      get_raw_sensor_zero_reading(PressureSensors::EXHALATION_PIN);
}

void sensors_init(float venturiPortDiameter, float venturiChokeDiameter) {
  if ((venturiPortDiameter > venturiChokeDiameter) &&
      venturiChokeDiameter > 0.0f) {
    venturiPortArea = diameterOfCircleToArea(venturiPortDiameter);
    venturiChokeArea = diameterOfCircleToArea(venturiChokeDiameter);
  } else {
    //@TODO should we indicate to caller that defaults were used since
    // parameters were invalid?
  }
  // Update coefficients used to be able to convert differential pressure to
  // volumetric flow
  venturiAreaProduct = venturiPortArea * venturiChokeArea;
  bernoulliAreaDivisor =
      1.0f / sqrtf(static_cast<float>(pow(venturiPortArea, 2)) -
                   static_cast<float>(pow(venturiChokeArea, 2)));
  // must wait at least 20 ms from Power-On-Reset for pressure sensors to
  // warm-up
  Hal.delay(20);
  zero_sensors();
}

//@TODO: Add alarms if sensor value is out of expected range?
float get_pressure_reading(AnalogPinId pinId) {
  int runningSum = 0;
  for (int i = 0; i < sensorAvgSize; i++) {
    runningSum +=
        (Hal.analogRead(pinId) - sensorZeroVals[static_cast<int>(pinId)]);
  }
  // disregarding remainder because that's in the noise floor anyway
  runningSum /= sensorAvgSize;
  // Sensitivity of all pressure sensors is 1 V/kPa; no division needed.
  return ((float)runningSum * ADC_LSB);
}

float convert_diff_pressure_to_volumetric_flow(
    float diffPressureInKiloPascals) {
  if (diffPressureInKiloPascals > 0.0f) {
    return ROOT_OF_TWO_OVER_DENSITY_OF_AIR * venturiAreaProduct *
           bernoulliAreaDivisor * sqrtf(diffPressureInKiloPascals * 1000.0f);
  } else {
    return -1.0f * ROOT_OF_TWO_OVER_DENSITY_OF_AIR * venturiAreaProduct *
           bernoulliAreaDivisor *
           sqrtf(-1.0f * diffPressureInKiloPascals * 1000.0f);
  }
}
