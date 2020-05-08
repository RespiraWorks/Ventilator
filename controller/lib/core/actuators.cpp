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

// This module is responsible for passing the actuator commands to Hal to
// generate the actual electrical signals.

#include "actuators.h"

void actuators_execute(ActuatorsState desired_state) {
  // Open/close the solenoid as appropriate.
  // Our solenoid is "normally open", so low voltage means open and high
  // voltage means closed.  Hardware spec: https://bit.ly/3aERr69
  Hal.digitalWrite(BinaryPin::SOLENOID,
                   desired_state.expire_valve_state == ValveState::OPEN
                       ? VoltageLevel::HAL_LOW
                       : VoltageLevel::HAL_HIGH);
  // set blower PWM
  // TODO: create PWM limits in HAL (255) and use this instead
  Hal.analogWrite(PwmPin::BLOWER,
                  static_cast<int>(desired_state.fan_power * 255));
}
