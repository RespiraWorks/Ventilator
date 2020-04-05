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
#include <Arduino.h>

#include "alarm.h"

void alarm_init() {

}

static int8_t stack_index = -1;


bool stackFull() {
    return (stack_index == (ALARM_NODES-1)) ? true : false;
}

bool stackEmpty() {
    return (stack_index == -1) ? true : false;
}

int32_t stackPeek(alarm_t *alarm) {

    int32_t return_status = RESULT_FAIL;

    if(!stackEmpty()) {
        alarm = &alarmStack_node[stack_index];
        return_status = RESULT_SUCCESS;
    }

    return return_status;
}

int32_t stackPop(alarm_t *alarm) {

    int32_t return_status = RESULT_FAIL;

    if(!stackEmpty()) {
        alarm = &alarmStack_node[stack_index];

        stack_index--;

        return_status = RESULT_SUCCESS;
    }

    return return_status;
}

void stackPush(alarm_t alarm) {

    if(!stackFull()) {
        stack_index++;

        alarmStack_node[stack_index] = alarm;
    }
    else {
        // Stack full, ignore alarm
        // TODO Log this
    }
}

void alarm_add(enum alarmID alarm, char *data) {
    alarm_t alarmSlot;

    alarmSlot.alarm = alarm;
    alarmSlot.timestamp = millis();

    // Copy alarm data
    alarmSlot.alarmData[0] = *data;
    alarmSlot.alarmData[1] = *(data+1);
    alarmSlot.alarmData[2] = *(data+2);
    alarmSlot.alarmData[3] = *(data+3);
    alarmSlot.alarmData[4] = *(data+4);
    alarmSlot.alarmData[5] = *(data+5);
    alarmSlot.alarmData[6] = *(data+6);
    alarmSlot.alarmData[7] = *(data+7);

    stackPush(alarmSlot);
}
