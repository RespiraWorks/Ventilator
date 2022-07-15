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

////////////////////////////////////////////////////////////////////////////////
/// WARNING: Do not include this file as header, nor compile it as a source! ///
///                                                                          ///
/// This is a template implementation file.                                  ///
/// It is included by the corresponding header.                              ///
////////////////////////////////////////////////////////////////////////////////

template <class SourceType>
AnalogSensor<SourceType>::AnalogSensor(const char *name, const char *help_supplement,
                                       SourceType&& source)
    : source_(std::move(source)),
      dbg_zero_("zero", Debug::Variable::Access::ReadOnly, 0.f, "V", "Voltage offset "),
      dbg_voltage_("voltage", Debug::Variable::Access::ReadOnly, 0.f, "V", "Voltage reading ") {
  dbg_zero_.prepend_name(name);
  dbg_zero_.append_help(help_supplement);

  dbg_voltage_.prepend_name(name);
  dbg_voltage_.append_help(help_supplement);
}

template <class SourceType>
void AnalogSensor<SourceType>::set_zero() {
  zero_ = source_.read();
  dbg_zero_.set(zero_.volts());
}

template <class SourceType>
float AnalogSensor<SourceType>::read_diff_volts() const {
  auto ret = (source_.read() - zero_).volts();
  dbg_voltage_.set(ret);
  return ret;
}
