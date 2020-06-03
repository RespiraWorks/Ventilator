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
#include "pinch_valve.h"

// local data
static PinchValve blower_pinch(0);
static PinchValve exhale_pinch(1);

void actuators_execute(const ActuatorsState &desired_state) {

  // set blower PWM
  Hal.analogWrite(PwmPin::BLOWER, desired_state.fan_power);

  // Set the blower pinch valve position
  if (desired_state.blower_valve_enabled)
    blower_pinch.SetOutput(desired_state.fan_valve);
  else
    blower_pinch.Disable();

  // Set the exhale pinch valve position
  if (desired_state.exhale_valve_enabled)
    exhale_pinch.SetOutput(desired_state.exhale_valve);
  else
    exhale_pinch.Disable();
}

// Return true if all actuators are enabled and ready for action
bool AreActuatorsReady() {
  return blower_pinch.IsReady() && exhale_pinch.IsReady();
}
