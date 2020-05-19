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

#include "errors.h"
#include <stdint.h>

/* Number of alarms we can store in the queue */
#define ALARM_NODES 4

// Each alarm can store 8 bytes - modifying this would mean modifying the
// memory copies.
#define ALARM_DATALEN 8

void alarm_init();
void alarm_add(const char *data);
int32_t alarm_read(uint32_t *timestamp, char *data);
bool alarm_available();
void alarm_remove();

#endif // ALARM_H
