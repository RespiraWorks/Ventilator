/* Copyright 2020, Edwin Chiu

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

#include "solenoid.h"

#include "hal.h"
#include <stdint.h>

// Solenoid output pin 5 TBC.
static const int O_SOLENOID = 5;

void solenoid_init() { Hal.setDigitalPinMode(O_SOLENOID, PinMode::HAL_OUTPUT); }

void solenoid_open() {
  // TODO: The solenoid normally open/closed param shouldn't come from the GUI;
  // where is it supposed to come from?  Is this just a hardcoded constant?
  /*if (parameters_getSolenoidNormalState() ==
      solenoidNormaleState::normally_open) {
    Hal.digitalWrite(O_SOLENOID, VoltageLevel::HAL_LOW);
  } else {
    Hal.digitalWrite(O_SOLENOID, VoltageLevel::HAL_HIGH);
  }*/
}

void solenoid_close() {
  // TODO: The solenoid normally open/closed param shouldn't come from the GUI;
  // where is it supposed to come from?  Is this just a hardcoded constant?
  /*if (parameters_getSolenoidNormalState() ==
      solenoidNormaleState::normally_open) {
    Hal.digitalWrite(O_SOLENOID, VoltageLevel::HAL_HIGH);
  } else {
    Hal.digitalWrite(O_SOLENOID, VoltageLevel::HAL_LOW);
  }*/
}
