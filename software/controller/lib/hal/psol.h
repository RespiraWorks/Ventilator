/* Copyright 2020-2021, RespiraWorks

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

#include "vars.h"

/// \TODO: generalize to be instantiable (dbg var prefixes, etc..)
class PSOL {
 public:
  void initialize(uint32_t cpu_frequency_hz);

  // Set the PSOL output level to a value from 0 (fully closed) to 1 (fully open)
  void set(float val);

 private:
  // Testing in Edwin's garage, we found that the psol was fully closed at
  // somewhere between 0.75 and 0.80 (i.e. definitely zero at 0.75 and probably
  // zero a bit above that) and fully open at 0.90.

  // UFlow IBV19M value
  Debug::Variable::Float pwm_closed_{"psol_pwm_closed", Debug::Variable::Access::ReadWrite, 0.35f,
                                     "ratio", "PWM power that closes the psol [0,1]"};

  // UFlow IBV19M value
  Debug::Variable::Float pwm_open_{"psol_pwm_open", Debug::Variable::Access::ReadWrite, 0.75f,
                                   "ratio", "PWM power that opens the psol [0,1]"};
};
