/* Copyright 2020-2022, RespiraWorks

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

#include <array>
#include <optional>

#include "debug_types.h"
#include "uart_base.h"
#include "units.h"

/// \TODO Either template the Interface class and/or have it take a truly abstract Channel. This
///   class expects it to have Read, TxFree and Write HAL expects it to have UARTInterruptHandler

namespace Debug {

// The debug serial port interface implements a very simple binary command
// structure.  Commands are sent using the following format:
//
//   <cmd> <data> <crc> <term>
//
// <cmd> is a single byte command code.
//
// <data> is zero or more bytes of command data.  The meaning of the data
// is dependent on the command code.
//
// <crc> is a 16-bit CRC on the command and data bytes sent LSB first
//
// The response to a command has a similar structure:
//   <err> <data> <crc> <term>
//
// <err> is an error code (0 for success)
//
// <data> is any data returned from the command.  If there's an error
// there is never any data
//
// <term> is a special character value indicating the end of a command.
//
// Two special char values are used (see debug_types.h) :
// - EndTransfer (0xF2), aka term - Signifies the end of a command.
// - Escape  (0xF1) - used to send a special character as data.
//
// The escape byte causes the serial processor to treat the next byte as
// data no matter what its value is.  It's used when the data being sent
// has a special value.
class Interface {
 public:
  explicit Interface(UART::Channel *uart);

  void add_handler(Command::Code code, Command::Handler *handler);

  // This function is called from the main loop to handle debug commands.
  // Returns true if this call has finished sending the response for a command,
  // which is useful for testing.
  bool Poll();

  static uint16_t ComputeCRC(const uint8_t *buffer, size_t length);

 private:
  State state_{State::AwaitingCommand};

  // Minimum debug message size is 3 bytes: 1 byte gives the command (respectively
  // the error code for responses), and 2 bytes are used for checksum.
  static constexpr size_t MinFrameSize{3};
  static constexpr size_t BufferSize{500};

  // Buffer into which request data is written in AwaitingCommand state
  std::array<uint8_t, BufferSize> request_ = {0};
  size_t request_size_{0};
  // Remember when we receive an escape char (in case the call happens in
  // between the escape char and the special char)
  bool escape_next_byte_{false};

  // Buffer into which the command handler writes its response and which is then
  // sent in Responding state.
  std::array<uint8_t, BufferSize> response_ = {0};
  size_t response_size_{0};
  size_t response_bytes_sent_{0};

  // Some commands take time to be fully processed, we record their start time
  // and status
  Time command_start_time_{microsSinceStartup(0)};
  bool command_processed_{true};
  size_t response_length_{0};

  // List of registered command handlers
  /// \TODO size of registry should be determined by range of Code enum
  Command::Handler *registry_[32] = {nullptr};

  // Uart channel on which data is sent
  UART::Channel *uart_;

  bool ReadNextByte();
  void ProcessCommand();
  bool SendNextByte();

  void SendResponse(ErrorCode error, size_t response_length);
  void SendError(ErrorCode error) { SendResponse(error, 0); }
};

}  // namespace Debug
