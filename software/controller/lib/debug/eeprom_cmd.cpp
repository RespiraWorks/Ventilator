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
#include "commands.h"

namespace Debug::Command {

ErrorCode EepromHandler::Process(Context *context) {

  // We have at least subcommand and address (3 bytes)
  if (context->request_length < 3)
    return ErrorCode::kMissingData;

  // Whatever the subcommand, bytes 1 and 2 are the address
  uint16_t address = u8_to_u16(&context->request[1]);

  Subcommand subcommand{context->request[0]};

  // Process subcommand
  switch (subcommand) {
  case Subcommand::kRead:
    return Read(address, context);

  case Subcommand::kWrite:
    return Write(address, context);

  default:
    return ErrorCode::kInvalidData;
  }
}

ErrorCode EepromHandler::Read(const uint16_t address, Context *context) {
  // Read command requires length (bytes 3 and 4)
  if (context->request_length < 5)
    return ErrorCode::kMissingData;
  uint16_t length = u8_to_u16(&context->request[3]);
  if (length > context->max_response_length)
    return ErrorCode::kNoMemory;

  if (eeprom_->ReadBytes(address, length, context->response,
                         context->processed)) {
    // only set response_length if the read has been successfully sent
    context->response_length = length;
  }
  return ErrorCode::kNone;
}

ErrorCode EepromHandler::Write(const uint16_t address, Context *context) {
  // at least one byte of data is given after the address
  if (context->request_length < 4)
    return ErrorCode::kMissingData;

  // length of data to be written is the length of the request minus
  // the subcommand and address bytes
  uint16_t length = static_cast<uint16_t>(context->request_length - 3);

  if (length > kMaxWriteLength)
    return ErrorCode::kNoMemory;

  // copy request data in our own array to allow eeprom to reinterpret_cast
  // the pointer we pass it (as pointers to const cannot be cast)
  uint8_t request[kMaxWriteLength] = {0};
  memcpy(&request[0], &(context->request[3]), length);
  context->response_length = 0;
  if (eeprom_->WriteBytes(address, length, &request, context->processed)) {
    return ErrorCode::kNone;
  } else {
    // could not send write request for some reason
    return ErrorCode::kInternalError;
  }
}

} // namespace Debug::Command
