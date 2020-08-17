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
static DebugFloat dbg_dp_inhale_raw("dp_inhale_raw",
                                    "Inhale diff pressure - no rezero, cmH2O");
static DebugFloat dbg_dp_exhale_raw("dp_exhale_raw",
                                    "Exhale diff pressure - no rezero, cmH2O");
static DebugFloat dbg_raw_flow_inhale("flow_inhale_raw",
                                      "Inhale flow rate - no rezero, cc/sec");
static DebugFloat dbg_raw_flow_exhale("flow_exhale_raw",
                                      "Exhale flow rate - no rezero, cc/sec");
static DebugFloat dbg_flow_inhale("flow_inhale", "Inhale flow rate, cc/sec");
static DebugFloat dbg_flow_exhale("flow_exhale", "Exhale flow rate, cc/sec");
static DebugFloat dbg_fio2("fio2", "Fraction of inspired oxygen, [0.0 - 1.0]");
// Flow correction happens as part of volume computation, in the Controller.
static DebugFloat dbg_flow_uncorrected("flow_uncorrected",
                                       "Uncorrected net flow rate, cc/sec");
static DebugFloat dbg_inhale_offset("rezero_inhale",
                                    "Inhale re-zero value, mV");
static DebugFloat dbg_exhale_offset("rezero_exhale",
                                    "Exhale re-zero value, mV");

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

// The pressure sensors output 1-5V, and each additional 1V of output
// corresponds to an additional 1kPa of pressure difference.
// https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
//
// The pressure sensor is scaled to 0-3.3V, which is the range captured by
// our ADC.  Therefore, if we multiply the received voltage by 5/3.3, we get
// a pressure in kPa.
static const float kPressureSensorGain{5.f / 3.3f};

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

FlowSensorRezero::FlowSensorRezero() {}

Voltage FlowSensorRezero::ZeroOffset(Pressure dp) {
  // TODO: refine value for these drift-suppression parameters
  static constexpr int rezero_sampling = 10;
  static constexpr Pressure max_drift = cmH2O(0.01f);
  static constexpr Pressure max_sum_outliers = cmH2O(0);
  // TODO: refine these sensor characterisation values with new venturi geometry
  static constexpr Pressure zero_flow_noise = cmH2O(0.012f);
  static constexpr float dp_signal_to_noise = 0.1f;
  // maximum value to allow rezeroing: if the measure is higher than this dp,
  // the noise model we are using fails, rezeroing is harder to perform and may
  // lead to wrong re-zeroing.
  static constexpr Pressure max_zeroing_dp = kPa(0.75f);

  // Compute rolling averages and error integrals with deadband to eventually
  // re-zero the sensors:
  average_dp_ += dp / static_cast<float>(rezero_sampling);
  if ((dp < min_dp_) || (dp > max_dp_)) {
    // Sum absolute value in order to make sure one high outlier does not
    // compensate a low one that occured previously. This is used to check
    // whether some measures in the integration interval could be attributed to
    // actual signal rather than noise.
    if (dp > last_average_dp_) {
      error_sum_ += (dp - last_average_dp_);
    } else {
      error_sum_ += (last_average_dp_ - dp);
    }
  }

  Voltage sensor_zero_delta = volts(0);
  if (cycles_ % rezero_sampling == 0) {
    // TODO: define sample time outside this method
    duration_since_last_rezero_ += rezero_sampling * milliseconds(10.0f);
    // First rezeroing condition: no outlier in signal
    if (error_sum_ <= max_sum_outliers) {
      // Max pressure that can be attributed to drift when pressure is close to
      // zero, maxed out at 2 seconds of max drift.
      Pressure max_zero =
          max_drift * std::min(2.0f, duration_since_last_rezero_.seconds());
      // Note that negative average is always attributed to drift, as it does
      // not correspond to an actual physical phenomenon in the flow sensor
      if (average_dp_ < max_zero) {
        sensor_zero_delta = volts(average_dp_.kPa() / kPressureSensorGain);
        // In essence, this made it so average_dp_ = 0.
        average_dp_ = kPa(0);
        duration_since_last_rezero_ = seconds(0);
      } else {
        // compute local change for rezeroing with non-zero flow
        Pressure dp_change = kPa(0);
        if (average_dp_ > last_average_dp_) {
          dp_change = average_dp_ - last_average_dp_;
        } else {
          dp_change = last_average_dp_ - average_dp_;
        }
        if (dp_change < max_drift) {
          sensor_zero_delta = volts((average_dp_ - last_average_dp_).kPa() /
                                    kPressureSensorGain);
          // In essence, this made it so average_dp_ = last_average_dp_.
          average_dp_ = last_average_dp_;
          // In this case we don't update duration_since_last_rezero_ since
          // this is not going fully to zero, but making a non-zero
        }
      }
    }
    // compute new deadband
    Pressure dp_noise = kPa(fabsf(average_dp_.kPa())) / dp_signal_to_noise;
    if (dp_noise > zero_flow_noise) {
      dp_noise = zero_flow_noise;
    }
    // Don't allow noise if value is too high. In effect what this does is make
    // the integral pick up all the noise and become large, preventing any
    // rezeroing
    if (average_dp_ > max_zeroing_dp) {
      dp_noise = kPa(0);
    }
    min_dp_ = average_dp_ - dp_noise;
    max_dp_ = average_dp_ + dp_noise;

    last_average_dp_ = average_dp_;
    // reset summation states
    average_dp_ = kPa(0);
    error_sum_ = kPa(0);
  }
  cycles_ = (cycles_ + 1) % rezero_sampling;

  return sensor_zero_delta;
};

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
    // Used for Before/After comparison - remove this once we have enough
    // comparison data
    initial_sensors_zero_vals_[static_cast<int>(s)] =
        sensors_zero_vals_[static_cast<int>(s)];
  }
}

// Reads a pressure sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
Pressure Sensors::ReadPressureSensor(Sensor s) const {
  return kPa(kPressureSensorGain * (Hal.analogRead(PinFor(s)) -
                                    sensors_zero_vals_[static_cast<int>(s)])
                                       .volts());
}

// Used for Before/After comparison - remove this once we have enough
// comparison data
Pressure Sensors::OldReadPressureSensor(Sensor s) const {
  return kPa(kPressureSensorGain *
             (Hal.analogRead(PinFor(s)) -
              initial_sensors_zero_vals_[static_cast<int>(s)])
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

SensorReadings Sensors::GetReadings() {
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

  // used for sensor-rezero evaluation - to be removed once we have enough data
  auto inflow_delta_raw = OldReadPressureSensor(Sensor::kInflowPressureDiff);
  auto outflow_delta_raw = OldReadPressureSensor(Sensor::kOutflowPressureDiff);
  VolumetricFlow inflow_raw = PressureDeltaToFlow(inflow_delta_raw);
  VolumetricFlow outflow_raw = PressureDeltaToFlow(outflow_delta_raw);

  // Update the flow sensors zeros if possible
  Voltage inhale_offset = inhale_zero_.ZeroOffset(inflow_delta);
  Voltage exhale_offset = exhale_zero_.ZeroOffset(outflow_delta);
  sensors_zero_vals_[static_cast<int>(Sensor::kInflowPressureDiff)] +=
      inhale_offset;
  sensors_zero_vals_[static_cast<int>(Sensor::kOutflowPressureDiff)] +=
      exhale_offset;

  // reacquire deltas based on the new zero
  inflow_delta = ReadPressureSensor(Sensor::kInflowPressureDiff);
  outflow_delta = ReadPressureSensor(Sensor::kOutflowPressureDiff);

  VolumetricFlow inflow = PressureDeltaToFlow(inflow_delta);
  VolumetricFlow outflow = PressureDeltaToFlow(outflow_delta);
  VolumetricFlow uncorrected_flow = inflow - outflow;

  // Set debug variables.
  dbg_dp_inhale.Set(inflow_delta.cmH2O());
  dbg_dp_exhale.Set(outflow_delta.cmH2O());
  dbg_pressure.Set(patient_pressure.cmH2O());
  dbg_fio2.Set(fio2);
  dbg_dp_inhale_raw.Set(inflow_delta_raw.cmH2O());
  dbg_dp_exhale_raw.Set(outflow_delta_raw.cmH2O());
  dbg_raw_flow_inhale.Set(inflow_raw.ml_per_sec());
  dbg_raw_flow_exhale.Set(outflow_raw.ml_per_sec());
  dbg_flow_inhale.Set(inflow.ml_per_sec());
  dbg_flow_exhale.Set(outflow.ml_per_sec());
  dbg_flow_uncorrected.Set(uncorrected_flow.ml_per_sec());
  dbg_inhale_offset.Set(inhale_offset.volts() * 1000.0f);
  dbg_exhale_offset.Set(exhale_offset.volts() * 1000.0f);

  return {
      .patient_pressure = patient_pressure,
      .inflow_pressure_diff = inflow_delta,
      .outflow_pressure_diff = outflow_delta,
      .fio2 = fio2,
      .inflow_raw = inflow_raw,
      .outflow_raw = outflow_raw,
      .inflow = inflow,
      .outflow = outflow,
  };
}
