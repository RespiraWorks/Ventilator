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
  PwmActuator(GPIO::PwmChannel channel, Frequency pwm_freq, Frequency cpu_frequency,
              const char *name, const char *help, float cal_0 = 0.0f, float cal_1 = 1.0f)
      : Actuator(name, help, cal_0, cal_1), pwm_pin_(channel, pwm_freq, cpu_frequency){};

  void set(const float value) { pwm_pin_.set(Actuator::get_value(value)); }

 private:
  GPIO::PwmPin pwm_pin_;
};
