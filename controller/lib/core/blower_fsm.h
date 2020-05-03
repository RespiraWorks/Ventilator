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

// This module encapsulates the blower system's finite state machine (FSM).
//
// The controllable parts of the "blower system" are
//
//  - the blower fan, which generates air pressure, and
//  - the expire valve, a solenoid which, when open, allows air to exit the
//    system.
//
// The purpose of this module is to determine, at any point in time, the ideal
// state of the blower system:
//
//  - the pressure that (ideally) should exist in the system, and
//  - whether the expire valve is open or closed.
//
// In other words, it's responsible for generating an idealized pressure curve
// and a valve on/off control over time.
//
// Once this module has determined the ideal state, it's the responsibility of
// the blower_pid module to open/close the valve and drive the blower to
// achieve the desired pressure.

enum class ValveState {
  OPEN,
  CLOSED,
};

// Represents a state that the blower FSM wants us to achieve at a given point
// in time.
struct BlowerSystemState {
  // Is the blower on?
  //
  // Note: blower_enabled == false isn't the same as setpoint_pressure == 0:
  //
  //  - If blower_enabled is false, we shut down the fan immediately, whereas
  //  - if setpoint_pressure == 0, PID spins down the fan to attempt to read 0
  //    kPa measured patient pressure.
  //
  bool blower_enabled;

  Pressure setpoint_pressure;
  ValveState expire_valve_state;
};

void blower_fsm_init();

// Gets the state that the the blower system should (ideally) deliver right
// now.
BlowerSystemState blower_fsm_desired_state(const VentParams &params);

#endif // BLOWER_FSM_H
