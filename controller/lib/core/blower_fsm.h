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

#ifndef BLOWER_FSM_H
#define BLOWER_FSM_H

#include "network_protocol.pb.h"
#include "units.h"

// This module encapsulates the blower's finite state machine (FSM).
//
// It determines the pressure setpoint that the blower fan should try to
// achieve at this moment.  In other words, it's responsible for generating an
// idealized pressure curve over time.
//
// Once this module has chosen a setpoint, it's the responsibility of the
// blower_pid module to drive the blower to achieve the desired pressure.
void blower_fsm_init();

// Gets the pressure that the blower should (ideally) deliver right now.
Pressure blower_fsm_get_setpoint(const VentParams &params);

#endif // BLOWER_FSM_H
