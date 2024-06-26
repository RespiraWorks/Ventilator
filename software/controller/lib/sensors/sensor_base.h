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

class PressureSensor {
 public:
  virtual Pressure read() const = 0;

 protected:
  PressureSensor(const char *name, const char *help_supplement);
  mutable Debug::Variable::Float dbg_pressure_;
};

class FlowSensor {
 public:
  virtual VolumetricFlow read(float air_density) const = 0;

 protected:
  FlowSensor(const char *name, const char *help_supplement);
  mutable Debug::Variable::Float dbg_flow_;
};

class OxygenSensor {
 public:
  virtual float read(Pressure p_ambient) const = 0;

 protected:
  OxygenSensor(const char *name, const char *help_supplement);
  mutable Debug::Variable::Float dbg_fio2_;
};
