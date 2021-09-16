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

#include "sensor_base.h"

class AnalogPressureSensor : public PressureSensor, public AnalogSensor {
 public:
  AnalogPressureSensor(const char *name, const char *help_supplement, AnalogPin pin,
                       float voltage_to_kPa);

  Pressure read(const HalApi &hal_api) const override;

 private:
  // Assume linear relationship, pending further research
  float voltage_to_kPa_;
};

class MPXV5004DP : public AnalogPressureSensor {
 public:
  MPXV5004DP(const char *name, const char *help_supplement, AnalogPin pin, float voltage_range);

  // min/max possible reading from MPXV5004DP pressure sensors
  // \TODO: are we supposed to use these somehow?
  constexpr static Pressure MinPressure{kPa(0.0f)};
  constexpr static Pressure MaxPressure{kPa(3.92f)};
};

class MPXV5010DP : public AnalogPressureSensor {
 public:
  MPXV5010DP(const char *name, const char *help_supplement, AnalogPin pin, float voltage_range);
  // min/max possible reading from MPXV5010DP pressure sensors
  constexpr static Pressure MinPressure{kPa(0.0f)};
  constexpr static Pressure MaxPressure{kPa(10.0f)};
};
