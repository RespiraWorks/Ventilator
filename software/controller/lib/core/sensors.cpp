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

static DebugFloat dbg_dp_inhale("dp_inhale", "Inhale diff pressure, cmH2O");
static DebugFloat dbg_dp_exhale("dp_exhale", "Exhale diff pressure, cmH2O");
static DebugFloat dbg_pressure("pressure", "Patient pressure, cmH2O");
static DebugFloat dbg_flow_inhale("flow_inhale", "Inhale flow rate, cc/sec");
static DebugFloat dbg_flow_exhale("flow_exhale", "Exhale flow rate, cc/sec");
static DebugFloat dbg_fio2("fio2", "Fraction of inspired oxygen, [0.0 - 1.0]");
// Flow correction happens as part of volume computation, in the Controller.
static DebugFloat dbg_flow_uncorrected("flow_uncorrected",
                                       "Uncorrected net flow rate, cc/sec");

//@TODO: Potential Caution: Density of air slightly varies over temperature and
// altitude - need mechanism to adjust based on delivery? Constant involving
// density of air. Density assumed at 15 deg. Celsius and 1 atm of pressure.
// Sourced from https://en.wikipedia.org/wiki/Density_of_air
static const float kDensityOfAirKgPerCubicMeter{1.225f}; // kg/m^3

// Diameters and correction coefficient relating to 3/4in Venturi, see
// https://bit.ly/2ARuReg.
//
// Correction factor of 0.97 is based on ISO recommendations for Reynolds of
// roughly 10^4 and machined (rather than cast) surfaces. Data fit is in good
// agreement based on comparison to Fleisch pneumotachograph; see
// https://github.com/RespiraWorks/Ventilator/pull/476
constexpr static Length kVenturiPortDiameter{millimeters(15.05f)};
constexpr static Length kVenturiChokeDiameter{millimeters(5.5f)};
constexpr static float kVenturiCorrection{0.97f};

static_assert(kVenturiPortDiameter > kVenturiChokeDiameter);
static_assert(kVenturiChokeDiameter > meters(0));

AnalogPin Sensors::PinFor(Sensor s) {
  switch (s) {
  case Sensor::kPatientPressure:
    return AnalogPin::kPatientPressure;
  case Sensor::kInflowPressureDiff:
    return AnalogPin::kInflowPressureDiff;
  case Sensor::kOutflowPressureDiff:
    return AnalogPin::kOutflowPressureDiff;
  case Sensor::kFIO2:
    return AnalogPin::kFIO2;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}

Sensors::Sensors() = default;

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

  for (Sensor s : {Sensor::kPatientPressure, Sensor::kInflowPressureDiff,
                   Sensor::kOutflowPressureDiff, Sensor::kFIO2}) {
    sensors_zero_vals_[static_cast<int>(s)] = Hal.analogRead(PinFor(s));
  }
}

// Reads a pressure sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
Pressure Sensors::ReadPressureSensor(Sensor s) const {
  // The pressure sensors output 1-5V, and each additional 1V of output
  // corresponds to an additional 1kPa of pressure difference.
  // https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
  //
  // The pressure sensor is scaled to 0-3.3V, which is the range captured by
  // our ADC.  Therefore, if we multiply the received voltage by 5/3.3, we get
  // a pressure in kPa.
  static const float kPressureSensorGain{5.f / 3.3f};
  return kPa(kPressureSensorGain * (Hal.analogRead(PinFor(s)) -
                                    sensors_zero_vals_[static_cast<int>(s)])
                                       .volts());
}

// Reads an oxygen sensor, returning the concentration of oxygen [0 ; 1.0]
//
// Output scales with partial pressure of O2, so ambient pressure must be
// compensated to get an accurate FIO2.
float Sensors::ReadOxygenSensor(const Pressure p_ambient) const {
  // Teledyne R24-compatible Electrochemical Cell Oxygen Sensor
  // http://www.medicalsolutiontechnology.com/wp-content/uploads/2012/09/GO-04-DATA-SHEET.pdf
  // Sensitivity of 0.060V/fio2, where fio2 is 0.0 to 1.0, at pressure = 1atm
  // https://www.apogeeinstruments.com/content/SO-100-200-spec-sheet.pdf:
  // sensitivity similar to SO-210-SS PCB has an op-amp to gain the output up by
  // 50V/V This gives about 3.0V full scale.

  // Standard air O2 concentration. This assumes that calibration occured with
  // pure air, meaning the system has been filled with air only.
  static const float kO2ConcentrationInAir{0.21f};

  static const float kAmplifierGain{50.0f};
  static const float kOxygenSensorGain{0.060f};

  // TODO: raise alarm if fio2 is out of expected (0,1) range
  return (Hal.analogRead(PinFor(Sensor::kFIO2)) -
          sensors_zero_vals_[static_cast<int>(Sensor::kFIO2)])
                 .volts() /
             (kAmplifierGain * kOxygenSensorGain) / p_ambient.atm() +
         kO2ConcentrationInAir;
}

VolumetricFlow Sensors::PressureDeltaToFlow(Pressure delta) {
  auto pow2 = [](float f) { return f * f; };

  // Returns an area in meters squared.
  auto diameter_to_area_m2 = [&](Length diameter) {
    return static_cast<float>(M_PI) / 4.0f * pow2(diameter.meters());
  };

  float portArea = diameter_to_area_m2(kVenturiPortDiameter);
  float chokeArea = diameter_to_area_m2(kVenturiChokeDiameter);
  return cubic_m_per_sec(
      kVenturiCorrection *
      std::copysign(std::sqrt(std::abs(delta.kPa()) * 1000.0f), delta.kPa()) *
      std::sqrt(2 / kDensityOfAirKgPerCubicMeter) * portArea * chokeArea /
      std::sqrt(pow2(portArea) - pow2(chokeArea)));
}

SensorReadings Sensors::GetReadings() const {
  auto patient_pressure = ReadPressureSensor(Sensor::kPatientPressure);
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  auto inflow_delta = ReadPressureSensor(Sensor::kInflowPressureDiff);
  auto outflow_delta = ReadPressureSensor(Sensor::kOutflowPressureDiff);
  // Fraction of Inspired Oxygen assuming ambient pressure of 101.3 kPa
  // TODO: measure ambient pressure from an additional sensor and/or estimate
  // from user input (from altitude?)
  Pressure p_ambient = kPa(101.3f);
  auto fio2 = ReadOxygenSensor(p_ambient);

  VolumetricFlow inflow = PressureDeltaToFlow(inflow_delta);
  VolumetricFlow outflow = PressureDeltaToFlow(outflow_delta);
  VolumetricFlow uncorrected_flow = inflow - outflow;

  // Set debug variables.
  dbg_dp_inhale.Set(inflow_delta.cmH2O());
  dbg_dp_exhale.Set(outflow_delta.cmH2O());
  dbg_pressure.Set(patient_pressure.cmH2O());
  dbg_fio2.Set(fio2);
  dbg_flow_inhale.Set(inflow.ml_per_sec());
  dbg_flow_exhale.Set(outflow.ml_per_sec());
  dbg_flow_uncorrected.Set(uncorrected_flow.ml_per_sec());

  return {
      .patient_pressure = patient_pressure,
      .inflow_pressure_diff = inflow_delta,
      .outflow_pressure_diff = outflow_delta,
      .fio2 = fio2,
      .inflow = inflow,
      .outflow = outflow,
  };
}
