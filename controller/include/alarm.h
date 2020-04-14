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
#include "errors.h"

#define ALARM_NODES     4 /* Number of alarms we can store in the queue */
#define ALARM_DATALEN   8 /* Each alarm can store 8 bytes - modifying this would mean modifying the memory copys*/

typedef struct alarm {
    enum dataID alarm;
    uint32_t timestamp;
    char data[ALARM_DATALEN];
} alarm_t;

typedef struct {
    alarm_t alarm[ALARM_NODES];
    int8_t top;
} stack_t;

// Public function prototypes

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
 *  @usage  Add an alarm to the top of the stack. If the stack is full, new alarm is lost.
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void alarm_add(enum dataID alarm, char *data);

/****************************************************************************************
 *  @brief
 *  @usage  Read the alarm at the top of the stack
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
int32_t alarm_read(enum dataID *alarmID, uint32_t *timestamp, char *data);

/****************************************************************************************
 *  @brief
 *  @usage   Are there any alarms available on the stack?
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
bool alarm_available();

/****************************************************************************************
 *  @brief
 *  @usage  Remove the alarm at the top of the stack
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void alarm_remove();

#endif // ALARM_H
