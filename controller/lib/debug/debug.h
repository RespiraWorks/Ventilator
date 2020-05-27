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

#ifndef DEBUG_H
#define DEBUG_H

#include "circular_buffer.h"
#include <optional>
#include <stdint.h>

// States for the internal state machine
enum class DbgPollState { WAIT_CMD, PROCESS_CMD, SEND_RESP };

// The binary serial interface uses two special characters
// These values are pretty arbitrary.
// See debug.cpp for a detailed description of how this works
enum class DbgSpecial { ESC = 0xf1, TERM = 0xf2 };

enum class DbgCmdCode {
  MODE = 0x00,            // Return the current firmware mode
  PEEK = 0x01,            // Peek into RAM
  POKE = 0x02,            // Poke values into RAM
  PRINT_BUFF_READ = 0x03, // Read strings from the print buffer
  VAR = 0x04,             // Variable access
  TRACE = 0x05,           // Data trace commands
};

enum class DbgErrCode {
  OK = 0x00,           // success
  CRC_ERR = 0x01,      // CRC error on command
  BAD_CMD = 0x02,      // Unknown command code received
  MISSING_DATA = 0x03, // Not enough data passed with command
  NO_MEMORY = 0x04,    // Insufficient memory
  INTERNAL = 0x05,     // Some type of interal error (aka bug)
  BAD_VARID = 0x06,    // The requested variable ID is invalid
  RANGE = 0x07,        // data out of range
};

// Each debug command is represented by an instance of this
// virtual class
class DebugCmd {
  friend class DebugSerial;
  static DebugCmd *cmdList[256];

public:
  DebugCmd(DbgCmdCode opcode);

  // Command handler.
  //   data - Buffer holding command data on entry.
  //   The response data should be written here.
  //
  //   len - Holds the data length on entry and response
  //   length on return.
  //
  //   max - Maximum number of bytes that can be written to data
  //
  //   Returns an error code.  For any non-zero error, the values
  //   returned in len and data will be ignored.
  virtual DbgErrCode HandleCmd(uint8_t *data, int *len, int max) {
    return DbgErrCode::BAD_CMD;
  }
};

// Singleton class which implements the debug serial port handler.
class DebugSerial {
public:
  DebugSerial();

  // This function is called from the main loop to handle
  // debug commands
  void Poll();

  // Printf style function to print data to a virtual
  // console.
  int Print(const char *fmt, ...);

  // Read a byte from the print buffer.
  // This is only intended to be called from the command that returns
  // print buffer data.
  std::optional<uint8_t> PrintBuffGet() { return printBuff.Get(); }

private:
  CircBuff<uint8_t, 2000> printBuff;

  uint8_t cmdBuff[500];
  int buffNdx;
  int respLen;
  DbgPollState pollState;
  bool prevCharEsc;

  bool ReadNextByte();
  void ProcessCmd();
  bool SendNextByte();

  uint16_t CalcCRC(uint8_t *buff, int len);
  void SendError(DbgErrCode err);
};
extern DebugSerial debug;

// Some simple data conversion functions.
// These assume little endian byte format
inline uint16_t u8_to_u16(uint8_t *dat) {
  uint16_t L = dat[0];
  uint16_t H = dat[1];
  return static_cast<uint16_t>(L | (H << 8));
}

inline uint32_t u8_to_u32(uint8_t *dat) {
  uint32_t A = dat[0];
  uint32_t B = dat[1];
  uint32_t C = dat[2];
  uint32_t D = dat[3];
  return static_cast<uint32_t>(A | (B << 8) | (C << 16) | (D << 24));
}

inline void u16_to_u8(uint16_t val, uint8_t *buff) {
  buff[0] = static_cast<uint8_t>(val);
  buff[1] = static_cast<uint8_t>(val >> 8);
}

inline void u32_to_u8(uint32_t val, uint8_t *buff) {
  buff[0] = static_cast<uint8_t>(val);
  buff[1] = static_cast<uint8_t>(val >> 8);
  buff[2] = static_cast<uint8_t>(val >> 16);
  buff[3] = static_cast<uint8_t>(val >> 24);
}

#endif
