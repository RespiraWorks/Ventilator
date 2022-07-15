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
#include "vars.h"

/// SourceType is any type that has a function with the following signature:
///     Voltage read() const;

template <class SourceType>
class AnalogSensor {
 public:
  AnalogSensor(const char *name, const char *help_supplement, SourceType &&source);

  void set_zero();

  float read_diff_volts() const;

 private:
  SourceType source_;
  Voltage zero_;

  mutable Debug::Variable::Float dbg_zero_;
  mutable Debug::Variable::Float dbg_voltage_;
};

#include "analog_sensor.tpp"
