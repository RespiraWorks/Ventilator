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
 * Two special char values are used:
 * - kEndTransfer (0xF2), aka term - Signifies the end of a command.
 * - kEscape  (0xF1) - used to send a special character as data.
 *
 * The escape byte causes the serial processor to treat the next byte as
 * data no matter what its value is.  It's used when the data being sent
 * has a special value.
 *
 */

#include "debug.h"
#include "commands.h"
#include "hal.h"
#include "sprintf.h"
#include <stdarg.h>
#include <string.h>

// Create a trace buffer that the Debug Handler uses to handle the trace
// command.
Debug::Trace trace;

// These variables are used to control the trace function
static FnDebugVar varTraceCtrl(
    VarType::UINT32, "trace_ctrl", "Used to start/stop the trace function",
    "0x%08x", [] { return static_cast<uint32_t>(trace.GetStatus()); },
    [](uint32_t value) {
      if (value == 1)
        trace.Start();
      else
        trace.Stop();
    });

static FnDebugVar varTracePeriod(
    VarType::UINT32, "trace_period",
    "Period that data will be captured.  Loop cycle units", "%u",
    [] { return trace.GetPeriod(); },
    [](uint32_t value) { trace.SetPeriod(value); });

static FnDebugVar varTraceSamp(
    VarType::UINT32, "trace_samples", "Number of trace samples saved so far",
    "%u", [] { return trace.GetNumSamples(); },
    [](uint32_t value) { /*ignored*/ });

static FnDebugVar vartrace_var1(
    VarType::INT32, "trace_var1", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<0>(); },
    [](int32_t value) { trace.SetTracedVarId<0>(value); });
static FnDebugVar vartrace_var2(
    VarType::INT32, "trace_var2", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<1>(); },
    [](int32_t value) { trace.SetTracedVarId<1>(value); });
static FnDebugVar vartrace_var3(
    VarType::INT32, "trace_var3", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<2>(); },
    [](int32_t value) { trace.SetTracedVarId<2>(value); });
static FnDebugVar vartrace_var4(
    VarType::INT32, "trace_var4", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<3>(); },
    [](int32_t value) { trace.SetTracedVarId<3>(value); });

// Create a handler for each of the known commands that the Debug Handler can
// link to.  This is a bit tedious but I can't find a simpler way.
Debug::Command::ModeHandler mode_command;
Debug::Command::PeekHandler peek_command;
Debug::Command::PokeHandler poke_command;
Debug::Command::ConsoleHandler console_command;
Debug::Command::VarHandler var_command;
Debug::Command::TraceHandler trace_command(&trace);

// Create the global debug handler
Debug::SerialHandler debug;

namespace Debug {

SerialHandler::SerialHandler() {
  // TODO - This is annoying. Could we make the Command Handler automatically
  // add themselves to this registry?
  registry_[static_cast<uint8_t>(Command::Code::kMode)] = &mode_command;
  registry_[static_cast<uint8_t>(Command::Code::kPeek)] = &peek_command;
  registry_[static_cast<uint8_t>(Command::Code::kPoke)] = &poke_command;
  registry_[static_cast<uint8_t>(Command::Code::kConsole)] = &console_command;
  registry_[static_cast<uint8_t>(Command::Code::kVariable)] = &var_command;
  registry_[static_cast<uint8_t>(Command::Code::kTrace)] = &trace_command;

  trace_ = &trace;
}

// This function is called from the main low priority background loop.
// Its a simple state machine that waits for a new command to be received
// over the debug serial port.  Process the command when one is received
// and sends the response back.
bool SerialHandler::Poll() {
  switch (state_) {
  // Waiting for a new command to be received.
  // I continue to process bytes until there are no more available,
  // or a full command has been received.  Either way, the
  // ReadNextByte function will return false when its time
  // to move on.
  case State::kWait:
    while (ReadNextByte()) {
    }
    return false;

  // Process the current command
  case State::kProcessing:
    ProcessCmd();
    request_size_ = 0;
    return false;

  // Send my response
  case State::kResponding:
    while (SendNextByte()) {
    }
    return true;
  }
  return false;
}

// Format a debug string printf sytle and save it to a local buffer
// which can be queried by the interface program.
//
// Returns the number of bytes actually written to the print buffer
size_t SerialHandler::Print(const char *formatting_str, ...) {
  char local_buffer[300];

  // Note that this uses a local sprintf implementation because
  // the one from the standard libraries will potentially dynamically
  // allocate memory.
  va_list values;
  va_start(values, formatting_str);
  size_t length =
      RWvsnprintf(local_buffer, sizeof(local_buffer), formatting_str, values);
  va_end(values);

  // Write as much as will fit to my print buffer.
  for (size_t count = 0; count < length; count++) {
    if (!print_buffer_.Put(local_buffer[count]))
      return count;
  }

  return length;
}

// Read the next byte from the debug serial port.
// Returns false if there were no more bytes available.
// Also returns false if a full command has been received.
bool SerialHandler::ReadNextByte() {
  // Get the next byte from the debug serial port if there is one.
  char next_char;
  if (Hal.debugRead(&next_char, 1) < 1)
    return false;

  uint8_t byte = static_cast<uint8_t>(next_char);

  // If the previous character received was an escape character
  // then just save this byte (assuming there's space)
  if (escape_next_byte_) {
    escape_next_byte_ = false;

    if (request_size_ < std::size(request_))
      request_[request_size_++] = byte;
    return true;
  }

  // If this is an escape character, don't save it just keep track
  // of the fact that we saw it.
  if (byte == static_cast<uint8_t>(SpecialChar::kEscape)) {
    escape_next_byte_ = true;
    return true;
  }

  // If this is an termination character, then change our state and return false
  if (byte == static_cast<uint8_t>(SpecialChar::kEndTransfer)) {
    state_ = State::kProcessing;
    return false;
  }

  // For other characters, just save them if there's space in the buffer
  if (request_size_ < std::size(request_))
    request_[request_size_++] = byte;
  return true;
}

// Send the next byte of my response to the last command.
// Returns false if no more data will fit in the output buffer,
// or if the entire response has been sent.
bool SerialHandler::SendNextByte() {

  // To simplify things below, I require at least 3 bytes
  // in the output buffer to continue
  if (Hal.debugBytesAvailableForWrite() < 3)
    return false;

  // See what the next character to send is.
  char next_char = response_[response_bytes_sent_++];

  // If its a special character, I need to escape it.
  if ((next_char == static_cast<char>(SpecialChar::kEndTransfer)) ||
      (next_char == static_cast<char>(SpecialChar::kEscape))) {
    char escaped_char[2];
    escaped_char[0] = static_cast<char>(SpecialChar::kEscape);
    escaped_char[1] = next_char;
    (void)Hal.debugWrite(escaped_char, 2);
  } else {
    (void)Hal.debugWrite(&next_char, 1);
  }

  // If there's more response to send, return true
  if (response_bytes_sent_ < response_size_)
    return true;

  // If that was the last byte in my response, send the
  // termination character and start waiting on the next
  // command.
  char end_transfer = static_cast<char>(SpecialChar::kEndTransfer);
  (void)Hal.debugWrite(&end_transfer, 1);

  state_ = State::kWait;
  response_bytes_sent_ = 0;
  return false;
}

// Process the received command
void SerialHandler::ProcessCmd() {
  // The total number of bytes received (not including the termination byte)
  // is the value of request_size_, which should be at least 3 (8 bits command
  // code + 16 bits checksum). If its not, I just ignore the command and jump to
  // waiting for the next one.
  // This means we can send kEndTransfer characters to synchronize
  // communication if necessary
  if (request_size_ < 3) {
    request_size_ = 0;
    state_ = State::kWait;
    return;
  }

  uint16_t crc = ComputeCRC(request_, request_size_ - 2);

  if (crc != u8_to_u16(&request_[request_size_ - 2])) {
    SendError(ErrorCode::kCrcError);
    return;
  }

  Command::Handler *cmd_handler = registry_[request_[0]];
  if (!cmd_handler) {
    SendError(ErrorCode::kUnknownCommand);
    return;
  }

  // The length that we pass in to the command handler doesn't
  // include the command code or CRC.  The max size is also reduced
  // by 3 to make sure we can add the error code and CRC.
  Command::Context context = {
      .request = &request_[1],
      .request_length = request_size_ - 3,
      .response = &response_[1],
      .max_response_length = sizeof(response_) - 3,
      .response_length = 0,
  };
  ErrorCode error = cmd_handler->Process(&context);
  if (error != ErrorCode::kNone) {
    SendError(error);
    return;
  }

  response_[0] = static_cast<uint8_t>(ErrorCode::kNone);

  // Calculate the CRC on the data and error code returned
  // and append this to the end of the response
  crc = ComputeCRC(response_, context.response_length + 1);
  u16_to_u8(crc, &response_[context.response_length + 1]);

  state_ = State::kResponding;
  response_bytes_sent_ = 0;
  // The size of the response that will be sent includes the error code (1 byte)
  // and checksum (2 bytes).
  response_size_ = context.response_length + 3;
}

void SerialHandler::SendError(ErrorCode error) {
  response_[0] = static_cast<uint8_t>(error);
  uint16_t crc = ComputeCRC(response_, 1);
  u16_to_u8(crc, &response_[1]);
  state_ = State::kResponding;
  response_bytes_sent_ = 0;
  response_size_ = 3;
}

// 16-bit CRC calculation for debug commands and responses
uint16_t SerialHandler::ComputeCRC(const uint8_t *buffer, size_t length) {
  const uint16_t CRC16POLY = 0xA001;
  static bool init = false;
  static uint16_t crc_table[256];

  // The first time this is called I'll build a table
  // to speed up CRC handling
  if (!init) {
    init = true;
    for (uint16_t byte = 0; byte < 256; byte++) {
      uint16_t crc = byte;

      for (uint8_t bit_number = 0; bit_number < 8; bit_number++) {
        bool lsb = (crc & 1) == 1;
        crc = static_cast<uint16_t>(crc >> 1);
        if (lsb)
          crc ^= CRC16POLY;
      }

      crc_table[byte] = crc;
    }
  }

  uint16_t crc = 0;

  for (uint32_t byte_number = 0; byte_number < length; byte_number++) {
    uint16_t local_crc = crc_table[0xFF & (buffer[byte_number] ^ crc)];

    crc = static_cast<uint16_t>(local_crc ^ (crc >> 8));
  }
  return crc;
}

} // namespace Debug
