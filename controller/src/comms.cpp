/* Copyright 2020, Edwin Chiu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "comms.h"
#include "hal.h"

/****************************************************************************************
 *    PRIVATE FUNCTION PROTOTYPES
 ****************************************************************************************/

static bool packet_check(char *packet);
static bool packet_receive(char *packet, uint8_t *packet_len);
static bool packet_checksumValidation(char *packet, uint8_t len);
static bool packet_cmdValidatation(char *packet);
static bool packet_modeValidation(char *packet);
static enum processPacket process_packet(char *packet, uint8_t len);
static void comms_sendModeERR(char *packet);
static void comms_sendChecksumERR(char *packet);
static void comms_sendCommandERR(char *packet);
static void cmd_execute(enum command cmd, char *dataTx, uint8_t lenTx,
                        char *dataRx, uint8_t *lenRx, uint8_t lenRxMax);
static void cmd_responseSend(char *packet, uint8_t len);
static void send_alarm();
static void cmd_responseSend(uint8_t cmd, char *packet, uint8_t len);

/****************************************************************************************
 *    DEFINE STATEMENTS
 ****************************************************************************************/

#define PACKET_LEN_MAX (32)

/****************************************************************************************
 *    PRIVATE VARIABLES
 ****************************************************************************************/

static char rx_packet[PACKET_LEN_MAX];
// 5 (MSGTYPE[1] + DATAID[1] + LEN [1] + CHECKSUM[2])
static char cmdResponse_data[PACKET_LEN_MAX - 5];

/****************************************************************************************
 *    TYPE DEFINITIONS
 ****************************************************************************************/

enum class handler_state {
  idle = 0x00,
  packet_arriving = 0x01,
  packet_process = 0x02,
  alarm_waiting = 0x03,
  alarm_process = 0x04,

  count /* Sentinel */
};

enum class packet_field {
  msg_type = 0x00,
  cmd = 0x01,
  len = 0x02,
  data = 0x03,
  checksumA = 0x04,
  checksumB = 0x05,

  count /* Sentinel */
};

/****************************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************************/

static bool packet_check(char *packet);
static bool packet_receive(char *packet, uint8_t *packet_len);
static bool packet_checksumValidation(char *packet, uint8_t len);
static bool packet_cmdValidatation(char *packet);
static bool packet_modeValidation(char *packet);
static enum processPacket process_packet(char *packet, uint8_t len);
static void comms_sendModeERR(char *packet);
static void comms_sendChecksumERR(char *packet);
static void comms_sendCommandERR(char *packet);
static void cmd_execute(enum command cmd, char *dataTx, uint8_t lenTx,
                        char *dataRx, uint8_t *lenRx, uint8_t lenRxMax);
static void cmd_responseSend(char *packet, uint8_t len);
static void send_alarm();
static void cmd_responseSend(uint8_t cmd, char *packet, uint8_t len);

void comms_init() { serialIO_init(); }

void comms_handler() {
  static enum handler_state state = handler_state::idle;
  static uint8_t packet_len = 0;
  static bool alarm_sent = false;
  static uint32_t alarmSentTime = 0;
  bool received = false;
  uint8_t cmdResponseData_len;
  enum processPacket packetStatus;

  // Timeout alarm waiting if it has been sent
  if (alarm_sent == true) {
    if ((Hal.millis() - alarmSentTime) >= DELAY_100MS) {
      // Alarm hasn't ack arrived within DELAY_100MS,
      // lets send the alarm again
      alarm_sent = false;
    }
  }

  switch (state) {
  case handler_state::idle:

    // TODO We don't need to execute though this FSM with the same frequency if
    // there is nothing to do

    // Do we have any incoming packets?
    if (serialIO_dataAvailable()) {
      /* Serial data received */
      state = handler_state::packet_arriving;
    } else if (alarm_available() &&
               alarm_sent == false) { // Are any alarms waiting to be handled?
      // Change state to process alarm
      // This has lower priority than a packet recieved via serialIO
      state = handler_state::alarm_waiting;
    } else {
      state = handler_state::idle;
    }
    break;

  case handler_state::packet_arriving: /* Don't know what the packet is yet */
    // Keep receiving packet until completion
    received = packet_receive(rx_packet, &packet_len);

    // Check if packet has finished arriving
    if (received) {
      state = handler_state::packet_process;
    }
    break;

  case handler_state::packet_process: /* Alarm ACK or Command */
    packetStatus = process_packet(rx_packet, packet_len);
    switch (packetStatus) {
    case processPacket::command:
      command_execute((enum command)rx_packet[(uint8_t)packet_field::cmd],
                      &rx_packet[(uint8_t)packet_field::data],
                      rx_packet[(uint8_t)packet_field::len], cmdResponse_data,
                      &cmdResponseData_len, sizeof(cmdResponse_data));

      // Send response to Interface Controller
      command_responseSend((uint8_t)rx_packet[(uint8_t)packet_field::cmd],
                           cmdResponse_data, cmdResponseData_len);
      break;

    case processPacket::ack:
      // We've received an alarm acknowledgement, remove the alarm from the
      // buffer
      alarm_remove();
      alarm_sent = false;
      break;

    case processPacket::nack:
      // The IC sent an ack packet (success or fail). Cannot be certain, don't
      // remove the alarm in the buffer
      alarm_sent = false;
      break;

    case processPacket::checksumErr:
      // The received packet had an invalid checksum, send error
      comms_sendChecksumERR(rx_packet);
      break;

    case processPacket::modeErr:
      // The received packet had an invalid mode, send error
      comms_sendModeERR(rx_packet);
      break;

    case processPacket::invalidErr:
      // The received packet had an invalid command, send error
      comms_sendCommandERR(rx_packet);
      break;

    case processPacket::msgTypeUnknown:
      // TODO Handle this error case
      break;

    default:
      // TODO handle this error case
      break;
    }

    state = handler_state::idle;
    break;

  case handler_state::alarm_waiting:
    // Read alarm
    send_alarm();

    alarm_sent = true;
    alarmSentTime = Hal.millis();
    state = handler_state::idle;
    break;

  default:
    // TODO Undefined state, log Error
    state = handler_state::idle; // Make sure FSM always ends in defined state
    break;
  }
}

void comms_sendResetState() {
  // NOTE : Given the bootloader clears MCUSR, it's not possible to determine
  // what reset the CPU without modifying the bootloader

  const char *version = version_getVersion();
  uint32_t time = Hal.millis();

  char resetData[12];
  resetData[0] = (time >> 24) & 0xFF;
  resetData[1] = (time >> 16) & 0xFF;
  resetData[2] = (time >> 8) & 0xFF;
  resetData[3] = time & 0xFF;

  // TODO Is using memcpy faster than this?
  resetData[4] = *version;
  resetData[5] = *(version + 1);
  resetData[6] = *(version + 2);
  resetData[7] = *(version + 3);
  resetData[8] = *(version + 4);
  resetData[9] = *(version + 5);
  resetData[10] = *(version + 6);
  resetData[11] = *(version + 7);

  serialIO_send(msgType::status, dataID::vc_boot, resetData, sizeof(resetData));
}

void comms_sendPeriodicReadings(float pressure, float volume, float flow) {
  char readingsData[16];
  uint32_t time;

  time = Hal.millis();

  readingsData[0] = (time >> 24) & 0xFF;
  readingsData[1] = (time >> 16) & 0xFF;
  readingsData[2] = (time >> 8) & 0xFF;
  readingsData[3] = time & 0xFF;

  readingsData[4] = (((uint32_t)pressure) >> 24) & 0xFF;
  readingsData[5] = (((uint32_t)pressure) >> 16) & 0xFF;
  readingsData[6] = (((uint32_t)pressure) >> 8) & 0xFF;
  readingsData[7] = ((uint32_t)pressure) & 0xFF;

  readingsData[8] = (((uint32_t)volume) >> 24) & 0xFF;
  readingsData[9] = (((uint32_t)volume) >> 16) & 0xFF;
  readingsData[10] = (((uint32_t)volume) >> 8) & 0xFF;
  readingsData[11] = ((uint32_t)volume) & 0xFF;

  readingsData[12] = (((uint32_t)flow) >> 24) & 0xFF;
  readingsData[13] = (((uint32_t)flow) >> 16) & 0xFF;
  readingsData[14] = (((uint32_t)flow) >> 8) & 0xFF;
  readingsData[15] = ((uint32_t)flow) & 0xFF;

  serialIO_send(msgType::data, dataID::data_1, readingsData,
                sizeof(readingsData));
}

/****************************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************************/

static void send_alarm() {
  uint32_t timestamp;
  char data[ALARM_DATALEN + sizeof(timestamp)];
  enum dataID alarmID;

  if (alarm_read(&alarmID, &timestamp, &data[sizeof(timestamp)]) ==
      VC_STATUS_SUCCESS) {

    data[0] = (timestamp >> 24) & 0xFF;
    data[1] = (timestamp >> 16) & 0xFF;
    data[2] = (timestamp >> 8) & 0xFF;
    data[3] = timestamp & 0xFF;

    serialIO_send(msgType::alarm, alarmID, data, sizeof(data));
  } else {
    // TODO Handle error
  }
}

static enum processPacket process_packet(char *packet, uint8_t len) {

  // Validate packet checksum
  if (!packet_checksumValidation(packet, len)) {
    // Checksum invalid
    // Therefore cannot be 100% sure that received msgType is correct
    // Can we assume however that the length is correct? If yes,
    // An Ack packet only has three bytes.
    // Ignore if Ack with checksum error has been received
    // The alarm will stay on the stack and we'll try to resend it later

    if (len == 3) {
      // Assume that the recieved packet is a ack/nack following
      // the transmission of an alarm
      return processPacket::nack;
    }

    return processPacket::checksumErr;
  }

  // What packet type is it?
  if (packet[(uint8_t)packet_field::msg_type] == (uint8_t)msgType::cmd) {
    // It's a command packet
    if (packet_cmdValidatation(packet) == true) {
      if (packet_modeValidation(packet)) {
        // Command ok for execution
        return processPacket::command;
      } else {
        // Send mode error packet
        return processPacket::modeErr;
      }
    } else {
      // Send cmd invalid packet
      return processPacket::invalidErr;
    }
  } else if (packet[(uint8_t)packet_field::msg_type] == (uint8_t)msgType::ack) {
    // An ACK packet
    return processPacket::ack;
  } else if (packet[(uint8_t)packet_field::msg_type] ==
             (uint8_t)msgType::nAck) {
    // A NACK packet
    return processPacket::nack;
  }

  // Error condition - unexpected msg_type
  return processPacket::msgTypeUnknown;
}

static bool packet_checksumValidation(char *packet, uint8_t len) {
  return checksum_check(packet, len);
}

static bool packet_cmdValidatation(char *packet) {
  uint8_t cmd = (uint8_t)packet[(uint8_t)packet_field::cmd];
  return (cmd >= (uint8_t)medicalMode::start &&
          cmd <= (uint8_t)medicalMode::end) ||
         (cmd >= (uint8_t)engMode::start && cmd <= (uint8_t)engMode::end) ||
         (cmd >= (uint8_t)mixedMode::start &&
          cmd <= (uint8_t)mixedMode::end);
}

static bool packet_modeValidation(char *packet) {
  uint8_t cmd = (uint8_t)packet[(uint8_t)packet_field::cmd];

  // If medical mode, reject eng-mode commands.
  bool isEngMode =
      cmd >= (uint8_t)engMode::start && cmd <= (uint8_t)engMode::end;
  return parameters_getOperatingMode() != operatingMode::medical || !isEngMode;
}

static bool packet_receive(char *packet, uint8_t *len) {
  // TODO Do we need to set serialtimer to something lower than 1 second?

  static enum packet_field field = packet_field::msg_type;
  static uint8_t packet_len = 0;
  static uint8_t data_len = 0;
  char msg_type;
  bool packet_complete = false;

  // Accelerate the packet acquisition process whilst data is waiting
  // by repeatedly reading the data
  while (serialIO_dataAvailable()) {
    switch (field) {
    case packet_field::msg_type:
      // NOTE: Assuming data already in buffer
      serialIO_readByte(&packet[packet_len]);

      msg_type = packet[packet_len];

      // Process field - what kind of packet is this? Command or Ack?
      if (msg_type == (char)msgType::cmd) {
        // Command packet
        field = packet_field::cmd;
      } else if (msg_type == (char)msgType::ack ||
                 msg_type == (char)msgType::nAck) {
        // Alarm acknowledgement
        field = packet_field::checksumA;
      } else {
        // Something else
        // FIXME Flag error
      }
      packet_len++;
      break;

    case packet_field::cmd:
      serialIO_readByte(&packet[packet_len++]);
      field = packet_field::len;
      break;

    case packet_field::len:
      serialIO_readByte(&packet[packet_len++]);

      // If no data, skip straight to the checksum
      if (packet[(uint8_t)packet_field::len] == 0)
        field = packet_field::checksumA;
      else
        field = packet_field::data;
      break;

    case packet_field::data:
      // TODO Set maximum LEN (incase LEN contains a number too high)
      serialIO_readByte(&packet[packet_len++]);
      if (++data_len == packet[(uint8_t)packet_field::len]) {
        field = packet_field::checksumA;
      }
      break;

    case packet_field::checksumA:
      serialIO_readByte(&packet[packet_len++]);
      field = packet_field::checksumB;
      break;

    case packet_field::checksumB:
      serialIO_readByte(&packet[packet_len++]);

      // Reset static counters to default values
      data_len = 0;
      *len = packet_len; // Save packet length
      packet_len = 0;
      field = packet_field::msg_type;
      packet_complete = true;
      break;

    default:
      // Should never arrive there
      // TODO Log error
      break;
    }
  }

  return packet_complete;
}

static void comms_sendModeERR(char *packet) {
  char emptyData;
  serialIO_send(msgType::rErrMode,
                (enum dataID)packet[(uint8_t)packet_field::cmd], &emptyData, 0);
}

static void comms_sendChecksumERR(char *packet) {
  char emptyData;
  serialIO_send(msgType::rErrChecksum,
                (enum dataID)packet[(uint8_t)packet_field::cmd], &emptyData, 0);
}

static void comms_sendCommandERR(char *packet) {
  char emptyData;
  serialIO_send(msgType::rErrCmd,
                (enum dataID)packet[(uint8_t)packet_field::cmd], &emptyData, 0);
}
