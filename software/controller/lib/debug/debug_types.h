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

#pragma once

#include <cstddef>
#include <cstdint>

namespace Debug {

// States for the internal state machine
enum class State { AwaitingCommand, Processing, AwaitingResponse, Responding };

// The binary serial interface uses two special characters
// These values are pretty arbitrary.
// See debug.cpp for a detailed description of how this works
enum class SpecialChar : uint8_t { Escape = 0xf1, EndTransfer = 0xf2 };

enum class ErrorCode : uint8_t {
  None = 0x00,             // No error (=success)
  CrcError = 0x01,         // CRC error on command
  UnknownCommand = 0x02,   // Unknown command code received
  MissingData = 0x03,      // Not enough data passed with command
  NoMemory = 0x04,         // Insufficient memory
  InternalError = 0x05,    // Some type of internal error (aka bug)
  UnknownVariable = 0x06,  // The requested variable ID is invalid
  InvalidData = 0x07,      // Invalid data in request
  Timeout = 0x08,          // Response timeout
};

namespace Command {

enum class Code : uint8_t {
  Mode = 0x00,          // Return the current firmware mode
  Peek = 0x01,          // Peek into RAM
  Poke = 0x02,          // Poke values into RAM
  Console = 0x03,       // Read strings from the print buffer - deprecated
  Variable = 0x04,      // Variable access
  Trace = 0x05,         // Data trace commands
  EepromAccess = 0x06,  // Read/Write in I2C EEPROM
};

// Structure that represents a command's parameters
struct Context {
  const uint8_t *request;            // pointer to the request (command's data)
  const size_t request_length;       // length of the request
  uint8_t *response;                 // pointer to the response (that will be
                                     // written by the command handler)
  const size_t max_response_length;  // maximum length the command handler
                                     // can use (enforces response limits)
  size_t response_length;            // (actual) length of the response once the
                                     // command is processed
  bool *processed;                   // pointer to a boolean that informs the interface handler
                                     // that a command's response is available (some commands
                                     // take time)
};

// Each debug command is represented by an instance of this virtual class
class Handler {
 public:
  Handler() = default;

  // Returns an error code.  For any non-zero error, the values returned in
  // response_length and response will be ignored.
  [[nodiscard]] virtual ErrorCode Process(Context *context) { return ErrorCode::UnknownCommand; }
};

}  // namespace Command

}  // namespace Debug
