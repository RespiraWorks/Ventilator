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

#include "sensors.h"
#include <cmath>

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

// Take this many samples from a sensor while zeroing it.
// TODO: Tune this value.
static constexpr int SENSOR_SAMPLES_FOR_INIT = 4;

// Take this many samples from a sensor while reading it.
// TODO: Tune this value.
static constexpr int SENSOR_SAMPLES_FOR_READ = 2;

/*static*/ AnalogPin Sensors::PinFor(Sensor s) {
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

Sensors::Sensors() {
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

  auto set_zero_level = [this](Sensor s) {
    float sum = 0;
    for (int i = 0; i < SENSOR_SAMPLES_FOR_INIT; i++) {
      sum += Hal.analogRead(PinFor(s)).volts();
    }
    // A bit of loss of mantissa here is okay.
    sensors_zero_vals_[s] = volts(sum / SENSOR_SAMPLES_FOR_INIT);
  };
  set_zero_level(PATIENT_PRESSURE);
  set_zero_level(INFLOW_PRESSURE_DIFF);
  set_zero_level(OUTFLOW_PRESSURE_DIFF);
}

// Reads a sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
Pressure Sensors::ReadPressureSensor(Sensor s) {
  float sum = 0;
  for (int i = 0; i < SENSOR_SAMPLES_FOR_READ; i++) {
    sum += Hal.analogRead(PinFor(s)).volts();
  }

  // The pressure sensors output 1-5V, and each additional 1V of output
  // corresponds to an additional 1kPa of pressure difference.
  // https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
  //
  // The pressure sensor is scaled to 0-3.3V, which is the range captured by
  // our ADC.  Therefore, if we multiply the received voltage by 5/3.3, we get
  // a pressure in kPa.
  static const float TRANSFER_FN_COEFF = 5.f / 3.3f;
  return kPa(
      TRANSFER_FN_COEFF *
      (volts(sum / SENSOR_SAMPLES_FOR_READ) - sensors_zero_vals_[s]).volts());
}

/*static*/ VolumetricFlow Sensors::PressureDeltaToFlow(Pressure delta) {
  auto pow2 = [](float f) { return f * f; };

  // Returns an area in meters squared.
  auto diameter_to_area_m2 = [&](Length diameter) {
    return static_cast<float>(M_PI) / 4.0f * pow2(diameter.meters());
  };

  float portArea = diameter_to_area_m2(DEFAULT_VENTURI_PORT_DIAM);
  float chokeArea = diameter_to_area_m2(DEFAULT_VENTURI_CHOKE_DIAM);
  return cubic_m_per_sec(
      std::copysign(std::sqrt(std::abs(delta.kPa()) * 1000.0f), delta.kPa()) *
      std::sqrt(2 / DENSITY_OF_AIR_KG_PER_CUBIC_METER) * portArea * chokeArea /
      std::sqrt(pow2(portArea) - pow2(chokeArea)));
}

void TVIntegrator::AddFlow(Time now, VolumetricFlow flow) {
  // TODO: This calculation should be much more sophisticated.  Some possible
  // improvements.
  //
  //  - Periodically re-zero the volume (e.g. what happens if the tubes are
  //    disconnected from the patient?)
  //
  //  - Measure time with better than millisecond granularity.
  if (!initialized_) {
    last_flow_ = flow;
    last_flow_measurement_time_ = now;
    initialized_ = true;
  } else if (Duration delta = now - last_flow_measurement_time_;
             delta >= VOLUME_INTEGRAL_INTERVAL) {
    volume_ = volume_ + ml(delta.minutes() *
                           (last_flow_.ml_per_min() + flow.ml_per_min()) / 2);
    last_flow_measurement_time_ = now;
  }
}

SensorReadings Sensors::GetSensorReadings() {
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  auto patient_pressure = ReadPressureSensor(PATIENT_PRESSURE);
  auto inflow_delta = ReadPressureSensor(INFLOW_PRESSURE_DIFF);
  auto outflow_delta = ReadPressureSensor(OUTFLOW_PRESSURE_DIFF);
  VolumetricFlow flow =
      PressureDeltaToFlow(inflow_delta) - PressureDeltaToFlow(outflow_delta);
  tv_integrator_.AddFlow(Hal.now(), flow);
  return {
      .patient_pressure_cm_h2o = patient_pressure.cmH2O(),
      .volume_ml = tv_integrator_.GetTV().ml(),
      .flow_ml_per_min = flow.ml_per_min(),
      .inflow_pressure_diff_cm_h2o = inflow_delta.cmH2O(),
      .outflow_pressure_diff_cm_h2o = outflow_delta.cmH2O(),
  };
}
