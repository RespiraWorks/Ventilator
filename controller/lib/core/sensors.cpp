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
// Flow correction happens as part of volume computation, in the Controller.
static DebugFloat dbg_flow_uncorrected("flow_uncorrected",
                                       "Uncorrected net flow rate, cc/sec");

//@TODO: Potential Caution: Density of air slightly varies over temperature and
// altitude - need mechanism to adjust based on delivery? Constant involving
// density of air. Density assumed at 15 deg. Celsius and 1 atm of pressure.
// Sourced from https://en.wikipedia.org/wiki/Density_of_air
static const float DENSITY_OF_AIR_KG_PER_CUBIC_METER = 1.225f; // kg/m^3

// Diameters and correction coefficient relating to 3/4in Venturi, see
// https://bit.ly/2ARuReg.
//
// Correction factor of 0.97 is based on ISO recommendations for Reynolds of
// roughly 10^4 and machined (rather than cast) surfaces. Data fit is in good
// agreement based on comparison to Fleisch pneumotachograph; see
// https://github.com/RespiraWorks/VentilatorSoftware/pull/476
constexpr static Length VENTURI_PORT_DIAM = millimeters(15.05f);
constexpr static Length VENTURI_CHOKE_DIAM = millimeters(5.5f);
constexpr static float VENTURI_CORRECTION = 0.97f;

static_assert(VENTURI_PORT_DIAM > VENTURI_CHOKE_DIAM);
static_assert(VENTURI_CHOKE_DIAM > meters(0));

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

// The pressure sensors output 1-5V, and each additional 1V of output
// corresponds to an additional 1kPa of pressure difference.
// https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
//
// The pressure sensor is scaled to 0-3.3V, which is the range captured by
// our ADC.  Therefore, if we multiply the received voltage by 5/3.3, we get
// a pressure in kPa.
static constexpr float PRESSURE_SENSOR_TRANSFER_FN_COEFF = 5.f / 3.3f;

Sensors::Sensors() = default;

FlowSensorRezero::FlowSensorRezero() {}

Voltage FlowSensorRezero::ZeroOffset(Pressure dp) {
  // TODO: refine value for these drift-suppression parameters
  static constexpr int rezero_sampling = 20;
  static constexpr Pressure max_drift = cmH2O(0.01f);
  static constexpr Pressure max_sum_outliers = cmH2O(0);
  // TODO: refine these sensor characterisation values with new venturi geometry
  static constexpr Pressure zero_flow_noise = cmH2O(0.012f);
  static constexpr float dp_signal_to_noise = 0.1f;
  // maximum value to allow rezeroing: if the measure is higher than this dp,
  // the noise model we are using fails and rezeroing is harder to perform
  static constexpr Pressure max_zeroing_dp = kPa(0.075f);

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
      // zero, maxed out at 1 second of max drift.
      Pressure max_zero =
          max_drift * std::min(1.0f, duration_since_last_rezero_.seconds());
      // note: negative average should always be attributed to drift: it does
      // not correspond to an actual physical phenomenon in the flow sensor
      if (average_dp_ < max_zero) {
        sensor_zero_delta =
            volts((average_dp_).kPa() / PRESSURE_SENSOR_TRANSFER_FN_COEFF);
        // In essence, this made it so average_dp_ = 0.
        average_dp_ = kPa(0);
        duration_since_last_rezero_ = seconds(0);
      } else {
        // compute local change for rezeroing with non-zero flow
        Pressure dp_change = kPa(0);
        if (average_dp_ < last_average_dp_) {
          dp_change = average_dp_ - last_average_dp_;
        } else {
          dp_change = last_average_dp_ - average_dp_;
        }
        if (dp_change < max_drift) {
          sensor_zero_delta = volts((average_dp_ - last_average_dp_).kPa() /
                                    PRESSURE_SENSOR_TRANSFER_FN_COEFF);
          // In essence, this made it so average_dp_ = last_average_dp_.
          average_dp_ = last_average_dp_;
          // In this case we don't update
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

  for (Sensor s :
       {PATIENT_PRESSURE, INFLOW_PRESSURE_DIFF, OUTFLOW_PRESSURE_DIFF}) {
    sensors_zero_vals_[s] = Hal.analogRead(PinFor(s));
    // Used for Before/After comparison - remove this once we have enough
    // comparison data
    initial_sensors_zero_vals_[s] = sensors_zero_vals_[s];
  }
}

// Reads a sensor, returning its value in kPa.
//
// @TODO: Add alarms if sensor value is out of expected range?
Pressure Sensors::ReadPressureSensor(Sensor s) {
  return kPa(PRESSURE_SENSOR_TRANSFER_FN_COEFF *
             (Hal.analogRead(PinFor(s)) - sensors_zero_vals_[s]).volts());
}

// Used for Before/After comparison - remove this once we have enough
// comparison data
Pressure Sensors::OldReadPressureSensor(Sensor s) {
  // The pressure sensors output 1-5V, and each additional 1V of output
  // corresponds to an additional 1kPa of pressure difference.
  // https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
  return kPa(
      PRESSURE_SENSOR_TRANSFER_FN_COEFF *
      (Hal.analogRead(PinFor(s)) - initial_sensors_zero_vals_[s]).volts());
}

/*static*/ VolumetricFlow Sensors::PressureDeltaToFlow(Pressure delta) {
  auto pow2 = [](float f) { return f * f; };

  // Returns an area in meters squared.
  auto diameter_to_area_m2 = [&](Length diameter) {
    return static_cast<float>(M_PI) / 4.0f * pow2(diameter.meters());
  };

  float portArea = diameter_to_area_m2(VENTURI_PORT_DIAM);
  float chokeArea = diameter_to_area_m2(VENTURI_CHOKE_DIAM);
  return cubic_m_per_sec(
      VENTURI_CORRECTION *
      std::copysign(std::sqrt(std::abs(delta.kPa()) * 1000.0f), delta.kPa()) *
      std::sqrt(2 / DENSITY_OF_AIR_KG_PER_CUBIC_METER) * portArea * chokeArea /
      std::sqrt(pow2(portArea) - pow2(chokeArea)));
}

SensorReadings Sensors::GetReadings() {
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  auto patient_pressure = ReadPressureSensor(PATIENT_PRESSURE);
  auto inflow_delta = ReadPressureSensor(INFLOW_PRESSURE_DIFF);
  auto outflow_delta = ReadPressureSensor(OUTFLOW_PRESSURE_DIFF);

  // used for sensor-rezero evaluation - to be removed once we have enough data
  auto inflow_delta_raw = OldReadPressureSensor(INFLOW_PRESSURE_DIFF);
  auto outflow_delta_raw = OldReadPressureSensor(OUTFLOW_PRESSURE_DIFF);
  VolumetricFlow inflow_raw = PressureDeltaToFlow(inflow_delta_raw);
  VolumetricFlow outflow_raw = PressureDeltaToFlow(outflow_delta_raw);

  // Update the flow sensors zeros if possible
  sensors_zero_vals_[INFLOW_PRESSURE_DIFF] +=
      inhale_zero_.ZeroOffset(inflow_delta);
  sensors_zero_vals_[OUTFLOW_PRESSURE_DIFF] +=
      exhale_zero_.ZeroOffset(outflow_delta);

  // reacquire deltas based on the new zero
  inflow_delta = ReadPressureSensor(INFLOW_PRESSURE_DIFF);
  outflow_delta = ReadPressureSensor(OUTFLOW_PRESSURE_DIFF);

  VolumetricFlow inflow = PressureDeltaToFlow(inflow_delta);
  VolumetricFlow outflow = PressureDeltaToFlow(outflow_delta);
  VolumetricFlow uncorrected_flow = inflow - outflow;

  // Set debug variables.
  dbg_dp_inhale.Set(inflow_delta.cmH2O());
  dbg_dp_exhale.Set(outflow_delta.cmH2O());
  dbg_pressure.Set(patient_pressure.cmH2O());
  dbg_dp_inhale_raw.Set(inflow_delta_raw.cmH2O());
  dbg_dp_exhale_raw.Set(outflow_delta_raw.cmH2O());
  dbg_raw_flow_inhale.Set(inflow_raw.ml_per_sec());
  dbg_raw_flow_exhale.Set(outflow_raw.ml_per_sec());
  dbg_flow_inhale.Set(inflow.ml_per_sec());
  dbg_flow_exhale.Set(outflow.ml_per_sec());
  dbg_flow_uncorrected.Set(uncorrected_flow.ml_per_sec());

  return {
      .patient_pressure = patient_pressure,
      .inflow_pressure_diff = inflow_delta,
      .outflow_pressure_diff = outflow_delta,
      .inflow_raw = inflow_raw,
      .outflow_raw = outflow_raw,
      .inflow = inflow,
      .outflow = outflow,
  };
}
