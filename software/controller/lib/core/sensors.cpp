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

#include "sensors.h"

//////////////////////////////////////////////////////////////////
//                   SENSOR LOGICAL MAPPINGS                    //
//   Change these if you route your sensor tubing differently   //
//////////////////////////////////////////////////////////////////
AnalogPin sensor_pin(Sensor s) {
  switch (s) {
    case Sensor::PatientPressure:
      return AnalogPin::InterimBoardAnalogPressure;
    case Sensor::OxygenInflowPressureDiff:
      return AnalogPin::U3PatientPressure;
    case Sensor::AirInflowPressureDiff:
      return AnalogPin::U4InhaleFlow;
    case Sensor::OutflowPressureDiff:
      return AnalogPin::U5ExhaleFlow;
    case Sensor::FIO2:
      return AnalogPin::InterimBoardOxygenSensor;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}

Sensors::Sensors() = default;

// NOTE - I can't do this in the constructor now because it gets called before
// the HAL is set up, so the busy wait never finishes.
void Sensors::calibrate() {
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
  hal.Delay(milliseconds(20));

  patient_pressure_sensor_.set_zero(hal);
  air_influx_sensor_dp_.set_zero(hal);
  oxygen_influx_sensor_dp_.set_zero(hal);
  outflow_sensor_dp_.set_zero(hal);
  fio2_sensor_.set_zero(hal);
}

/// \TODO: Add alarms if sensor value is out of expected range?

SensorReadings Sensors::get_readings() const {
  //@TODO: Potential Caution: Density of air slightly varies over temperature and
  // altitude - need mechanism to adjust based on delivery? Constant involving
  // density of air. Density assumed at 15 deg. Celsius and 1 atm of pressure.
  // Sourced from https://en.wikipedia.org/wiki/Density_of_air
  static constexpr float AirDensity{1.225f};  // kg/m^3

  // Assuming ambient pressure of 101.3 kPa
  // TODO: measure ambient pressure from an additional sensor
  //  and/or estimate from user input (from altitude?)
  static constexpr Pressure AmbientPressure = kPa(101.3f);

  return {
      .patient_pressure = patient_pressure_sensor_.read(hal),
      .fio2 = fio2_sensor_.read(hal, AmbientPressure),
      .air_inflow = air_influx_sensor_.read(hal, AirDensity),
      .oxygen_inflow = oxygen_influx_sensor_.read(hal, AirDensity),
      .outflow = outflow_sensor_.read(hal, AirDensity),
  };
}
