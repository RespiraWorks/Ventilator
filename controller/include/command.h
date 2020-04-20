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

#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

#include "alarm.h"
#include "packet_types.h"
#include "parameters.h"
#include "serialIO.h"
#include "watchdog.h"

void command_execute(enum command cmd, char *dataTx, uint8_t lenTx,
                     char *dataRx, uint8_t *lenRx, uint8_t lenRxMax);
void command_responseSend(uint8_t cmd, char *packet, uint8_t len);

#endif // COMMAND_H
