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

#include "sensors.h"
#include "hal.h"
#include "vars.h"
#include <cmath>

// Debug variables useful for tracing
static float dp_inhale, dp_exhale, pressure;
static DebugVar d1("dp_inhale", &dp_inhale, "Inhale diff pressure, cm/h2O",
                   "%.4f");
static DebugVar d2("dp_exhale", &dp_exhale, "Exhale diff pressure, cm/h2O",
                   "%.4f");
static DebugVar d3("pressure", &pressure, "Patient pressure, cm/h2O", "%.4f");

static float flow_inhale, flow_exhale, flow_delta, volume;
static DebugVar d4("flow_inhale", &flow_inhale, "Inhale flow rate, cc/sec",
                   "%.3f");
static DebugVar d5("flow_exhale", &flow_exhale, "Exhale flow rate, cc/sec",
                   "%.3f");
static DebugVar d6("flow_delta", &flow_delta,
                   "Inhale - Exhale flow rate, cc/sec", "%.3f");
static DebugVar d7("volume", &volume, "Tidal volume, ml", "%0.3f");

//@TODO: Potential Caution: Density of air slightly varies over temperature and
// altitude - need mechanism to adjust based on delivery? Constant involving
// density of air. Density assumed at 15 deg. Celsius and 1 atm of pressure.
// Sourced from https://en.wikipedia.org/wiki/Density_of_air
static const float DENSITY_OF_AIR_KG_PER_CUBIC_METER = 1.225f; // kg/m^3

// Diameters relating to Ethan's Alpha Venturi - II
// (https://docs.google.com/spreadsheets/d/1G9Kb-ImlluK8MOx-ce2rlHUBnTOtAFQvKjjs1bEhlpM/edit#gid=963553579)
// Port diameter must be larger than choke diameter
constexpr static Length DEFAULT_VENTURI_PORT_DIAM = millimeters(14.0);
constexpr static Length DEFAULT_VENTURI_CHOKE_DIAM = millimeters(5.5f);

static_assert(DEFAULT_VENTURI_PORT_DIAM > DEFAULT_VENTURI_CHOKE_DIAM);
static_assert(DEFAULT_VENTURI_CHOKE_DIAM > meters(0));

// TODO: VOLUME_INTEGRAL_INTERVAL was not chosen carefully.
static constexpr Duration VOLUME_INTEGRAL_INTERVAL = milliseconds(5);

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

Sensors::Sensors() {}

// NOTE - I can't do this in the constructor now because it gets called before
// the HAL is set up, so the busy wait never finishes.
void Sensors::Calibrate() {
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

  for (Sensor s :
       {PATIENT_PRESSURE, INFLOW_PRESSURE_DIFF, OUTFLOW_PRESSURE_DIFF}) {
    sensors_zero_vals_[s] = Hal.analogRead(PinFor(s));
  }
}

// Reads a sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
Pressure Sensors::ReadPressureSensor(Sensor s) {
  // The pressure sensors output 1-5V, and each additional 1V of output
  // corresponds to an additional 1kPa of pressure difference.
  // https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
  //
  // The pressure sensor is scaled to 0-3.3V, which is the range captured by
  // our ADC.  Therefore, if we multiply the received voltage by 5/3.3, we get
  // a pressure in kPa.
  static const float TRANSFER_FN_COEFF = 5.f / 3.3f;
  return kPa(TRANSFER_FN_COEFF *
             (Hal.analogRead(PinFor(s)) - sensors_zero_vals_[s]).volts());
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
  Duration delta = now - last_flow_measurement_time_;
  if (delta >= VOLUME_INTEGRAL_INTERVAL) {
    volume_ += delta * (last_flow_ + flow) / 2.0f;
    last_flow_measurement_time_ = now;
    last_flow_ = flow;
  }
}

SensorReadings Sensors::GetSensorReadings() {
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  auto patient_pressure = ReadPressureSensor(PATIENT_PRESSURE);
  auto inflow_delta = ReadPressureSensor(INFLOW_PRESSURE_DIFF);
  auto outflow_delta = ReadPressureSensor(OUTFLOW_PRESSURE_DIFF);

  dp_inhale = inflow_delta.cmH2O();
  dp_exhale = outflow_delta.cmH2O();
  pressure = patient_pressure.cmH2O();

  VolumetricFlow inflow = PressureDeltaToFlow(inflow_delta);
  VolumetricFlow outflow = PressureDeltaToFlow(outflow_delta);
  VolumetricFlow flow = inflow - outflow;
  tv_integrator_.AddFlow(Hal.now(), flow);

  flow_inhale = inflow.ml_per_sec();
  flow_exhale = outflow.ml_per_sec();
  flow_delta = flow.ml_per_sec();
  volume = tv_integrator_.GetTV().ml();

  return {
      .patient_pressure_cm_h2o = patient_pressure.cmH2O(),
      .volume_ml = tv_integrator_.GetTV().ml(),
      .flow_ml_per_min = flow.ml_per_min(),
      .inflow_pressure_diff_cm_h2o = inflow_delta.cmH2O(),
      .outflow_pressure_diff_cm_h2o = outflow_delta.cmH2O(),
  };
}
