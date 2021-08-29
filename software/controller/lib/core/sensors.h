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
*/

#pragma once

#include "hal.h"
#include "units.h"
#include "vars.h"

// Keep this in sync with the Sensor enum below
constexpr static uint16_t NumSensors{5};

enum class Sensor {
  PatientPressure,
  AirInflowPressureDiff,
  OxygenInflowPressureDiff,
  OutflowPressureDiff,
  FIO2,
};

// Logical mappings: conceptual sensor -> ADC channel
AnalogPin PinFor(Sensor s);

struct SensorReadings {
  Pressure patient_pressure;

  // fraction of inspired oxygen (fiO2)
  float fio2;

  // Inflow and outflow at the two venturis, calculated from inflow/outflow
  // pressure diff.
  //
  // These are "uncorrected" values.  We account for high-frequency noise by
  // e.g. averaging many samples, but we don't account here for low-frequency
  // sensor zero-point drift.
  VolumetricFlow air_inflow;
  VolumetricFlow oxygen_inflow;
  VolumetricFlow outflow;
};

// Provides calibrated sensor readings, including tidal volume (TV)
// integrated from flow.
class Sensors {
 public:
  Sensors();

  // Perform some initial sensor calibration.  This function should
  // be called on system startup before any other sensor functions
  // are called.
  void Calibrate();

  // Read the sensors.
  SensorReadings GetReadings() const;

  // min/max possible reading from MPXV5004GP pressure sensors
  // The canonical list of hardware in the device is: https://bit.ly/3aERr69
  constexpr static Pressure MinPressure{kPa(0.0f)};
  constexpr static Pressure MaxPressure{kPa(3.92f)};

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
  Pressure ReadPressureSensor(Sensor s) const;
  float ReadOxygenSensor(Pressure p_ambient) const;

  // Calibrated average sensor values in a zero state.
  Voltage sensors_zero_vals_[NumSensors];

  using DbgFloat = Debug::Variable::Float;

  mutable DbgFloat inflow_air_dp_{"inflow_air_dp", Debug::Variable::Access::ReadOnly, 0.0f, "cmH2O",
                                  "Air influx differential pressure"};
  mutable DbgFloat inflow_oxy_dp_{"inflow_oxygen_dp", Debug::Variable::Access::ReadOnly, 0.0f,
                                  "cmH2O", "Concentrated oxygen influx differential pressure"};
  mutable DbgFloat outflow_dp_{"outflow_dp", Debug::Variable::Access::ReadOnly, 0.0f, "cmH2O",
                               "Exhale differential pressure"};
  mutable DbgFloat patient_pressure_{"pressure", Debug::Variable::Access::ReadOnly, 0.0f, "cmH2O",
                                     "Patient pressure"};
  mutable DbgFloat inflow_air_{"inflow_air", Debug::Variable::Access::ReadOnly, 0.0f, "mL/s",
                               "Air influx flow rate"};
  mutable DbgFloat inflow_oxy_{"inflow_oxygen", Debug::Variable::Access::ReadOnly, 0.0f, "mL/s",
                               "Concentrated oxygen influx flow rate"};
  mutable DbgFloat outflow_{"outflow", Debug::Variable::Access::ReadOnly, 0.0f, "mL/s",
                            "Outflow rate"};
  mutable DbgFloat fio2_{"fio2", Debug::Variable::Access::ReadOnly, 0.0f, "ratio",
                         "Fraction of inspired oxygen"};
};
