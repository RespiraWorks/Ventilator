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

#pragma once

#include <stdint.h>

#include "actuator_base.h"
#include "pwm.h"

class PwmActuator : public Actuator {
 public:
  PwmActuator(const PwmPin pin, const Frequency pwm_freq, const char *name, const char *help,
              float cal_0 = 0.0f, float cal_1 = 1.0f)
      : Actuator(name, help, cal_0, cal_1), pwm_(pin, pwm_freq){};

  void initialize_pwm(Frequency cpu_frequency) { pwm_.initialize(cpu_frequency); }

  void set(const float value) { pwm_.set(get_value(value)); }

 private:
  PWM pwm_;
};
