/* Copyright 2021, RespiraWorks

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

#include <stdint.h>

#include "interpolant.h"
#include "pwm.h"

class Actuator {
 public:
  Actuator(const char *name, const char *help, float cal_0 = 0.0f, float cal_1 = 1.0f)
      : calibration_("cal", cal_0, cal_1, "", "Calibration table") {
    calibration_.cal_table_.prepend_name(name);
    calibration_.cal_table_.append_help(help);
    forced_value_.prepend_name(name);
    forced_value_.prepend_name("forced_");
    forced_value_.append_help("Force the setting");
    forced_value_.append_help(help);
    forced_value_.append_help(
        " to a certain value in [0,1].  Specify a value outside"
        " this range to let the controller control it.");
  };
  Actuator(const char *name, const char *help, std::array<float, ActuatorsCalSize> calibration)
      : calibration_("cal", calibration, "", "Calibration table") {
    calibration_.cal_table_.prepend_name(name);
    calibration_.cal_table_.append_help(help);
    forced_value_.prepend_name(name);
    forced_value_.prepend_name("forced_");
    forced_value_.append_help("Force the setting");
    forced_value_.append_help(help);
    forced_value_.append_help(
        " to a certain value in [0,1].  Specify a value outside"
        " this range to let the controller control it.");
  };

  // Link calibration table to nv params (if deemed necessary)
  void LinkCalibration(NVParams::Handler *nv_params, const uint16_t offset) {
    calibration_.cal_table_.link(nv_params, offset);
  };

  // We shouldn't need to use this function outside of a derived class but is still public for
  // testability reasons.
  float get_value(float input) {
    auto forced = forced_value_.get();
    if (forced >= 0 && forced <= 1) return calibration_.get_value(forced);

    return calibration_.get_value(input);
  }

 protected:
  // Calibration table, linearizing value in [0,1] range
  Interpolant<ActuatorsCalSize> calibration_;

 private:
  // Debug var that can be used from debug interface to force the actuator's setting
  Debug::Variable::Float forced_value_{"setting", Debug::Variable::Access::ReadWrite, -1.f,
                                       "ratio"};
};

class PwmActuator : public Actuator {
 public:
  PwmActuator(const PwmPin pin, const Frequency pwm_freq, const char *name, const char *help,
              float cal_0 = 0.0f, float cal_1 = 1.0f)
      : Actuator(name, help, cal_0, cal_1), pwm_(pin, pwm_freq) {}

  void initialize_pwm(Frequency cpu_frequency) { pwm_.initialize(cpu_frequency); }

  void set(const float value) { pwm_.set(get_value(value)); }

 private:
  PWM pwm_;
};
