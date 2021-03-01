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

#ifndef COMMAND_H
#define COMMAND_H

#include "binary_utils.h"
#include "hal.h"
#include "interface.h"

namespace Debug::Command {

// Mode command.
// This returns a single byte of data which gives the firmware mode:
//  0 - Running in normal mode
//  1 - Running in boot mode.
//
// We don't actually have a boot mode yet, but its only a matter of time.
// Once we start doing things like updating firmware (not through a debugger),
// we will need a separate boot loader image to ensure graceful recovery.
class ModeHandler : public Handler {
public:
  ModeHandler() = default;
  ErrorCode Process(Context *context) override {
    context->response_length = 1;
    context->response[0] = 0;
    return ErrorCode::kNone;
  }
};

// Peek command.
// Allows us to read raw memory values.
//
// The data passed to the command consists of a 32-bit starting address and
// a 16-bit byte count.
class PeekHandler : public Handler {
public:
  PeekHandler() = default;
  ErrorCode Process(Context *context) override {
    // The data passed to this command consists of a 4 byte address
    // and a two byte count of how many bytes to return
    // Length should be exactly 6, but if more data is passed
    // I'll just ignore it.
    if (context->request_length < 6)
      return ErrorCode::kMissingData;

    uint32_t address = u8_to_u32(&context->request[0]);
    uint32_t count = u8_to_u16(&context->request[4]);

    // Limit the number of output bytes based on buffer size
    if (count > context->max_response_length)
      count = context->max_response_length;

    // Some registers can't handle byte accesses, so rather then just
    // use a simple memcpy here I will do 32-bit or 16-bit accesses
    // as long as both the address and count are aligned to 32 or 16 bits.
    if ((address & 1) || (count & 1)) {
      // no 16-bit alignment is possible
      memcpy(context->response, reinterpret_cast<void *>(address), count);
    } else if ((address & 3) || (count & 3)) {
      // aligned to 16 bits but not 32
      uint16_t *ptr = reinterpret_cast<uint16_t *>(address);
      for (int byte_number = 0; byte_number < count / 2; byte_number++) {
        uint16_t value = *ptr++;
        u16_to_u8(value, &context->response[2 * byte_number]);
      }
    } else {
      // aligned to 32 bits
      uint32_t *ptr = reinterpret_cast<uint32_t *>(address);

      for (int byte_number = 0; byte_number < count / 4; byte_number++) {
        uint32_t value = *ptr++;
        u32_to_u8(value, &context->response[4 * byte_number]);
      }
    }

    context->response_length = count;
    return ErrorCode::kNone;
  }
};

// Poke command.
// Allows us to write raw values in memory.  Use with caution!
// The data passed to the command consists of a 32-bit starting address and one
// or more data bytes to be written to consecutive addresses starting at the
// given address
class PokeHandler : public Handler {
public:
  PokeHandler() = default;
  ErrorCode Process(Context *context) override {

    // Total command length must be at least 5.  That's
    // four for the address and at least one data byte.
    if (context->request_length < 5)
      return ErrorCode::kMissingData;

    uint32_t address = u8_to_u32(&context->request[0]);

    uint32_t count = context->request_length - 4;

    // Some registers can't handle byte accesses, so rather then just
    // use a simple memcpy here I will do 32-bit or 16-bit accesses
    // as long as both the address and count are aligned to 32 or 16 bits.
    if ((address & 1) || (count & 1)) {
      // no 16-bit alignment is possible
      uint8_t *ptr = reinterpret_cast<uint8_t *>(address);
      for (int byte_number = 0; byte_number < count; byte_number++) {
        *ptr++ = context->request[4 + byte_number];
      }
    } else if ((address & 3) || (count & 3)) {
      // 16-bit alignment is possible but not 32 bits
      uint16_t *ptr = reinterpret_cast<uint16_t *>(address);
      count /= 2;
      for (int byte_number = 0; byte_number < count; byte_number++) {
        *ptr++ = u8_to_u16(&context->request[4 + byte_number * 2]);
      }
    } else {
      // aligned to 32 bits
      uint32_t *ptr = reinterpret_cast<uint32_t *>(address);
      count /= 4;
      for (int byte_number = 0; byte_number < count; byte_number++) {
        *ptr++ = u8_to_u32(&context->request[4 + byte_number * 4]);
      }
    }

    context->response_length = 0;
    return ErrorCode::kNone;
  }
};

// Trace command
// Used to download data from the trace buffer.
//
// Data passed to the command is a single byte which defines what the command
// does:
//  0 - Used to disable the trace and flush the trace buffer
//  1 - Used to read data from the buffer
class TraceHandler : public Handler {
public:
  explicit TraceHandler(Trace *trace) : trace_(trace){};

  ErrorCode Process(Context *context) override;
  ErrorCode ReadTraceBuffer(Context *context);

private:
  Trace *trace_{nullptr};
};

// Command handler for variable access
//
// The first byte of data passed to the command gives a sub-command
// which defines what the command does and the structure of it's data.
//
// Sub-commands:
//  0 - Used to read info about a variable.  The debug interface calls
//      this repeatedly on startup to enumerate the variables currently
//      supported by the code.  This way new debug variables can be added
//      on the fly without modifying the Python code to match.
//
//      Input data to this command is a 16-bit variable ID (assigned
//      dynamically as variables are created).  The output is info about
//      the variable.  See the code below for details of the output format
//
//  1 - Read the variables value.
//
//  2 - Set the variables value.
//
class VarHandler : public Handler {
public:
  VarHandler() = default;

  ErrorCode Process(Context *context) override;

  // Return info about one of the variables. The 16-bit variable ID is passed
  // in.  These IDs are automatically assigned as variables are registered in
  // the system starting with 0.
  // The Python code can read them all out until it gets an error code
  // indicating that the passed ID is invalid.
  ErrorCode GetVarInfo(Context *context);

  ErrorCode GetVar(Context *context);

  ErrorCode SetVar(Context *context);
};

} // namespace Debug::Command

#endif // COMMAND_H
