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
