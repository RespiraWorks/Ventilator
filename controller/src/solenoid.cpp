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

void solenoid_init() {
    pinMode(O_SOLENOID, OUTPUT);
}

void solenoid_open() {
    if(parameters_getSolenoidNormalState() == solenoidNormaleState::normally_open) {
        digitalWrite(O_SOLENOID, LOW);
    }
    else {
        digitalWrite(O_SOLENOID, HIGH);
    }
}

void solenoid_close() {
    if(parameters_getSolenoidNormalState() == solenoidNormaleState::normally_open) {
        digitalWrite(O_SOLENOID, HIGH);
    }
    else {
        digitalWrite(O_SOLENOID, LOW);
    }
}
