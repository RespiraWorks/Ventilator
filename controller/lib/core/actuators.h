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

#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "blower_fsm.h"
#include "hal.h"
#include <optional>

struct ActuatorsState {
  // Pressure set point in cm H2O.
  // Not really an actuator state (maybe it should move?)
  // Only meaningful in pressure control mode.
  float setpoint_cm_h2o{0.0f};

  // Valve setting for the FIO2 proportional solenoid
  // Range 0 to 1 where 0 is fully closed and 1 is fully open.
  float fio2_valve{0.0f};

  // Power for the blower fan.  Range 0 (off) to 1 (full power)
  float blower_power{0.0f};

  // Pinch valve that regulates output of the blower.
  // If the valve is disabled, then this is nullopt.
  // If enabled the range is 0 (fully closed) to 1 (fully open)
  std::optional<float> blower_valve;

  // Pinch valve that regulates exaust flow
  // If the valve is disabled, then this is nullopt.
  // If enabled the range is 0 (fully closed) to 1 (fully open)
  std::optional<float> exhale_valve;
};

// Causes passed state to be applied to the actuators
void actuators_execute(const ActuatorsState &desired_state);

// Returns true if the actuators are ready for action or false
// if they aren't (for example pinch valves are homing).
// The system should be kept in a safe state until this returns true.
bool AreActuatorsReady();

#endif // ACTUATORS_H
