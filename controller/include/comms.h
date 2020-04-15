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

#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>

#include "checksum.h"

#include "packet_types.h"

enum class processPacket {
    checksumErr     = 0x00,
    modeErr         = 0x01,
    invalidErr      = 0x02,
    command         = 0x03,
    ack             = 0x04,
    nack            = 0x05,
    msgTypeUnknown  = 0x06,

    count                       /* Sentinel */
};

/* Used to store the minimum and maximum medical mode command numbers
*  This data is used to validate the incoming commands to be sure that
*  they're in the proper ranges.
*/
enum class medicalMode {
    start   = 0x00,     /* First medical mode command */
    end     = 0x10,     /* Final medical mode command */

    count
};

/* Used to store the minimum and maximum engineering mode command numbers
*  This data is used to validate the incoming commands to be sure that
*  they're in the proper ranges.
*/
enum class engMode {
    start   = 0x20,     /* First engineering mode command */
    end     = 0x27,     /* Final engineering mode command */

    count
};

/* Used to store the minimum and maximum mixed mode command numbers
*  This data is used to validate the incoming commands to be sure that
*  they're in the proper ranges.
*/
enum class mixedMode {
    start   = 0x40,     /* First mixed mode command */
    end     = 0x44,     /* Final mixed mode command */

    count
};


// Public function prototypes

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_init();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_handler();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_sendFlow(float flow);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_sendPressure(float pressure);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_sendVolume(float volume);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_sendFlowPressureVolume(float flow, float pressure, float volume);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_sendResetState();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void comms_sendPeriodicReadings(float pressure, float volume, float flow);

#endif  // COMMS_H
