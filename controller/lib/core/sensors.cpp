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
static const float DENSITY_OF_AIR_KG_PER_CUBIC_METER = 1.225f; // kg/m^3

// Diameters relating to Ethan's Alpha Venturi - II
// (https://docs.google.com/spreadsheets/d/1G9Kb-ImlluK8MOx-ce2rlHUBnTOtAFQvKjjs1bEhlpM/edit#gid=963553579)
// Port diameter must be larger than choke diameter
constexpr static Length DEFAULT_VENTURI_PORT_DIAM = millimeters(14);
constexpr static Length DEFAULT_VENTURI_CHOKE_DIAM = millimeters(5.5f);

static_assert(DEFAULT_VENTURI_PORT_DIAM > DEFAULT_VENTURI_CHOKE_DIAM);
static_assert(DEFAULT_VENTURI_CHOKE_DIAM > meters(0));

// TODO: VOLUME_INTEGRAL_INTERVAL was not chosen carefully.
static constexpr Duration VOLUME_INTEGRAL_INTERVAL = milliseconds(5);

// Arduino Nano ADC is 10 bit, default 5V Vref_P (~4.9 mV
// per count) [V];
//
// TODO: Update this for STM32, see https://bit.ly/3aERr69.
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

// Returns an area in meters squared.
static float diameter_to_area_m2(Length diameter) {
  return static_cast<float>(M_PI) / 4.0f * diameter.meters() *
         diameter.meters();
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
  Hal.delay(milliseconds(20));

  auto set_zero_level = [](Sensor s) {
    // Perform most of the computation exactly in "int", to avoid a situation
    // where sum + x == sum, in case sum is above the maximum exactly
    // representable integer in a float.
    int sum = 0;
    for (int i = 0; i < SENSOR_SAMPLES_FOR_INIT; i++) {
      sum += Hal.analogRead(pin_for(s));
    }
    // A bit of loss of mantissa here is okay.
    sensorZeroVals[s] = static_cast<float>(sum) / SENSOR_SAMPLES_FOR_INIT;
  };
  set_zero_level(PATIENT_PRESSURE);
  set_zero_level(INFLOW_PRESSURE_DIFF);
  set_zero_level(OUTFLOW_PRESSURE_DIFF);
}

// Reads a sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
static Pressure read_pressure_sensor(Sensor s) {
  int sum = 0;
  for (int i = 0; i < SENSOR_SAMPLES_FOR_READ; i++) {
    sum += Hal.analogRead(pin_for(s));
  }
  // Sensitivity of all pressure sensors is 1 V/kPa; no division needed.
  return kPa(ADC_LSB * (static_cast<float>(sum) / SENSOR_SAMPLES_FOR_READ -
                        sensorZeroVals[s]));
}

Pressure get_patient_pressure() {
  return read_pressure_sensor(PATIENT_PRESSURE);
}

VolumetricFlow get_volumetric_inflow() {
  return pressure_delta_to_flow(read_pressure_sensor(INFLOW_PRESSURE_DIFF));
}

VolumetricFlow get_volumetric_outflow() {
  return pressure_delta_to_flow(read_pressure_sensor(OUTFLOW_PRESSURE_DIFF));
}

/*
 * @brief Method implements Bernoulli's equation assuming the Venturi Effect.
 * https://en.wikipedia.org/wiki/Venturi_effect
 *
 * Solves for the volumetric flow rate since A1/A2, rho, and differential
 * pressure are known. Q = sqrt(2/rho) * (A1*A2) * 1/sqrt(A1^2-A2^2) *
 * sqrt(p1-p2); based on (p1 - p2) = (rho/2) * (v2^2 - v1^2); where A1 > A2
 *
 * @return the volumetric flow in [meters^3/s]. Can be negative, indicating
 * direction of flow, depending on how the differential sensor is attached to
 * the venturi.
 */
VolumetricFlow pressure_delta_to_flow(Pressure delta) {
  // TODO(jlebar): Make these constexpr once we have a C++ standard library
  // PortArea must be larger than the ChokeArea [meters^2]
  float venturiPortArea = diameter_to_area_m2(DEFAULT_VENTURI_PORT_DIAM);
  float venturiChokeArea = diameter_to_area_m2(DEFAULT_VENTURI_CHOKE_DIAM);
  //[meters^4]
  float venturiAreaProduct = venturiPortArea * venturiChokeArea;
  // Equivalent to 1/sqrt(A1^2 - A2^2) guaranteed never to have a negative
  // radicand [1/meters^2]
  float bernoulliAreaDivisor =
      1.0f / sqrtf(venturiPortArea * venturiPortArea -
                   venturiChokeArea * venturiChokeArea);

  float sgn = copysignf(1.0f, delta.kPa());
  return cubic_m_per_sec(sgn * sqrtf(2 / DENSITY_OF_AIR_KG_PER_CUBIC_METER) *
                         venturiAreaProduct * bernoulliAreaDivisor *
                         sqrtf(abs(delta.kPa()) * 1000.0f));
}

Volume integrate_flow(VolumetricFlow flow) {
  // volume integral computation data
  static Time last_flow_measurement_time = millisSinceStartup(0);
  static VolumetricFlow last_flow = cubic_m_per_sec(0);
  static Volume volume = ml(0);
  static bool integrator_initialized = 0;

  // TODO: This calculation should be much more sophisticated.  Some possible
  // improvements.
  //
  //  - Periodically re-zero the volume (e.g. what happens if the tubes are
  //    disconnected from the patient?)
  //
  //  - Measure time with better than millisecond granularity.
  Time now = Hal.now();
  if (integrator_initialized == 0) { // First time
    last_flow = flow;
    last_flow_measurement_time = now;
    integrator_initialized = 1;
  } else if (Duration delta = now - last_flow_measurement_time;
             delta >= VOLUME_INTEGRAL_INTERVAL) {
    volume = volume + ml(delta.minutes() *
                         (last_flow.ml_per_min() + flow.ml_per_min()) / 2);
    last_flow_measurement_time = now;
  }
  return volume;
}

SensorReadings get_sensor_readings() {
  SensorReadings readings;
  Pressure cur_pressure = get_patient_pressure();
  // Store sensor readings so they can eventually be sent to the GUI.
  // This pressure is just from the patient sensor, converted to the right
  // units.
  readings.pressure_cm_h2o = cur_pressure.cmH2O();

  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  VolumetricFlow flow = get_volumetric_inflow() - get_volumetric_outflow();
  readings.flow_ml_per_min = flow.ml_per_min();

  readings.volume_ml = integrate_flow(flow).ml();

  return readings;
}
