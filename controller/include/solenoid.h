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

#ifndef SOLENOID_H
#define SOLENOID_H

#include <Arduino.h>

#include "actuators.h"
#include "stdint.h"

/****************************************************************************************
 * SOLENOID STATE DEFINTIONS
 ****************************************************************************************/

enum class solenoid_normaleState {
    normally_open    = 0x00,
    normally_closed  = 0x01,

    count                       /* Sentinel */
};

 /****************************************************************************************
  * PUBLIC FUNCTION PROTOTYPES
  ****************************************************************************************/

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void solenoid_init();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void solenoid_setNormalState();


/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void solenoid_open();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void solenoid_close();

#endif // SOLENOID_H
