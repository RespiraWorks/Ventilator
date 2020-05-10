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
#ifndef BLOWER_PID_H
#define BLOWER_PID_H

#include "blower_fsm.h"
#include "network_protocol.pb.h"
#include "units.h"

void blower_pid_init();

// Computes the fan power necessary to match pressure setpoint in desired state
// by running the necessary step of the pid with input = current pressure
// fan power represents the necessary power between 0 (Off) and 1 (full power)
//
float blower_pid_compute_fan_power(Time now,
                                   const BlowerSystemState &desired_state,
                                   const SensorReadings &sensor_readings);

#endif // BLOWER_PID_H
