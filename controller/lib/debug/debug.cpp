/* Copyright 2020, RespiraWorks

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

/*
 * The debug serial port interface implements a very simple binary command
 * structure.  Commands are sent using the following format:
 *
 *   <cmd> <data> <crc> <term>
 *
 * <cmd> is a single byte command code.
 *
 * <data> is zero or more bytes of command data.  The meaning of the data
 * is dependent on the command code.
 *
 * <crc> is a 16-bit CRC on the command and data bytes sent LSB first
 *
 * The response to a command has a similar structure:
 *   <err> <data> <crc> <term>
 *
 * <err> is an error code (0 for success)
 *
 * <data> is any data returned from the command.  If there's an error
 * there is never any data
 *
 * <term> is a special character value indicating the end of a command.
 *
 * There are two special char values are used:
 * - TERM (0xF2) - Signifies the end of a command.
 * - ESC  (0xF1) - used to send a special character as data.
 *
 * The escape byte causes the serial processor to treat the next byte as
 * data no matter what its value is.  It's used when the data being sent
 * has a special value.
 *
 */

#include "debug.h"
#include "hal.h"
#include "pb_read_cmd.h"
#include "peek_cmd.h"
#include "poke_cmd.h"
#include "sprintf.h"
#include "trace_cmd.h"
#include "var_cmd.h"
#include <stdarg.h>
#include <string.h>

// Mode command.  This returns a single byte of data which
// gives the firmware mode:
//  0 - Running in normal mode
//  1 - Running in boot mode.
//
// We don't actually have a boot mode yet, but its only
// a matter of time.  Once we start doing things like
// updating firmware (not through a debugger) we will
// need a separate boot loader image to ensure graceful
// recovery.
class ModeCmd : public DebugCmd {
public:
  ModeCmd() : DebugCmd(DbgCmdCode::MODE) {}
  DbgErrCode HandleCmd(CmdContext *context) override {
    context->resp_len = 1;
    context->resp[0] = 0;
    return DbgErrCode::OK;
  }
};
ModeCmd modeCmd;

// global debug handler
DebugSerial debug;

// List of registered command handlers
DebugCmd *DebugCmd::cmdList[256];

DebugSerial::DebugSerial() {
  buffNdx = 0;
  pollState = DbgPollState::WAIT_CMD;
  prevCharEsc = false;

  // TODO - This is annoying.  I had intended to make the constructors
  // of the various commands automatically add them to this list, but
  // the linker keeps removing them and I can't figure out how to
  // prevent that.  For now I'm just explicitely adding them here.
  // They still add themselves in their static constructors, but
  // that shouldn't cause any harm.
  DebugCmd::cmdList[static_cast<int>(DbgCmdCode::MODE)] = &modeCmd;
  DebugCmd::cmdList[static_cast<int>(DbgCmdCode::PEEK)] = &peekCmd;
  DebugCmd::cmdList[static_cast<int>(DbgCmdCode::POKE)] = &pokeCmd;
  DebugCmd::cmdList[static_cast<int>(DbgCmdCode::PRINT_BUFF_READ)] = &pbReadCmd;
  DebugCmd::cmdList[static_cast<int>(DbgCmdCode::VAR)] = &varCmd;
  DebugCmd::cmdList[static_cast<int>(DbgCmdCode::TRACE)] = &traceCmd;
}

// This function is called from the main low priority background loop.
// Its a simple state machine that waits for a new command to be received
// over the debug serial port.  Process the command when one is received
// and sends the response back.
void DebugSerial::Poll() {
  switch (pollState) {
  // Waiting for a new command to be received.
  // I continue to process bytes until there are no more available,
  // or a full command has been received.  Either way, the
  // ReadNextByte function will return false when its time
  // to move on.
  case DbgPollState::WAIT_CMD:
    while (ReadNextByte()) {
    }
    return;

  // Process the current command
  case DbgPollState::PROCESS_CMD:
    ProcessCmd();
    return;

  // Send my response
  case DbgPollState::SEND_RESP:
    while (SendNextByte()) {
    }
    return;
  }
}

// Format a debug string printf sytle and save it to a local buffer
// which can be queried by the interface program.
//
// Returns the number of bytes actually written to the print buffer
int DebugSerial::Print(const char *fmt, ...) {
  char buff[300];

  // Note that this uses a local sprintf implementation because
  // the one from the standard libraries will potentially dynamically
  // allocate memory.
  va_list ap;
  va_start(ap, fmt);
  int len = RWvsnprintf(buff, sizeof(buff), fmt, ap);
  va_end(ap);

  // Write as much as will fit to my print buffer.
  for (int i = 0; i < len; i++) {
    if (!printBuff.Put(buff[i]))
      return i;
  }

  return len;
}

// Read the next byte from the debug serial port
// Returns false if there were no more bytes available
// Also returns false if a full command has been received.
bool DebugSerial::ReadNextByte() {
  // Get the next byte from the debug serial port
  // if there is one.
  char ch;
  if (Hal.debugRead(&ch, 1) < 1)
    return false;

  uint8_t byte = static_cast<uint8_t>(ch);

  // If the previous character received was an escape character
  // then just save this byte (assuming there's space)
  if (prevCharEsc) {
    prevCharEsc = false;

    if (buffNdx < static_cast<int>(sizeof(cmdInBuff)))
      cmdInBuff[buffNdx++] = byte;
    return true;
  }

  // If this is an escape character, don't save it
  // just keep track of the fact that we saw it.
  if (byte == static_cast<uint8_t>(DbgSpecial::ESC)) {
    prevCharEsc = true;
    return true;
  }

  // If this is an termination character, then
  // change our state and return false
  if (byte == static_cast<uint8_t>(DbgSpecial::TERM)) {
    pollState = DbgPollState::PROCESS_CMD;
    return false;
  }

  // For other boring characters, just save them
  // if there's space in my buffer
  if (buffNdx < static_cast<int>(sizeof(cmdInBuff)))
    cmdInBuff[buffNdx++] = byte;
  return true;
}

// Send the next byte of my response to the last command.
// Returns false if no more data will fit in the output
// buffer, or if the entire response has been sent
bool DebugSerial::SendNextByte() {

  // To simplify things below, I require at least 3 bytes
  // in the output buffer to continue
  if (Hal.debugBytesAvailableForWrite() < 3)
    return false;

  // See what the next character to send is.
  uint8_t ch = cmdOutBuff[buffNdx++];

  // If its a special character, I need to escape it.
  if ((ch == static_cast<uint8_t>(DbgSpecial::TERM)) ||
      (ch == static_cast<uint8_t>(DbgSpecial::ESC))) {
    char tmp[2];
    tmp[0] = static_cast<char>(DbgSpecial::ESC);
    tmp[1] = ch;
    Hal.debugWrite(tmp, 2);
  }

  else {
    char tmp = ch;
    Hal.debugWrite(&tmp, 1);
  }

  // If there's more response to send, return true
  if (buffNdx < respLen)
    return true;

  // If that was the last byte in my response, send the
  // terminitaion character and start waiting on the next
  // command.
  char tmp = static_cast<char>(DbgSpecial::TERM);
  Hal.debugWrite(&tmp, 1);

  pollState = DbgPollState::WAIT_CMD;
  buffNdx = 0;
  return false;
}

// Process the received command
void DebugSerial::ProcessCmd() {
  // The total number of bytes received (not including
  // the termination byte) is the value of buffNdx.
  // This should be at least 3 (command & checksum).
  // If its not, I just ignore the command and jump
  // to waiting on the next.
  // This means we can send TERM characters to synchronize
  // communications if necessary
  if (buffNdx < 3) {
    buffNdx = 0;
    pollState = DbgPollState::WAIT_CMD;
    return;
  }

  uint16_t crc = CalcCRC(cmdInBuff, buffNdx - 2);

  if (crc != u8_to_u16(&cmdInBuff[buffNdx - 2])) {
    SendError(DbgErrCode::CRC_ERR);
    return;
  }

  DebugCmd *cmd = DebugCmd::cmdList[cmdInBuff[0]];
  if (!cmd) {
    SendError(DbgErrCode::BAD_CMD);
    return;
  }

  // The length that we pass in to the command handler doesn't
  // include the command code or CRC.  The max size is also reduced
  // by 3 to make sure we can add the error code and CRC.
  int len_in = buffNdx - 3;
  int len_out = 0;
  CmdContext context = {.req = &cmdInBuff[1],
                        .req_len = len_in,
                        .resp = &cmdOutBuff[1],
                        .max_resp_len =
                            static_cast<int>(sizeof(cmdOutBuff) - 3),
                        .resp_len = 0};
  DbgErrCode err = cmd->HandleCmd(&context);
  if (err != DbgErrCode::OK) {
    SendError(err);
    return;
  }

  len_out = context.resp_len;

  cmdOutBuff[0] = static_cast<uint8_t>(DbgErrCode::OK);

  // Calculate the CRC on the data and error code returned
  // and append this to the end of the response
  crc = CalcCRC(cmdOutBuff, len_out + 1);
  u16_to_u8(crc, &cmdOutBuff[len_out + 1]);

  pollState = DbgPollState::SEND_RESP;
  buffNdx = 0;
  respLen = len_out + 3;
}

void DebugSerial::SendError(DbgErrCode err) {
  cmdOutBuff[0] = static_cast<uint8_t>(err);
  uint16_t crc = CalcCRC(cmdOutBuff, 1);
  u16_to_u8(crc, &cmdOutBuff[1]);
  pollState = DbgPollState::SEND_RESP;
  buffNdx = 0;
  respLen = 3;
}

// 16-bit CRC calculation for debug commands and responses
uint16_t DebugSerial::CalcCRC(uint8_t *buff, int len) {
  const uint16_t CRC16POLY = 0xA001;
  static bool init = false;
  static uint16_t tbl[256];

  // The first time this is called I'll build a table
  // to speed up CRC handling
  if (!init) {
    init = true;
    for (uint16_t i = 0; i < 256; i++) {
      uint16_t crc = i;

      for (int j = 0; j < 8; j++) {
        int lsbSet = (crc & 1) == 1;
        crc = static_cast<uint16_t>(crc >> 1);
        if (lsbSet)
          crc ^= CRC16POLY;
      }

      tbl[i] = crc;
    }
  }

  uint16_t crc = 0;

  for (int i = 0; i < len; i++) {
    uint16_t tmp = tbl[0xFF & (buff[i] ^ crc)];

    crc = static_cast<uint16_t>(tmp ^ (crc >> 8));
  }
  return crc;
}

DebugCmd::DebugCmd(DbgCmdCode opcode) {
  cmdList[static_cast<uint8_t>(opcode)] = this;
}
