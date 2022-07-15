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

////////////////////////////////////////////////////////////////////////////////
/// WARNING: Do not include this file as header, nor compile it as a source! ///
///                                                                          ///
/// This is a template implementation file.                                  ///
/// It is included by the corresponding header.                              ///
////////////////////////////////////////////////////////////////////////////////

template <class SourceType>
TeledyneR24<SourceType>::TeledyneR24(const char *name, const char *help_supplement,
                                     SourceType&& source)
    : OxygenSensor(name, help_supplement), AnalogSensor<SourceType>(name, help_supplement, std::move(source)) {}

// Reads an oxygen sensor, returning the concentration of oxygen [0 ; 1.0]
//
// Output scales with partial pressure of O2, so ambient pressure must be
// compensated to get an accurate FIO2.
template <class SourceType>
float TeledyneR24<SourceType>::read(Pressure p_ambient) const {
  // Teledyne R24-compatible Electrochemical Cell Oxygen Sensor
  // http://www.medicalsolutiontechnology.com/wp-content/uploads/2012/09/GO-04-DATA-SHEET.pdf
  // Sensitivity of 0.060V/fio2, where fio2 is 0.0 to 1.0, at pressure = 1atm
  // https://www.apogeeinstruments.com/content/SO-100-200-spec-sheet.pdf:
  // sensitivity similar to SO-210-SS PCB has an op-amp to gain the output up by
  // 50V/V This gives about 3.0V full scale.

  // Standard air O2 concentration. This assumes that calibration occurred with
  // pure air, meaning the system has been filled with air only.
  static constexpr float O2ConcentrationInAir{0.21f};

  static constexpr float AmplifierGain{50.0f};
  static constexpr float OxygenSensorGain{0.060f};

  // TODO: raise alarm if fio2 is out of expected (0,1) range
  auto ret =
      AnalogSensor<SourceType>::read_diff_volts() / (AmplifierGain * OxygenSensorGain) / p_ambient.atm() +
      O2ConcentrationInAir;

  dbg_fio2_.set(ret);

  return ret;
}
