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
 *    PRIVATE FUNCTION PROTOTYPES
 ****************************************************************************************/

static float convIntTofloat(char *data, uint8_t len);
static void convfloatToInt(float float_value, char *data, uint8_t len);

/****************************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************************/

void command_execute(enum command cmd,
                     char *dataTx,
                     uint8_t lenTx,
                     char *dataRx,
                     uint8_t *lenRx,
                     uint8_t lenRxMax) {

    *lenRx = 0; // Initialise the value to zero

    switch(cmd) {
        /* Medical mode commands */

        case command::set_rr:

            parameters_setRR(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_rr:

            *lenRx = 4;
            convfloatToInt(parameters_getRR(), dataRx, *lenRx);

            break;

        case command::set_tv:

            parameters_setTV(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_tv:

            *lenRx = 4;

            convfloatToInt(parameters_getTV(), dataRx, *lenRx);

            break;

        case command::set_peep:

            parameters_setPEEP(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_peep:

            *lenRx = 4;

            convfloatToInt(parameters_getPEEP(), dataRx, *lenRx);

            break;

        case command::set_pip:

            parameters_setPIP(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_pip:

            *lenRx = 4;
            convfloatToInt(parameters_getPIP(), dataRx, *lenRx);

            break;

        case command::set_dwell:

            parameters_setDwell(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_dwell:

            *lenRx = 4;
            convfloatToInt(parameters_getDwell(), dataRx, *lenRx);

            break;

        case command::set_ier:

            parameters_setInspireExpireRatio(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_ier:

            *lenRx = 4;
            convfloatToInt(parameters_getInspireExpireRatio(), dataRx, *lenRx);

            break;


        case command::get_pressure:

            break;

        case command::get_flow:

            break;

        case command::get_volume:

            break;

        /* Engineering mode commands */

        case command::set_kp:

            parameters_setKp(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_Kp:

            *lenRx = 4;
            convfloatToInt(parameters_getKp(), dataRx, *lenRx);

            break;

        case command::set_Ki:

            parameters_setKi(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_Ki:

            *lenRx = 4;

            convfloatToInt(parameters_getKi(), dataRx, *lenRx);

            break;

        case command::set_Kd:

            parameters_setKi(convIntTofloat(dataTx, lenTx));

            break;

        case command::get_Kd:

            *lenRx = 4;
            convfloatToInt(parameters_getKd(), dataRx, *lenRx);

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

        case command::set_ventilatorMode:

            parameters_setVentilatorMode((enum ventilatorMode) dataTx[0]);

            break;

        case command::get_ventilatorMode:

            *lenRx = 1;
            dataTx[0] = (char) parameters_getVentilatorMode();

            break;

        case command::start_ventilator:

            break;

        case command::stop_ventilator:

            break;

        case command::set_solenoidNormalState:

            parameters_setSolenoidNormalState((enum solenoidNormaleState) dataTx[0]);

            break;

        default:

            break;
    }
}

void command_responseSend(uint8_t cmd, char *packet, uint8_t len) {
    serialIO_send(msgType::rAck, (enum dataID) cmd, packet, len);
}

/****************************************************************************************
*    PRIVATE FUNCTIONS
****************************************************************************************/

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
static float convIntTofloat(char *data, uint8_t len) {

    float f;

    memcpy(&f, data, len);

    return f;
}

/****************************************************************************************
*  @brief
*  @usage
*  @param
*  @param
*  @return
****************************************************************************************/
static void convfloatToInt(float float_value, char *data, uint8_t len) {
    memcpy(data, &float_value, len);
}
