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
enum class State { kWait, kProcessing, kResponding };

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
};

namespace Command {

enum class Code : uint8_t {
  kMode = 0x00,     // Return the current firmware mode
  kPeek = 0x01,     // Peek into RAM
  kPoke = 0x02,     // Poke values into RAM
  kConsole = 0x03,  // Read strings from the print buffer
  kVariable = 0x04, // Variable access
  kTrace = 0x05,    // Data trace commands
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

} // namespace Command

} // namespace Debug

#endif // DEBUG_TYPES_H
