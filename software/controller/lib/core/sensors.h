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

#ifndef SENSORS_H
#define SENSORS_H

#include "hal.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "units.h"

struct SensorReadings {
  Pressure patient_pressure;
  // Pressure differences read at the inflow/outflow venturis.
  Pressure inflow_pressure_diff;
  Pressure outflow_pressure_diff;

  // fraction of inspired oxygen (fiO2)
  float fio2;

  // Inflow and outflow at the two venturis, calculated from inflow/outflow
  // pressure diff.
  //
  // These are raw values (i.e uncorrected). We account for high-frequency noise
  // by e.g. averaging many samples, but we don't account here for low-frequency
  // sensor zero-point drift - for sensor-rezero evaluation
  VolumetricFlow inflow_raw;
  VolumetricFlow outflow_raw;
  // These are corrected values, which autozero when the flow is low enough or
  // constant enough to allow a rezero : drift is taken into account when the
  // ventilator is Off and when the flow is near-constant (assume the dynamic
  // of the system is fast, compared to sensor drift)
  VolumetricFlow inflow;
  VolumetricFlow outflow;
};

// Provides a re-zeroing algorithm for the flow sensors, and correct for their
// estimated zero-point drift:
// Monitor the pressure measurements and check whether their evolution can be
// attributed to drift, in which case we can declare a new sensor zero.
//
// Mathematically:
// Periodically compute:
//   A = average of the last n measurements (goal is to remove the noise).
//   S = sum of measurement we know are not noise (any measurement outside
//     A (from previous period) +/- noise)
// If S. is 0 (no data point is *definitely* not noise) and compute a new
// sensor zero, using either one of these logics:
//   - A is close enough to 0 to be attributed to drift over time since the
//     last successful re-zero ==> bring sensor zero to average
//   - A is close enough to "A from previous period" to be attributed to drift
//     over 1 period ==> bring sensor zero to previous zero + change in average
//
// Some known limits of this algorithm:
// - This makes us blind to minute leaks (while we are only near-sighted in
//   this regard when not using this)
// - Similarly, any leak that starts out small and slowly increases over time
//   will be harder to detect (but it can be detected by making sure the sensor
//   zero does not get too high - something we are not doing for now)
// - More importantly, this makes us blind to minute changes in flow that can
//   occur when ventilating a patient. My assumption right now is that this is
//   not happening on "normal" breathing (even for an ARDS patient) but this
//   needs confirmation.
class FlowSensorRezero {
public:
  FlowSensorRezero();
  // ZeroOffset needs to be called every loop iteration to update the average dp
  // and summation states, even if there is no need or we don't meet the
  // conditions to rezero the sensor.
  // Output is a "voltage delta" that needs to be incorporated into the sensor
  // zero value. This new zero should also lead to a new computation of sensor
  // output in order for the new value to take this new zero into account.
  Voltage ZeroOffset(Pressure dp);

private:
  // cycles counter to keep track of passing time
  int cycles_ = 0;

  // States to know how the average dp changed.
  Pressure average_dp_ = kPa(0);
  Pressure last_average_dp_ = kPa(0);

  // States to integrate deviation from average and evaluate whether it is close
  // enough to zero to allow re-zeroing the corresponding sensor
  Pressure error_sum_ = kPa(0);

  // States for deadband definition (deadband is last_average_dp_ +/- noise).
  // Init at noise around 0.
  Pressure min_dp_ = cmH2O(-0.013f);
  Pressure max_dp_ = cmH2O(0.013f);

  // Duration since last rezero, used to compute max sensor average that can be
  // attributed to drift, as allowable_drift * duration, capped to an absolute
  // maximum
  Duration duration_since_last_rezero_ = seconds(0);
};

// Provides calibrated sensor readings.
class Sensors {
public:
  Sensors();

  // Perform some initial sensor calibration.  This function should
  // be called on system startup before any other sensor functions
  // are called.
  void Calibrate();

  // Read the sensors.
  SensorReadings GetReadings();

  // min/max possible reading from MPXV5004GP pressure sensors
  // The canonical list of hardware in the device is: https://bit.ly/3aERr69
  constexpr static Pressure kMinPressure{kPa(0.0f)};
  constexpr static Pressure kMaxPressure{kPa(3.92f)};

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
  static VolumetricFlow PressureDeltaToFlow(Pressure delta);

private:
  enum class Sensor {
    kPatientPressure,
    kInflowPressureDiff,
    kOutflowPressureDiff,
    kFIO2,
  };
  // Keep this in sync with the Sensor enum!
  constexpr static int kNumSensors{4};

  static AnalogPin PinFor(Sensor s);
  Pressure ReadPressureSensor(Sensor s) const;
  float ReadOxygenSensor(const Pressure p_ambient) const;

  // Calibrated average sensor values in a zero state.
  Voltage sensors_zero_vals_[kNumSensors];

  // States used to access rezero_algorithms
  FlowSensorRezero inhale_zero_;
  FlowSensorRezero exhale_zero_;

  // Used for Before/After comparison - remove this once we have enough
  // comparison data
  Pressure OldReadPressureSensor(Sensor s) const;
  Voltage initial_sensors_zero_vals_[kNumSensors];
};

#endif // SENSORS_H
