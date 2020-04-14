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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "model.h"
#include "ventilator_defaults.h"
#include "ventilator_limits.h"

void parameters_setKp(float kp_value);

float parameters_getKp();

void parameters_setKi(float ki_value);

float parameters_getKi();

void parameters_setKd(float kd_value);

float parameters_getKd();

void parameters_init();

// Respiratory rate
float parameters_getRR();
void parameters_setRR(float rr_value);

// Tidal volume
float parameters_getTV();
void parameters_setTV(float tv_value);

// Respiration rate
void parameters_setPEEP(float peep_value);
float parameters_getPEEP();

// Inspire/Expire ratio
void parameters_setInspireExpireRatio(float ier_value);
float parameters_getInspireExpireRatio();

// Peak Inspiratory Pressure (PIP)
void parameters_setPIP(float pip_value);

float parameters_getPIP();

// Dwell
void parameters_setDwell(float Dwell);
float parameters_getDwell();

enum operatingMode parameters_getOperatingMode();
void parameters_setOperatingMode(operatingMode);

void parameters_setPeriodicReadings(bool active);
bool parameters_getPeriodicReadings();

void parameters_setPeriodicMode(periodicMode periodicMode_value);
enum periodicMode parameters_getPeriodicMode();

void parameters_setVentilatorMode(ventilatorMode ventilatorMode_value);
enum ventilatorMode parameters_getVentilatorMode();

void parameters_setSolenoidNormalState(solenoidNormaleState normalState_value);
enum solenoidNormaleState parameters_getSolenoidNormalState();

#endif // PARAMETERS_H
