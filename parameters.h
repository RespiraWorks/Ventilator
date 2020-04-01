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

// Used to check that parameters are within "sane" values
// TODO Fill these boundaries with realistic values

#define RR_BOUNDARY_MIN 0.0
#define RR_DOUNDARY_MAX 100.0

#define TV_BOUNDARY_MIN 0.0
#define TV_BOUNDARY_MAX 100.0

#define PEEP_BOUNDARY_MIN 0.0
#define PEEP_BOUNDARY_MAX 100.0

#define ED_BOUNDARY_MIN 0.0
#define ED_BOUNDARY_MAX 100.0

#define ID_BOUNDARY_MIN 0.0
#define ID_BOUNDARY_MAX 100.0

#define PIP_BOUNDARY_MIN 0.0
#define PIP_BOUNDARY_MAX 100.0

#define DWELL_BOUNDARY_MIN 0.0
#define DWELL_BOUNDARY_MAX 100.0

// Ventilator parameter defaults
// FIXME Put some default fallback values that are realistic 
#define RR_DEFAULT    10.0
#define TV_DEFAULT    10.0
#define PEEP_DEFAULT  10.0
#define ED_DEFAULT    10.0
#define ID_DEFAULT    10.0
#define PIP_DEFAULT   10.0
#define DWELL_DEFAULT 10.0

// Calibration parameter defaults

// PID parameter defaults

#define KP_DEFAULT   2.0
#define KI_DEFAULT   8.0
#define KD_DEFAULT   0.0

// Private function prototypes

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
static void init_defaultVentilatorParameters();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
static void init_defaultPIDParameters();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
static void init_defaultCalibrationParameters();




// Public function prototypes

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

// Expiration duration

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_setExpireDuration(float expireduration_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_getExpireDuration();

// Inspiration duration

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_setInspireDuration(float inspireduration_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_getInspireDuration();

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

#endif // PARAMETERS_H