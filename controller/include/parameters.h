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

#include "eeprom.h"
#include "packet_types.h"
#include "ventilator_limits.h"
#include "ventilator_defaults.h"

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setKp(float kp_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getKp();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setKi(float ki_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getKi();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setKd(float kd_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getKd();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_init();

// Respiratory rate

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getRR();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setRR(float rr_value);

// Tidal volume

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getTV();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setTV(float tv_value);

// Respiration rate

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setPEEP(float peep_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getPEEP();

// Inspire/Expire ratio

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setInspireExpireRatio(float ier_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getInspireExpireRatio();

// Peak Inspiratory Pressure (PIP)

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setPIP(float pip_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getPIP();

// Dwell

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setDwell(float Dwell);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
float parameters_getDwell();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
enum operatingMode parameters_getOperatingMode() ;

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setOperatingMode(enum operatingMode) ;

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setPeriodicMode(enum periodicMode periodicMode_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
enum periodicMode parameters_getPeriodicMode();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setVentilatorMode(enum ventilatorMode ventilatorMode_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
enum ventilatorMode parameters_getVentilatorMode();

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
void parameters_setSolenoidNormalState(enum solenoidNormaleState normalState_value);

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
enum solenoidNormaleState parameters_getSolenoidNormalState();

#endif // PARAMETERS_H
