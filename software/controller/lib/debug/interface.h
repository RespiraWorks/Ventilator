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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "circular_buffer.h"
#include "debug_types.h"
#include "trace.h"
#include <optional>

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
// - kEndTransfer (0xF2), aka term - Signifies the end of a command.
// - kEscape  (0xF1) - used to send a special character as data.
//
// The escape byte causes the serial processor to treat the next byte as
// data no matter what its value is.  It's used when the data being sent
// has a special value.
class Interface {
public:
  Interface();

  // This function is called from the main loop to handle debug commands.
  // Returns true if this call has finished sending the response for a command,
  // which is useful for testing.
  bool Poll();

  static uint16_t ComputeCRC(const uint8_t *buffer, size_t length);

  void SampleTraceVars() { trace_->MaybeSample(); }

private:
  State state_{State::kWait};

  // Buffer into which request data is written in kWait state
  uint8_t request_[500] = {0};
  uint32_t request_size_{0};
  // Remember when we receive an escape char (in case the call happens in
  // between the escape char and the special char)
  bool escape_next_byte_{false};

  // Buffer into which the command handler writes its response and which is then
  // sent in kResponding state.
  uint8_t response_[500] = {0};
  uint32_t response_size_{0};
  uint32_t response_bytes_sent_{0};

  // List of registered command handlers
  Command::Handler *registry_[32] = {nullptr};

  // Trace buffer (populated when SampleTraceVars is called and Trace is
  // enabled through the trace command)
  Trace *trace_;

  bool ReadNextByte();
  void ProcessCommand();
  bool SendNextByte();

  void SendResponse(ErrorCode error, uint32_t response_length);
  void SendError(ErrorCode error) { SendResponse(error, 0); }
};

} // namespace Debug

#endif // INTERFACE_H
