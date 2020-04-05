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

#include <Arduino.h>

#include "comms.h"
#include "watchdog.h"
#include "version.h"
#include "serialIO.h"
#include "alarm.h"

void comms_init() {
    serialIO_init();
}

enum class handler_state {
    idle            = 0x00,
    packet_arriving = 0x01,
    packet_process  = 0x02,
    alarm_waiting   = 0x03,
    alarm_process   = 0x04,

    count
};

enum class packet_field {
    msg_type    = 0x00,
    cmd         = 0x01,
    dataID      = 0x02,
    len         = 0x03,
    data        = 0x04,
    checksum    = 0x05,

    count
};

static char rx_packet[64];
static char tx_packet[64];

void comms_handler() {

    static enum handler_state current_state = handler_state::idle;
    bool complete;

    switch(current_state) {

        case handler_state::idle:

            // TODO We don't need to execute though this FSM with the same frequency if there is nothing to do

            // Do we have any incoming packets?
            if(serialIO_dataAvailable()) {
                /* Serial data received */

                current_state = handler_state::packet_arriving;
            }
            else if(alarm_available()) { // Are any alarms waiting to be handled?
                // Change state to process alarm
                // This has lower priority than a packet recieved via serialIO

                current_state = handler_state::alarm_waiting;
            }
            else {
                current_state = handler_state::idle;
            }

            break;

        case handler_state::packet_arriving: /* Don't know what the packet is yet */
                // Keep receiving packet until completion

                complete = packet_check(rx_packet);

                // Check if packet has finished arriving
                if(complete) {
                    current_state = handler_state::packet_process;
                }

            break;

        case handler_state::packet_process: /* Alarm ACK or Command */
                // process_packet();
            break;

        case handler_state::alarm_waiting:

            break;

        case handler_state::alarm_process:

            break;

        default:

            break;
    }
}

static bool packet_check(char *packet) {

    // TODO Do we need to set serialtimer to something lower than 1 second?

    static enum packet_field current_field = packet_field::msg_type;
    static uint8_t packet_len = 0;

    switch(current_field) {

        case packet_field::msg_type:

            //NOTE: Assuming data already in buffer
            serialIO_readByte(&packet[packet_len]);

            // Process field - what kind of packet is this? Command or Ack?

            if(packet[packet_len] == msgType::cmd) {
                // Command packet
                current_field = packet_field::cmd;
            }
            else if(packet[packet_len] == msgType::ack || packet[packet_len] == msgType::nAck) {
                // Alarm acknowledgement
                current_field = packet_field::checksum;
            }
            else {
                // Something else
                // TODO Flag error
            }

            packet_len++;

            break;

        case packet_field::cmd:

            if(serialIO_dataAvailable()) {
                serialIO_readByte(&packet[packet_len++]);
                
                current_field = packet_field::len;
            }
            else {
                current_field = packet_field::cmd;
            }

            break;

        case packet_field::dataID:

            break;

        case packet_field::len:

            break;

        case packet_field::data:

            break;

        case packet_field::checksum:

            break;
    }

    return false;
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

    serialIO_send(msgType::status, dataID::vc_boot, resetData, sizeof(resetData));
}
