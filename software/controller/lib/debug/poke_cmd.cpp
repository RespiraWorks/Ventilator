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

ErrorCode PokeHandler::Process(Context *context) {

  // Total command length must be at least 5.  That's
  // four for the address and at least one data byte.
  if (context->request_length < 5)
    return ErrorCode::kMissingData;

  size_t address = address_msw_ + u8_to_u32(&context->request[0]);
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
  *(context->processed) = true;
  return ErrorCode::kNone;
}

} // namespace Debug::Command
