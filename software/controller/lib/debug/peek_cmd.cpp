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

ErrorCode PeekHandler::Process(Context *context) {
  // The data passed to this command consists of a 4 byte address
  // and a two byte count of how many bytes to return
  // Length should be exactly 6, but if more data is passed
  // I'll just ignore it.
  if (context->request_length < 6)
    return ErrorCode::kMissingData;

  size_t address = address_msw_ + u8_to_u32(&context->request[0]);
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
  *(context->processed) = true;
  return ErrorCode::kNone;
}

} // namespace Debug::Command
