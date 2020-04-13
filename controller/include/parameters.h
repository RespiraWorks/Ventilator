/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
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

#endif // PARAMETERS_H
