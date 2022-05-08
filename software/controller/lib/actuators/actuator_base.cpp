/* Copyright 2021-2022, RespiraWorks

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

#include "actuator_base.h"

Actuator::Actuator(const char *name, const char *help, const char *setting_name, float cal_0,
                   float cal_1)
    : calibration_("cal", cal_0, cal_1, "", "Calibration table") {
  InitDebugVars(name, help, setting_name);
};

Actuator::Actuator(const char *name, const char *help,
                   std::array<float, ActuatorsCalSize> calibration, const char *setting_name)
    : calibration_("cal", calibration, "", "Calibration table") {
  InitDebugVars(name, help, setting_name);
};

void Actuator::InitDebugVars(const char *name, const char *help, const char *setting_name) {
  calibration_.cal_table_.prepend_name(name);
  calibration_.cal_table_.append_help(help);
  forced_value_.prepend_name(setting_name, /*strict=*/true);
  forced_value_.prepend_name(name);
  forced_value_.prepend_name("forced_");
  forced_value_.append_help("Force the", /*strict=*/true);
  forced_value_.append_help(setting_name);
  forced_value_.append_help(help);
  forced_value_.append_help(
      " to a certain value in [0,1].  Specify a value outside"
      " this range to let the controller control it.");
};

float Actuator::get_value(float input) {
  auto forced = forced_value_.get();
  if (forced >= 0 && forced <= 1) return calibration_.get_value(forced);

  return calibration_.get_value(input);
}
