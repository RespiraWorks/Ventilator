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
  Actuator(const char *name, const char *help, float cal_0 = 0.0f, float cal_1 = 1.0f);
  Actuator(const char *name, const char *help, std::array<float, ActuatorsCalSize> calibration);

  // Link calibration table to nv params (if deemed necessary).
  // This function overwrites the existing (init?) calibration values with contents of nv_params
  void LinkCalibration(NVParams::Handler *nv_params, const uint16_t offset) {
    calibration_.cal_table_.link(nv_params, offset);
  };

  // We shouldn't need to use this function outside of a derived class but is still public for
  // testability reasons.
  float get_value(float input);

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
              float cal_0 = 0.0f, float cal_1 = 1.0f);

  void initialize_pwm(Frequency cpu_frequency) { pwm_.initialize(cpu_frequency); }

  void set(const float value) { pwm_.set(get_value(value)); }

 private:
  PWM pwm_;
};
