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

#include "flow_integrator.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "units.h"

// Provides calibrated sensor readings, including tidal volume (TV)
// integrated from flow.
class Sensors {
public:
  Sensors();

  // Perform some initial sensor calibration.  This function should
  // be called on system startup before any other sensor functions
  // are called.
  void Calibrate();

  // Called once for every breath cycle.  Sensors assume that tidal volume
  // should be 0 at the beginning of a breath and will calibrate to that
  // over time.
  //
  // TODO: This will not work as-is in pressure-assist mode.  TV is 0 at the
  // beginning of inspiratory effort by assumption, but by the time the
  // ventilator notices that an effort has occurred, TV is no longer 0 --
  // indeed, the way the ventilator notices that effort has occurred is by
  // detecting a flow over time, i.e. a change in volume!
  //
  // One way to fix this for pressure-assist mode is for us to keep track of
  // the measured TV at the start of the detected inspiratory effort.  Then we
  // can tell the Sensors module, TV was X mL Y milliseconds ago, but it should
  // have been 0.  That should be enough for us to calibrate to.
  void NoteNewBreath();

  // get the sensor readings (patient pressure, volumetric flow and tidal
  // volume) from the sensors
  SensorsProto GetSensorReadings();

  // min/max possible reading from MPXV5004GP pressure sensors
  // The canonical list of hardware in the device is: https://bit.ly/3aERr69
  inline constexpr static Pressure P_VAL_MIN = kPa(0.0f);
  inline constexpr static Pressure P_VAL_MAX = kPa(3.92f);

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
  enum Sensor {
    PATIENT_PRESSURE,
    INFLOW_PRESSURE_DIFF,
    OUTFLOW_PRESSURE_DIFF,
  };
  // Keep this in sync with the Sensor enum!
  inline constexpr static int NUM_SENSORS = 3;

  static AnalogPin PinFor(Sensor s);
  Pressure ReadPressureSensor(Sensor s);

  // Calibrated average sensor values in a zero state.
  Voltage sensors_zero_vals_[NUM_SENSORS];

  // We integrate flow to calculate tidal volume.  For debugging purposes, we
  // keep track of volume both with and without error correction.  (With error
  // correction we call NoteExpectedVolume, and without we don't.)  See
  // description of errors in FlowIntegrator.h.
  FlowIntegrator flow_integrator_;
  FlowIntegrator uncorrected_flow_integrator_;
};

#endif // SENSORS_H
