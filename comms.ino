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

#include "comms.h"
#include "watchdog.h"

void comms_init() {
  serialIO_init();
}

void comms_send_reset_status() {

    // NOTE : Given the bootloader clears MCUSR, it's not possible to determine what reset the CPU without modifying the bootloader
    unsigned char watchdog_state;

    serialIO_send(data_type::status_packet, data_id::vc_boot, &watchdog_state, 0);
}
