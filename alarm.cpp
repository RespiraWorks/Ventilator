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

static stack_t stack;

void alarm_init() {
    stack.top = -1;
}

void alarm_add(enum dataID alarmID, char *data) {
    alarm_t alarm;

    if(!stack_full()) {
        // No point spending time doing these operations if the stack is full

        alarm.alarm = alarmID;
        alarm.timestamp = millis();

        // Copy alarm data
        for(uint8_t idx = 0; idx < ALARM_DATALEN; idx++) {
            alarm.data[idx] = *(data + idx);
        }

        stack_push(alarm);
    }
    else {
        // Stack full, ignore alarm
        // TODO Log this
    }
}

bool alarm_available() {
    return (stack_empty() == false) ? true : false;
}

void alarm_remove() {
    alarm_t *alarm;

    stack_pop(&alarm); // Don't need this alarm anymore, remove it
}

int32_t alarm_read(enum dataID *alarmID, uint32_t *timestamp, char *data) {

    int32_t return_status = VC_STATUS_FAILURE;
    alarm_t *alarm;

    return_status = stack_peek(&alarm);

    if(return_status == VC_STATUS_SUCCESS) {
        *alarmID   = (enum dataID)  alarm->alarm;
        *timestamp = alarm->timestamp;

        for(uint8_t idx = 0; idx < ALARM_DATALEN; idx++) {
            *(data + idx) = alarm->data[idx];
        }
    }

    return return_status;
}

static bool stack_full() {
    return (stack.top == (ALARM_NODES-1)) ? true : false;
}

static bool stack_empty() {
    return (stack.top == -1) ? true : false;
}

static int32_t stack_peek(alarm_t **alarm) {

    int32_t return_status = VC_STATUS_FAILURE;

    if(!stack_empty()) {
        *alarm = &stack.alarm[stack.top];

        return_status = VC_STATUS_SUCCESS;
    }

    return return_status;
}

static int32_t stack_pop(alarm_t **alarm) {

    int32_t return_status = VC_STATUS_FAILURE;

    if(!stack_empty()) {
        *alarm = &stack.alarm[stack.top];

        stack.top--;

        return_status = VC_STATUS_SUCCESS;
    }

    return return_status;
}

static void stack_push(alarm_t alarm) {
    if(!stack_full()) {
        stack.top++;

        stack.alarm[stack.top] = alarm;
    }
}
