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

#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

/*
* Stores all command numbers that can sent by the GUI
*/
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

/*
* Stores all message types that can sent by the Ventilator controller and the GUI
*/
enum class msgType {
    cmd             = 0x00,     /* Command */
    ack             = 0x01,     /* Ventilator Controller alarm Ack */
    nAck            = 0x02,     /* Ventilator Controller alarm Fail */

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
    /* DataID values should start at a value higher than the higest command number */

    /* Status */
    vc_boot      = 0x80,    /* Status sent when arduino boots (includes software version) */


    /* Alarms */
    alarm_1     = 0xA0,
    alarm_2     = 0xA1,

    /* Data */
    data_1      = 0xC0,

    count                   /* Sentinel */
};


#endif // PACKET_TYPES_H
