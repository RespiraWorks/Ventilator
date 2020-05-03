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
#include "hal.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "units.h"

void blower_pid_init();

// Runs one step of the blower_pid module, which is responsible for spinning
// the blower to attempt to achieve the given setpoint pressure and
// opening/closing the expire solenoid.
//
// Puts readings from e.g. the pressure sensor into `readings`, and puts the
// current fan power (range [0-1]) into `fan_power`.
void blower_pid_execute(const BlowerSystemState &desired_state,
                        SensorReadings *readings, float *fan_power);

#endif // BLOWER_PID_H
