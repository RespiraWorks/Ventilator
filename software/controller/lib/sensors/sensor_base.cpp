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

#include "sensor_base.h"

/// \TODO: generalize these classes to not require reference to entire HAL

AnalogSensor::AnalogSensor(const char *name, const char *help_supplement, GPIO::Port port,
                           uint8_t pin, ADC *adc, AdcChannel adc_channel)
    : pin_(port, pin, adc, adc_channel),
      dbg_zero_("zero", Debug::Variable::Access::ReadOnly, 0.f, "V", "Voltage offset "),
      dbg_voltage_("voltage", Debug::Variable::Access::ReadOnly, 0.f, "V", "Voltage reading ") {
  dbg_zero_.prepend_name(name);
  dbg_zero_.append_help(help_supplement);

  dbg_voltage_.prepend_name(name);
  dbg_voltage_.append_help(help_supplement);
}

void AnalogSensor::set_zero() {
  zero_ = pin_.read();
  dbg_zero_.set(zero_.volts());
}

float AnalogSensor::read_diff_volts() const {
  auto ret = (pin_.read() - zero_).volts();
  dbg_voltage_.set(ret);
  return ret;
}

PressureSensor::PressureSensor(const char *name, const char *help_supplement)
    : dbg_pressure_("dp", Debug::Variable::Access::ReadOnly, 0.f, "cmH2O",
                    "Differential pressure ") {
  dbg_pressure_.prepend_name(name);
  dbg_pressure_.append_help(help_supplement);
}

FlowSensor::FlowSensor(const char *name, const char *help_supplement)
    : dbg_flow_("flow", Debug::Variable::Access::ReadOnly, 0.f, "mL/s", "Volumetric flow ") {
  dbg_flow_.prepend_name(name);
  dbg_flow_.append_help(help_supplement);
}

OxygenSensor::OxygenSensor(const char *name, const char *help_supplement)
    : dbg_fio2_("fio2", Debug::Variable::Access::ReadOnly, 0.f, "ratio", "Fraction of oxygen ") {
  dbg_fio2_.prepend_name(name);
  dbg_fio2_.append_help(help_supplement);
}
