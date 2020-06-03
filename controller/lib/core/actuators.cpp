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

// Called once at system startup to initialize any
// actuators that need it
void actuators_init(void) {}

void actuators_execute(const ActuatorsState &desired_state) {
  // Open/close the solenoid as appropriate.
  // Our solenoid is "normally open", so low voltage means open and high
  // voltage means closed.  Hardware spec: https://bit.ly/3aERr69
  Hal.digitalWrite(BinaryPin::SOLENOID,
                   desired_state.expire_valve_state == ValveState::OPEN
                       ? VoltageLevel::LOW
                       : VoltageLevel::HIGH);
  // set blower PWM
  Hal.analogWrite(PwmPin::BLOWER, desired_state.fan_power);

  // Set the blower pinch valve position
  blower_pinch.SetOutput(desired_state.fan_valve);
}
