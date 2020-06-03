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

// This file implements the ventilator controller used when the ventilator
// is turned off.  This is a pretty trivial state, we just disable the
// pinch valves to reduce current draw and turn the fan off.
//
// We also support some debug variables that are only available when
// running in this mode.  These variables allow various actuators to be
// controlled through the debug interface for testing purposes
//
// FIXME - The test function should be moved to a special test mode in the
// future, but is currently being handled here for convenience

#include "controller.h"
#include "vars.h"

// local data
static DebugFloat dbg_fan_power("test_fan_power",
                                "Fan power setting for testing (0 to 1)");
static DebugFloat
    dbg_blower_valve("test_blower_valve",
                     "Blower valve setting for test. 0 to 1 or -1 to disable");
static DebugFloat
    dbg_exhale_valve("test_exhale_valve",
                     "Exhale valve setting for test. 0 to 1 or -1 to disable");

// When we first enter the disabled state, clear all the debug
// variables
void VentModeNone::Enter() {
  dbg_fan_power.Set(0);
  dbg_blower_valve.Set(-1.0f);
  dbg_exhale_valve.Set(-1.0f);
}

ActuatorsState VentModeNone::Run(const VentParams &params,
                                 const SensorReadings &readings) {
  ActuatorsState state;

  state.fan_power = dbg_fan_power.Get();

  state.fan_valve = dbg_blower_valve.Get();
  state.exhale_valve = dbg_exhale_valve.Get();

  state.blower_valve_enabled = dbg_blower_valve.Get() >= 0;
  state.exhale_valve_enabled = dbg_exhale_valve.Get() >= 0;

  return state;
}
