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

#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "blower_fsm.h"
#include "hal.h"

typedef struct _ActuatorsState {
  ValveState expire_valve_state;
  float fan_power;
} ActuatorsState;

#define ActuatorsState_init_zero                                               \
  { ValveState::OPEN, 0.0 }

void actuators_execute(ActuatorsState desired_state);

#endif // ACTUATORS_H
