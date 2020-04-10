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

enum class medicalMode {
    start   = 0x00,     /* First medical mode command */
    end     = 0x10,     /* Final medical mode command */

    count
};

enum class engMode {
    start   = 0x20,     /* First engineering mode command */
    end     = 0x27,     /* Final engineering mode command */

    count
};

enum class mixedMode {
    start   = 0x40,     /* First mixed mode command */
    end     = 0x44,     /* Final mixed mode command */

    count
};

enum class command {

    /* Medical mode commands */

    set_rr          = 0x00,
    get_rr          = 0x01,
    set_tv          = 0x02,
    get_tv          = 0x03,
    set_peep        = 0x04,
    get_peep        = 0x05,
    set_pip         = 0x06,
    get_pip         = 0x07,
    set_dwell       = 0x08,
    get_dwell       = 0x09,
    set_id          = 0x0a,   /* Inspiration duration */
    get_id          = 0x0b,
    set_ed          = 0x0c,   /* Expiration duration */
    get_ed          = 0x0d,

    get_pressure    = 0x0e,
    get_flow        = 0x0f,
    get_volume      = 0x10,

    /* Engineering mode commands */

    set_kp          = 0x20,     /* PID constant Kp */
    get_Kp          = 0x21,
    set_Ki          = 0x22,     /* PID constant Ki */
    get_Ki          = 0x23,
    set_Kd          = 0x24,     /* PID constant Kd */
    get_Kd          = 0x25,

    set_blower      = 0x26,     /* Turn blower ON/OFF */
    reset_vc        = 0x27,     /* Reset Ventilation Controller */

    /* Mixed Engineering/Medical mode commands */

    set_periodic    = 0x40,     /* Periodic data transmission mode (Pressure, Flow, Volume) */
    get_periodic    = 0x41,
    set_mode        = 0x42,     /* Engineering or medical mode */
    get_mode        = 0x43,
    comms_check     = 0x44,     /* Communications check command */

    count                       /* Sentinel */
};

enum class msgType {
    cmd             = 0x00,     /* Command */
    ack             = 0x01,     /* VC alarm Ack */
    nAck            = 0x02,     /* VC alarm Fail */

    rAck            = 0x10,     /* Response Ack */
    rErrChecksum    = 0x11,     /* Response checksum error */
    rErrMode        = 0x12,     /* Response mode error */
    rErrCmd         = 0x13,     /* Response cmd error */

    status          = 0x20,     /* Status */
    alarm           = 0x30,     /* Alarm */
    data            = 0x40,     /* Data */

    count                       /* Sentinel */
};

enum class dataID {
    /* Status */
    vc_boot      = 0x80,    /* Status sent when arduino boots (includes software version) */


    /* Alarms */
    alarm_1     = 0xA0,
    alarm_2     = 0xA1,

    /* Data */
    data_1      = 0xC0,

    count                   /* Sentinel */
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

// Private function prototypes

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static bool packet_check(char *packet);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static bool packet_receive(char *packet, uint8_t *packet_len);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static bool packet_checksumValidation(char *packet, uint8_t len);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static bool packet_cmdValidatation(char *packet);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static bool packet_modeValidation(char *packet);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
enum processPacket process_packet(char *packet, uint8_t len);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void comms_sendModeERR(char *packet);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void comms_sendChecksumERR(char *packet);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void comms_sendCommandERR(char *packet);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void cmd_execute(enum command cmd, char *dataTx, uint8_t lenTx,
                        char *dataRx, uint8_t *lenRx, uint8_t lenRxMax);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void cmd_responseSend(char *packet, uint8_t len);

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void send_alarm();

/****************************************************************************************
 *  @brief
 *  @usage
 *  @param
 *  @param
 *  @return
 ****************************************************************************************/
static void cmd_responseSend(uint8_t cmd, char *packet, uint8_t len);

#endif  // COMMS_H
