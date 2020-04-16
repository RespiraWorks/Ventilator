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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "packet_types.h"
#include "ACV.h"
#include "PRVC.h"

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void controller_init();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void controller_handler();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void controller_setVentilatorMode(enum ventilatorMode vmode);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void controller_start();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void controller_stop();

#endif // CONTROLLER_H
