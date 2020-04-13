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

#include "command.h"

/****************************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************************/

void command_execute(enum command cmd, char *dataTx, uint8_t lenTx, char *dataRx, uint8_t *lenRx, uint8_t lenRxMax) {

    *lenRx = 0; // Initialise the value to zero

    union {
      float f;
      unsigned char byte[4];
    } conv;

    switch(cmd) {
        /* Medical mode commands */

        case command::set_rr:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setRR(conv.f);

            break;

        case command::get_rr:

            *lenRx = 4;
            conv.f = parameters_getRR();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_tv:
            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setTV(conv.f);

            break;

        case command::get_tv:

            *lenRx = 4;
            conv.f = parameters_getTV();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_peep:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setPEEP(conv.f);

            break;

        case command::get_peep:

            *lenRx = 4;
            conv.f = parameters_getPEEP();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_pip:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setPIP(conv.f);

            break;

        case command::get_pip:

            *lenRx = 4;
            conv.f = parameters_getPIP();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_dwell:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setDwell(conv.f);

            break;

        case command::get_dwell:

            *lenRx = 4;
            conv.f = parameters_getDwell();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_ier:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setInspireExpireRatio(conv.f);

            break;

        case command::get_ier:

            *lenRx = 4;
            conv.f = parameters_getInspireExpireRatio();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;


        case command::get_pressure:

            break;

        case command::get_flow:

            break;

        case command::get_volume:

            break;

        /* Engineering mode commands */

        case command::set_kp:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setKp(conv.f);

            break;

        case command::get_Kp:

            *lenRx = 4;
            conv.f = parameters_getKp();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_Ki:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setKi(conv.f);

            break;

        case command::get_Ki:

            *lenRx = 4;
            conv.f = parameters_getKi();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_Kd:

            conv.byte[0] = dataTx[0];
            conv.byte[1] = dataTx[1];
            conv.byte[2] = dataTx[2];
            conv.byte[3] = dataTx[3];

            parameters_setKi(conv.f);

            break;

        case command::get_Kd:

            *lenRx = 4;
            conv.f = parameters_getKd();

            dataRx[0] = conv.byte[0];
            dataRx[1] = conv.byte[1];
            dataRx[2] = conv.byte[2];
            dataRx[3] = conv.byte[3];

            break;

        case command::set_blower:

            break;

        case command::reset_vc:

            // TODO Do any necessary cleaning up before reset

            watchdog_reboot();

            break;

        /* Mixed mode commands */

        case command::set_periodic:

            parameters_setPeriodicMode((enum periodicMode) dataTx[0]);

            break;

        case command::get_periodic:
            *lenRx = 1;
            dataRx[0] = (char) parameters_getPeriodicMode();

            break;

        case command::set_mode:

            parameters_setOperatingMode((enum operatingMode) dataTx[0]);

            break;

        case command::get_mode:

            *lenRx = 1;
            dataTx[0] = (char) parameters_getOperatingMode();

            break;

        case command::comms_check:

            break;

        default:

            break;
    }
}

void command_responseSend(uint8_t cmd, char *packet, uint8_t len) {
    serialIO_send(msgType::rAck, (enum dataID) cmd, packet, len);
}
