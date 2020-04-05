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

#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include "comms.h"
/*
typedef struct alarm {
    enum alarmID alarm;
    uint32_t timestamp;
    char alarmData[8];
    struct alarm *next_ptr;
} alarm_t;
*/




#define RESULT_FAIL -1
#define RESULT_SUCCESS 0

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void alarm_init();

/****************************************************************************************
 *  @brief      
 *  @usage  Add an alarm to the end of the queue. If the queue is full, new alarm is lost.  
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void alarm_add(enum alarmID alarm, char *data);

/****************************************************************************************
 *  @brief      
 *  @usage  Read the alarm at the start of the queue
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void alarm_read(enum alarmID *alarm, char *data);

/****************************************************************************************
 *  @brief      
 *  @usage   Are there any alarms available in the queue?   
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
bool alarm_available();

/****************************************************************************************
 *  @brief      
 *  @usage  Remove the alarm at the start of the queue    
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void alarm_remove();

#endif // ALARM_H
