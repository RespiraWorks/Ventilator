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

#ifndef SERIALIO_H
#define SERIALIO_H

#include <stdint.h>
#include "comms.h"

// Public functions

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void serialIO_init();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void serialIO_send(enum msgType type, enum dataID id, char *data, uint8_t len);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
bool serialIO_checkChecksum(char *packet, uint8_t packet_len);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
bool serialIO_dataAvailable();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
void serialIO_readByte(char *buffer);


// Private functions

/****************************************************************************************
 *  @brief
 *  @usage    Original checksum function taken from here https://en.wikipedia.org/wiki/Fletcher%27s_checksum
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static uint16_t Fletcher16_calc(uint16_t *sum1, uint16_t *sum2, char *data, uint8_t count);

#endif // SERIALIO_H
