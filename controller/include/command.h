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

#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <string.h>

#include "packet_types.h"
#include "parameters.h"
#include "serialIO.h"
#include "alarm.h"
#include "watchdog.h"

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void command_execute(enum command cmd, char *dataTx, uint8_t lenTx, char *dataRx, uint8_t *lenRx, uint8_t lenRxMax);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void command_responseSend(uint8_t cmd, char *packet, uint8_t len);

#endif // COMMAND_H
