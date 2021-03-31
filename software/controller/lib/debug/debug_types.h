/* Copyright 2020-2021, RespiraWorks

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

#ifndef DEBUG_TYPES_H
#define DEBUG_TYPES_H

#include <stdint.h>

namespace Debug {

// States for the internal state machine
enum class State {
  kAwaitingCommand,
  kProcessing,
  kAwaitingResponse,
  kResponding
};

// The binary serial interface uses two special characters
// These values are pretty arbitrary.
// See debug.cpp for a detailed description of how this works
enum class SpecialChar : uint8_t { kEscape = 0xf1, kEndTransfer = 0xf2 };

enum class ErrorCode : uint8_t {
  kNone = 0x00,            // No error (=success)
  kCrcError = 0x01,        // CRC error on command
  kUnknownCommand = 0x02,  // Unknown command code received
  kMissingData = 0x03,     // Not enough data passed with command
  kNoMemory = 0x04,        // Insufficient memory
  kInternalError = 0x05,   // Some type of internal error (aka bug)
  kUnknownVariable = 0x06, // The requested variable ID is invalid
  kInvalidData = 0x07,     // data is out of range
  kTimeout = 0x08,         // response timeout
};

namespace Command {

enum class Code : uint8_t {
  kMode = 0x00,         // Return the current firmware mode
  kPeek = 0x01,         // Peek into RAM
  kPoke = 0x02,         // Poke values into RAM
  kConsole = 0x03,      // Read strings from the print buffer - deprecated
  kVariable = 0x04,     // Variable access
  kTrace = 0x05,        // Data trace commands
  kEepromAccess = 0x06, // Read/Write in I2C EEPROM
};

// Structure that represents a command's parameters
struct Context {
  const uint8_t *request;             // pointer to the request (command's data)
  const uint32_t request_length;      // length of the request
  uint8_t *response;                  // pointer to the response (that will be
                                      // written by the command handler)
  const uint32_t max_response_length; // maximum length the command handler
                                      // can use (enforces response limits)
  uint32_t response_length; // (actual) length of the response once the
                            // command is processed
  bool *processed; // pointer to a boolean that informs the interface handler
                   // that a command's response is available (some commands take
                   // time)
};

// Each debug command is represented by an instance of this virtual class
class Handler {
public:
  Handler() = default;

  // Returns an error code.  For any non-zero error, the values returned in
  // response_length and response will be ignored.
  [[nodiscard]] virtual ErrorCode Process(Context *context) {
    return ErrorCode::kUnknownCommand;
  }
};

enum class Subcommand {
  kFlushTrace = 0x00,    // disable and flush the trace
  kDownloadTrace = 0x01, // download the trace buffer
  kVarInfo = 0x00,       // get variable info (name, type, help string)
  kGetVar = 0x01,        // get variable value
  kSetVar = 0x02,        // set variable value
  kEepromRead = 0x00,    // read value in EEPROM
  kEepromWrite = 0x01,   // write value in EEPROM
};

} // namespace Command

} // namespace Debug

#endif // DEBUG_TYPES_H
