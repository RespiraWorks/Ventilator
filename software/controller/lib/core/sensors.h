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

#include <optional>

#include "oxygen.h"
#include "pressure_sensors.h"
#include "system_constants.h"
#include "venturi.h"

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

  // Performs general init.  This function should
  // be called on system startup before any other sensor functions
  // are called.
  void init(Frequency cpu_frequency);

  // Performs calibration of the sensors. For now, since sensor calibration are not
  // stored in EEPROM, this function should be called immediately after init, but
  // it shouldn't stay that way (to accomodate for ventilator restart while linked
  // to a patient)
  void calibrate();

  // Read the sensors.
  SensorReadings get_readings() const;

  // for testing purposes, I made this a public member
  ADC adc;

 private:
  // Fundamental sensors. Because those use GPIO Pins, we need to delay instantiation to after
  // the Hal has been properly initialized. We use std::optional to achieve that, and emplace them
  // within init function
  std::optional<MPXV5010DP> patient_pressure_sensor_{std::nullopt};
  std::optional<TeledyneR24> fio2_sensor_{std::nullopt};
  std::optional<MPXV5004DP> air_influx_sensor_dp_{std::nullopt};
  std::optional<MPXV5004DP> oxygen_influx_sensor_dp_{std::nullopt};
  std::optional<MPXV5004DP> outflow_sensor_dp_{std::nullopt};

  // These require existing DP sensors to link to.
  std::optional<VenturiFlowSensor> air_influx_sensor_{std::nullopt};
  std::optional<VenturiFlowSensor> oxygen_influx_sensor_{std::nullopt};
  std::optional<VenturiFlowSensor> outflow_sensor_{std::nullopt};
};
