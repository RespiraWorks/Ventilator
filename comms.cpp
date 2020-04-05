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

#include <string.h>
#include <Arduino.h>

#include "comms.h"
#include "watchdog.h"
#include "version.h"
#include "serialIO.h"

void comms_init() {
    serialIO_init();
}

void comms_handler() {

}

void comms_sendResetState() {
    char resetData[12];
    uint32_t time;
    char *version;

    // NOTE : Given the bootloader clears MCUSR, it's not possible to determine what reset the CPU without modifying the bootloader

    version = version_getVersion();

    time = millis();

    resetData[0] = (time >> 24) & 0xFF;
    resetData[1] = (time >> 16) & 0xFF;
    resetData[2] = (time >> 8) & 0xFF;
    resetData[3] = time & 0xFF;

    resetData[4] = *version;
    resetData[5] = *(version+1);
    resetData[6] = *(version+2);
    resetData[7] = *(version+3);
    resetData[8] = *(version+4);
    resetData[9] = *(version+5);
    resetData[10] = *(version+6);
    resetData[11] = *(version+7);

    serialIO_send(dataType::status_packet, dataID::vc_boot, resetData, sizeof(resetData));
}
