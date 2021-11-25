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

#include "system_constants.h"
#include "system_timer.h"

//////////////////////////////////////////////////////////////////
//                   SENSOR LOGICAL MAPPINGS                    //
//   Change these if you route your sensor tubing differently   //
//////////////////////////////////////////////////////////////////
GPIO::AdcChannel adc_channel(Sensor s) {
  switch (s) {
    case Sensor::PatientPressure:
      return InterimBoardAnalogPressure;
    case Sensor::OxygenInflowPressureDiff:
      return U3PatientPressure;
    case Sensor::AirInflowPressureDiff:
      return U4InhaleFlow;
    case Sensor::OutflowPressureDiff:
      return U5ExhaleFlow;
    case Sensor::FIO2:
      return InterimBoardOxygenSensor;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}

Sensors::Sensors() = default;

// NOTE - I can't do this in the constructor because the Hal needs to be initialized
// and we need to be able to write to the registers
void Sensors::init(Frequency cpu_frequency) {
  // Here we create all sensors and initialize the adc.
  patient_pressure_sensor_.emplace("patient_pressure_", "for patient airway pressure",
                                   adc_channel(Sensor::PatientPressure), &adc, ADC::VoltageRange);
  fio2_sensor_.emplace("fio2", "Fraction of oxygen in supplied air", adc_channel(Sensor::FIO2),
                       &adc);
  air_influx_sensor_dp_.emplace("air_influx_", "for ambient air influx",
                                adc_channel(Sensor::AirInflowPressureDiff), &adc,
                                ADC::VoltageRange);
  oxygen_influx_sensor_dp_.emplace("oxygen_influx_", "for concentrated oxygen influx",
                                   adc_channel(Sensor::OxygenInflowPressureDiff), &adc,
                                   ADC::VoltageRange);
  outflow_sensor_dp_.emplace("outflow_", "for outflow", adc_channel(Sensor::OutflowPressureDiff),
                             &adc, ADC::VoltageRange);

  // These require existing DP sensors to link to
  air_influx_sensor_.emplace("air_influx_", "for ambient air influx",
                             &air_influx_sensor_dp_.value(), VenturiPortDiameter,
                             VenturiChokeDiameter, VenturiCorrection);
  oxygen_influx_sensor_.emplace("oxygen_influx_", "for concentrated oxygen influx",
                                &oxygen_influx_sensor_dp_.value(), VenturiPortDiameter,
                                VenturiChokeDiameter, VenturiCorrection);
  outflow_sensor_.emplace("outflow_", "for outflow", &outflow_sensor_dp_.value(),
                          VenturiPortDiameter, VenturiChokeDiameter, VenturiCorrection);

  /// \TODO: fault somehow if this returns false
  [[maybe_unused]] bool buffer_size_sufficient = adc.initialize(cpu_frequency);
}

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
  SystemTimer::singleton().delay(milliseconds(20));

  patient_pressure_sensor_->set_zero();
  air_influx_sensor_dp_->set_zero();
  oxygen_influx_sensor_dp_->set_zero();
  outflow_sensor_dp_->set_zero();
  fio2_sensor_->set_zero();
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
      .patient_pressure = patient_pressure_sensor_->read(),
      .fio2 = fio2_sensor_->read(AmbientPressure),
      .air_inflow = air_influx_sensor_->read(AirDensity),
      .oxygen_inflow = oxygen_influx_sensor_->read(AirDensity),
      .outflow = outflow_sensor_->read(AirDensity),
  };
}
