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
#include "units.h"

// A namespace class for constants related to pressure sensors.
class PressureSensors {
public:
  PressureSensors() = delete;

  // min/max possible reading from MPXV5004GP pressure sensors
  // The canonical list of hardware in the device is: https://bit.ly/3aERr69
  inline constexpr static Pressure P_VAL_MIN = kPa(0.0f);
  inline constexpr static Pressure P_VAL_MAX = kPa(3.92f);
};

void sensors_init();

// Gets gets the current pressure at the patient, in kPa.
Pressure get_patient_pressure();

// Gets the volumetric inflow (i.e. rate of air flowing into the patient).
VolumetricFlow get_volumetric_inflow();

// Gets the volumetric outflow (i.e. rate of air flowing out of the patient).
VolumetricFlow get_volumetric_outflow();

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
VolumetricFlow pressure_delta_to_flow(Pressure delta);

// get the sensor readings (patient pressure, volumetric flow and tidal
// volume) from the sensors
SensorReadings get_sensor_readings();

#endif // SENSORS_H
