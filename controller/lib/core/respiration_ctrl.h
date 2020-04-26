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

#ifndef RESPIRATION_CTRL_H
#define RESPIRATION_CTRL_H

#include "ACV.h"
#include "PRVC.h"

#include <PID_v1.h>

#include "network_protocol.pb.h"

// TODO: replace this with proper drivers call
inline constexpr AnalogPinId DPSENSOR_PIN = AnalogPinId::HAL_A0;
inline constexpr PwmPinId BLOWERSPD_PIN = PwmPinId::PWM_3;

void respiration_ctrl_init();

// Runs one step of the respiration control module, which is responsible for
// spinning up the blower.
//
// `params` is the last set of params we received from the GUI.  Puts readings
// from e.g. the pressure sensor into `readings`.
void respiration_ctrl_handler(const VentParams &params,
                              SensorReadings *readings);

#endif // RESPIRATIONCTRL_H
