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

#include "pressure_sensors.h"

AnalogPressureSensor::AnalogPressureSensor(const char *name, const char *help_supplement,
                                           AnalogPin pin, float voltage_to_kPa)
    : PressureSensor(name, help_supplement),
      AnalogSensor(name, help_supplement, pin),
      voltage_to_kPa_(voltage_to_kPa) {}

Pressure AnalogPressureSensor::read(const HalApi &hal_api) const {
  auto ret = kPa(AnalogSensor::read_diff_volts(hal_api) * voltage_to_kPa_);
  dbg_pressure_.set(ret.cmH2O());
  return ret;
}

// The pressure sensors output 1-5V, and each additional 1V of output
// corresponds to an additional 1kPa of pressure difference.
// https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf.
//
// voltage_range is whatever the voltage is scaled to as captured by your ADC.
// Therefore, if we multiply the received voltage by 5/voltage_range, we get a pressure in kPa.
MPXV5004DP::MPXV5004DP(const char *name, const char *help_supplement, AnalogPin pin,
                       float voltage_range)
    : AnalogPressureSensor(name, help_supplement, pin, 5.f / voltage_range) {}

// Vout = Vss * (0.09 * P + 0.04) between 0 and 5 V :
// https://www.nxp.com/docs/en/data-sheet/MPX5010.pdf
// Vss = 5, therefore P = (Vout - Vzero) * (1 / 0.45)
//
// voltage_range is whatever the voltage is scaled to as captured by your ADC.
// Therefore, if we multiply the received voltage by 5/voltage_range and divide it by 0.45, we get a
// pressure in kPa.
MPXV5010DP::MPXV5010DP(const char *name, const char *help_supplement, AnalogPin pin,
                       float voltage_range)
    : AnalogPressureSensor(name, help_supplement, pin, 5.f / voltage_range / 0.45f) {}
