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

#ifndef DEBUG_H
#define DEBUG_H

#include "circular_buffer.h"
#include "trace.h"
#include <optional>
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
  kInternalError = 0x05,   // Some type of interal error (aka bug)
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

// Each debug command is represented by an instance of this
// virtual class
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

// Singleton class which implements the debug serial port handler.
class SerialHandler {
public:
  SerialHandler();

  // This function is called from the main loop to handle debug commands.
  // Returns true if this call has finished sending the response for a command,
  // which is useful for testing.
  bool Poll();

  // Printf style function to print data to a virtual console.
  size_t Print(const char *formatting_str, ...);

  // Read a byte from the print buffer.  Only intended to be called from the
  // command that returns print buffer data.
  std::optional<uint8_t> GetNextCharFromBuffer() { return print_buffer_.Get(); }

  static uint16_t ComputeCRC(const uint8_t *buffer, size_t length);

  void SampleTraceVars() { trace_->MaybeSample(); }

private:
  State state_{State::kWait};

  // Buffer into which request data is written in kWait state
  uint8_t request_[500] = {0};
  uint32_t request_size_{0};
  // Remember when we recieve an escape char (in case the call happens in
  // between the escape char and the special char)
  bool escape_next_byte_{false};

  // Buffer into which the command handler writes its response and which is then
  // sent in kResponding state.
  uint8_t response_[500] = {0};
  uint32_t response_size_{0};
  uint32_t response_bytes_sent_{0};

  // List of registered command handlers
  Command::Handler *registry_[256] = {nullptr};

  // Trace buffer (populated when SampleTraceVars is called and Trace is
  // enabled through the trace command)
  Trace *trace_;

  // Print buffer for console mode (populated when Print method is called)
  CircularBuffer<uint8_t, 2000> print_buffer_;

  bool ReadNextByte();
  void ProcessCmd();
  bool SendNextByte();

  void SendError(ErrorCode error);
};

// Some simple data conversion functions.
// These assume little endian byte format
inline uint16_t u8_to_u16(const uint8_t *dat) {
  uint16_t L = dat[0];
  uint16_t H = dat[1];
  return static_cast<uint16_t>(L | (H << 8));
}

inline uint32_t u8_to_u32(const uint8_t *dat) {
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

} // namespace Debug

extern Debug::SerialHandler debug;

#endif // DEBUG_H
