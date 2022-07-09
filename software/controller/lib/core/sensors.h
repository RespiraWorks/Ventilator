/* Copyright 2020-2022, RespiraWorks

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

#include "units.h"

struct SensorReadings {
  Pressure patient_pressure;

  // fraction of inspired oxygen (fiO2)
  float fio2;

  // These are "uncorrected" values.  We account for high-frequency noise by
  // e.g. averaging many samples, but we don't account here for low-frequency
  // sensor zero-point drift.
  VolumetricFlow air_inflow;
  VolumetricFlow oxygen_inflow;
  VolumetricFlow outflow;
};

// Provides calibrated sensor readings
class AbstractSensors {
 public:
  // Performs calibration of the sensors. For now, since sensor calibration are not stored in
  // EEPROM, this function should be called immediately after init, but it shouldn't stay that way
  // (to accommodate for ventilator restart while linked to a patient). Assume system quiescence,
  // sufficient pause before this step.
  /// \TODO store calibration in EEPROM and have this called only upon user request to make certain
  //    the vent is not linked to a patient
  virtual void calibrate() = 0;

  // Read the sensors.
  virtual SensorReadings get_readings() const = 0;
};
