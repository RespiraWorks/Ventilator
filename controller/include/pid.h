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
#ifndef PID_H
#define PID_H

#include <PID_v1.h>

#include "hal.h"

inline constexpr AnalogPinId DPSENSOR_PIN = AnalogPinId::HAL_A0;
inline constexpr PwmPinId BLOWERSPD_PIN = PwmPinId::PWM_3;
inline constexpr int BLOWER_HIGH = 142;
inline constexpr int BLOWER_LOW = 130;

// state machine variables
inline constexpr int INSPIRE_TIME = 1600;
inline constexpr int INSPIRE_RATE = 1;
inline constexpr int PIP = 142;
inline constexpr int INSPIRE_DWELL = 800;
inline constexpr int INSPIRE_DWELL_PRESSURE = 140;
inline constexpr int EXPIRE_TIME = 1000;
inline constexpr int EXPIRE_RATE = 1;
inline constexpr int PEEP = 130;
inline constexpr int EXPIRE_DWELL = 600;

// not implemented yet
inline constexpr int AC = 0;
inline constexpr int RR = 0;
inline constexpr int IE = 0;

void pid_execute();
void pid_init();

#endif // PID_H
