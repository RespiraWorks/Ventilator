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

#include "oxygen.h"
#include "pressure_sensors.h"
#include "venturi.h"

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

 private:
  /// \TODO: get this either from IDC constants header or something like that
  static constexpr float ADC_voltage_range_{3.3f};

  // \TODO: create a physical constants header for custom parts like venturi
  // Diameters and correction coefficient relating to 3/4in Venturi, see https://bit.ly/2ARuReg.
  // Correction factor of 0.97 is based on ISO recommendations for Reynolds of roughly 10^4 and
  // machined (rather than cast) surfaces. Data fit is in good agreement based on comparison to
  // Fleisch pneumotachograph; see https://github.com/RespiraWorks/Ventilator/pull/476
  constexpr static Length venturi_port_diameter_{millimeters(15.05f)};
  constexpr static Length venturi_choke_diameter_{millimeters(5.5f)};
  constexpr static float venturi_correction_{0.97f};

  static_assert(venturi_port_diameter_ > venturi_choke_diameter_);
  static_assert(venturi_choke_diameter_ > meters(0));

  // Fundamental sensors
  MPXV5004DP patient_pressure_sensor_{"patient_pressure_", " in patient airway",
                                      PinFor(Sensor::PatientPressure), ADC_voltage_range_};
  TeledyneR24 fio2_sensor_{"fio2", PinFor(Sensor::FIO2), " "};
  MPXV5004DP air_influx_sensor_dp_{"air_influx_", " for ambient air influx",
                                   PinFor(Sensor::AirInflowPressureDiff), ADC_voltage_range_};
  MPXV5004DP oxygen_influx_sensor_dp_{"oxygen_influx_", " for concentrated oxygen influx",
                                      PinFor(Sensor::OxygenInflowPressureDiff), ADC_voltage_range_};
  MPXV5004DP outflow_sensor_dp_{"outflow_", " for outflow", PinFor(Sensor::OutflowPressureDiff),
                                ADC_voltage_range_};

  // These require existing DP sensors to link to
  VenturiFlowSensor air_influx_sensor_{"air_influx_",           " for ambient air influx",
                                       &air_influx_sensor_dp_,  venturi_port_diameter_,
                                       venturi_choke_diameter_, venturi_correction_};
  VenturiFlowSensor oxygen_influx_sensor_{
      "oxygen_influx_",       " for concentrated oxygen influx", &oxygen_influx_sensor_dp_,
      venturi_port_diameter_, venturi_choke_diameter_,           venturi_correction_};
  VenturiFlowSensor outflow_sensor_{"outflow_",
                                    " for outflow",
                                    &outflow_sensor_dp_,
                                    venturi_port_diameter_,
                                    venturi_choke_diameter_,
                                    venturi_correction_};
};
