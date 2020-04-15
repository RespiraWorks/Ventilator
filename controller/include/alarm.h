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
