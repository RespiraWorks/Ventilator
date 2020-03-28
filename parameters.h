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
#define RR_DEFAULT    = 10.0
#define TV_DEFAULT    = 10.0
#define PEEP_DEFAULT  = 10.0
#define ED_DEFAULT    = 10.0
#define ID_DEFAULT    = 10.0
#define PIP_DEFAULT   = 10.0
#define DWELL_DEFAULT = 10.0

// Calibration parameter defaults

// PID parameter defaults

#define KP_DEFAULT = 2.0
#define KI_DEFAULT = 8.0
#define KD_DEFAULT = 0.0

// Private function prototypes

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
static void init_default_ventilatorParameters();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
static void init_default_PIDParameters();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
static void init_default_CalibrationParameters();




// Public function prototypes

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_Kp(float kp_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_Kp();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_Ki(float ki_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_Ki();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_Kd(float kd_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_Kd(); 

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
float parameters_get_RR();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_RR(float rr_value);

// Tidal volume

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_TV();

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_TV(float tv_value);

// Respiration rate

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_PEEP(float peep_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_PEEP();

// Expiration duration

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_ExpireDuration(float expireduration_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_ExpireDuration();

// Inspiration duration

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_InspireDuration(float inspireduration_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_InspireDuration();

// Peak Inspiratory Pressure (PIP)

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_PIP(float pip_value);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_PIP();

// Dwell

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void parameters_set_Dwell(float Dwell);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
float parameters_get_Dwell();

#endif // PARAMETERS_H