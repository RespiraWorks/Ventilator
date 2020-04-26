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

#include "math.h"
#include "sensors.h"

//@TODO: Potential Caution: Density of air slightly varies over temperature and
// altitude - need mechanism to adjust based on delivery? Constant involving
// density of air. Density assumed at 15 deg. Celsius and 1 atm of pressure.
// Sourced from https://en.wikipedia.org/wiki/Density_of_air
static const float ROOT_OF_TWO_OVER_DENSITY_OF_AIR =
    sqrtf(2.0f / 1.225f); //[sqrt(meters^3/kg)]

// Arduino Nano ADC is 10 bit, default 5V Vref_P (~4.9 mV
// per count) [V];
static const float ADC_LSB = 5.0f / 1024.0f;

// Take this many samples from a sensor while zeroing it.
// TODO: Tune this value.
static constexpr int SENSOR_SAMPLES_FOR_INIT = 4;

// Take this many samples from a sensor while reading it.
// TODO: Tune this value.
static constexpr int SENSOR_SAMPLES_FOR_READ = 2;

namespace {
enum Sensor {
  PATIENT_PRESSURE,
  INFLOW_PRESSURE_DIFF,
  OUTFLOW_PRESSURE_DIFF,
};

// Keep this in sync with the Sensors enum!
constexpr int NUM_SENSORS = 3;

} // anonymous namespace

static float sensorZeroVals[NUM_SENSORS];

AnalogPin pin_for(Sensor s) {
  switch (s) {
  case PATIENT_PRESSURE:
    return AnalogPin::PATIENT_PRESSURE;
  case INFLOW_PRESSURE_DIFF:
    return AnalogPin::INFLOW_PRESSURE_DIFF;
  case OUTFLOW_PRESSURE_DIFF:
    return AnalogPin::OUTFLOW_PRESSURE_DIFF;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}

static float diameter_to_area(float diameter) {
  return static_cast<float>(M_PI) / 4.0f * diameter * diameter;
}

void sensors_init() {
  // We wait 20ms from power-on-reset for pressure sensors to warm up.
  //
  // TODO: Is 20ms the right amount of time?  We're basing it on the data sheet
  // for MPXV7002, https://www.nxp.com/docs/en/data-sheet/MPXV7002.pdf table 1,
  // last entry.  But we're not acutally using that pressure sensor, we're
  // using MPXV5004DP!  The 5004DP datasheet doesn't say anything about a
  // startup time.  20ms is probably fine, but we should verify.
  //
  // TODO: This is unsafe if/when the controller starts up while connected to a
  // patient!  Calibration is valid only if the physical system is quiescent,
  // but if a patient is attached (or if the blower was running just a few
  // milliseconds ago), obviously that's not true.
  //
  // It seems that we'll need to save calibration readings to non-volatile
  // memory and provide operators with a way to shut off the device's blowers,
  // open any necessary valves, and recalibrate.
  Hal.delay(20);

  auto set_zero_level = [](Sensor s) {
    int sum = 0;
    for (int i = 0; i < SENSOR_SAMPLES_FOR_INIT; i++) {
      sum += Hal.analogRead(pin_for(s));
    }
    sensorZeroVals[s] = static_cast<float>(sum) / SENSOR_SAMPLES_FOR_INIT;
  };
  set_zero_level(PATIENT_PRESSURE);
  set_zero_level(INFLOW_PRESSURE_DIFF);
  set_zero_level(OUTFLOW_PRESSURE_DIFF);
}

// Reads a sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
static float read_pressure_sensor_kpa(Sensor s) {
  int sum = 0;
  for (int i = 0; i < SENSOR_SAMPLES_FOR_READ; i++) {
    sum += Hal.analogRead(pin_for(s)) - sensorZeroVals[s];
  }
  // Sensitivity of all pressure sensors is 1 V/kPa; no division needed.
  return static_cast<float>(sum) / SENSOR_SAMPLES_FOR_READ * ADC_LSB;
}

float get_patient_pressure_kpa() {
  return read_pressure_sensor_kpa(PATIENT_PRESSURE);
}

float get_volumetric_inflow_m3ps() {
  return pressure_delta_to_volumetric_flow(
      read_pressure_sensor_kpa(INFLOW_PRESSURE_DIFF));
}

float get_volumetric_outflow_m3ps() {
  return pressure_delta_to_volumetric_flow(
      read_pressure_sensor_kpa(OUTFLOW_PRESSURE_DIFF));
}

float pressure_delta_to_volumetric_flow(float diffPressureInKiloPascals) {
  // TODO(jlebar): Make these constexpr once we have a C++ standard library
  // PortArea must be larger than the ChokeArea [meters^2]
  float venturiPortArea =
      diameter_to_area(PressureSensors::DEFAULT_VENTURI_PORT_DIAM);
  float venturiChokeArea =
      diameter_to_area(PressureSensors::DEFAULT_VENTURI_CHOKE_DIAM);
  //[meters^4]
  float venturiAreaProduct = venturiPortArea * venturiChokeArea;
  // Equivalent to 1/sqrt(A1^2 - A2^2) guaranteed never to have a negative
  // radicand [1/meters^2]
  float bernoulliAreaDivisor =
      1.0f / sqrtf(venturiPortArea * venturiPortArea -
                   venturiChokeArea * venturiChokeArea);

  float sgn = copysignf(1.0f, diffPressureInKiloPascals);
  return sgn * ROOT_OF_TWO_OVER_DENSITY_OF_AIR * venturiAreaProduct *
         bernoulliAreaDivisor * sqrtf(abs(diffPressureInKiloPascals) * 1000.0f);
}
