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

#include "actuators.h"

void Actuators::execute(const ActuatorsState &desired_state) {
  // set blower PWM
  blower_.set(desired_state.blower_power);

  // Set the blower pinch valve position
  if (desired_state.blower_valve)
    blower_pinch_.SetOutput(*desired_state.blower_valve);
  else
    blower_pinch_.Disable();

  // Set the exhale pinch valve position
  if (desired_state.exhale_valve)
    exhale_pinch_.SetOutput(*desired_state.exhale_valve);
  else
    exhale_pinch_.Disable();

  psol_.set(desired_state.fio2_valve);
}

// Return true if all actuators are enabled and ready for action
bool Actuators::ready() { return blower_pinch_.IsReady() && exhale_pinch_.IsReady(); }
