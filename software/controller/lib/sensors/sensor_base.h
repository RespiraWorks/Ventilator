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

#include "hal.h"
#include "units.h"
#include "vars.h"

class AnalogSensor {
 public:
  AnalogSensor(const char *name, const char *help_supplement, AnalogPin pin);

  void set_zero(const HalApi &hal_api);

  float read_diff_volts(const HalApi &hal_api) const;

 private:
  AnalogPin pin_;
  Voltage zero_;

  mutable Debug::Variable::Float dbg_zero_;
  mutable Debug::Variable::Float dbg_voltage_;
};

class PressureSensor {
 public:
  virtual Pressure read(const HalApi &hal_api) const = 0;

 protected:
  PressureSensor(const char *name, const char *help_supplement);
  mutable Debug::Variable::Float dbg_pressure_;
};

class FlowSensor {
 public:
  virtual VolumetricFlow read(const HalApi &hal_api, float air_density) const = 0;

 protected:
  FlowSensor(const char *name, const char *help_supplement);
  mutable Debug::Variable::Float dbg_flow_;
};

class OxygenSensor {
 public:
  virtual float read(const HalApi &hal_api, Pressure p_ambient) const = 0;

 protected:
  OxygenSensor(const char *name, const char *help_supplement);
  mutable Debug::Variable::Float dbg_fio2_;
};
