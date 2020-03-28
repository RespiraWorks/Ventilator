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

#include "serialIO.h"

enum class packetType {
    cmd_response    = 0x00, /* Reponse to a Interface Controller command */
    status          = 0x01, /* Ventilation Controller status */
    alarm           = 0x02, /* Ventilation Controller Alarm */
    data            = 0x03, /* Ventilation Controller Data */

    count
};

enum class interfaceCmd {

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

    /* Mode configuration */

    set_mode        = 0x50,     /* Engineering or medical mode */

    count                       /* Sentinel */
};

enum class interfaceStatus {
    vc_boot      = 0x01, /* Status sent when arduino boots - includes watchdog restart status */

    count                /* Sentinel */
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
void comms_send(double Setpoint, int16_t sensorValue);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void comms_send_flow(float flow);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void comms_send_pressure(float pressure);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void comms_send_volume(float volume);

/****************************************************************************************
 *  @brief      
 *  @usage      
 *  @param      
 *  @param      
 *  @return     
 ****************************************************************************************/
void comms_send_fpv(float flow, float pressure, float volume);


// Private function prototypes



#endif  // COMMS_H
