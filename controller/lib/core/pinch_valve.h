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

#ifndef PINCH_VALVE_H
#define PINCH_VALVE_H

#include "stepper.h"
#include "units.h"

enum class PinchValveHomeState {
  DISABLED,
  LOWER_AMP,
  SET_HOME_SPEED,
  MOVE_TO_STOP,
  WAIT_MOVE_STOP,
  SET_NORMAL_AMP,
  MOVE_OFFSET,
  WAIT_MOVE_OFFSET,
  ZERO_POS,
  SET_NORMAL_SPEED,
  HOMED
};

// The PinchValve represents a single stepper driven pinch
// value in the system.
//
// The pinch valve motors are expected to be wired up so
// that moves in the positive direction move away from the
// tube and therefore allow more flow, and moves in the
// negative direction move toward the tube pinching it and
// thus reduce flow.
// If you find that your motor moves in the opposite
// direction to this then you should swap the motor wires
// around to get the correct direction of motion

class PinchValve {
public:
  // Create a new pinch valve using the specified
  // stepper motor.
  explicit PinchValve(int motor_index);

  // Initialize the pinch value absolute position.
  // This should be called at startup from the
  // background loop before the value is used as
  // an actuator.
  //
  // It performs a motion into the pin valve hard
  // stop and sets the initial position based on
  // that move
  void Home();

  // This function is called from the high priority
  // loop to set the pinch valve position.
  // Values range from 0 (fully closed) to 1 (fully open)
  // Values outside that range will be clipped.
  void SetOutput(float value);

  // Disable the valve.  It will need to be homed
  // before it can be used
  void Disable();

  // Return true if the pinch valve is ready for action
  bool IsReady() { return home_state_ == PinchValveHomeState::HOMED; }

private:
  Time move_start_time_;

  int motor_index_;

  PinchValveHomeState home_state_{PinchValveHomeState::DISABLED};
};

#endif
