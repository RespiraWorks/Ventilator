/* Copyright 2020, RespiraWorks

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
 * This file implements a 'peek' command which allows us
 * to read raw memory values.
 *
 * The data passed to the command consists of a 32-bit
 * starting address and an 16-bit byte count.
 */

#ifndef PEEK_CMD_H_
#define PEEK_CMD_H_

#include "debug.h"
#include <string.h>

class PeekCmd : public DebugCmd {
public:
  PeekCmd() : DebugCmd(DbgCmdCode::PEEK) {}
  DbgErrCode HandleCmd(CmdContext *context) override {
    // The data passed to this command consists of a 4 byte address
    // and a two byte count of how many bytes to return
    // Length should be exactly 6, but if more data is passed
    // I'll just ignore it.
    if (context->req_len < 6)
      return DbgErrCode::MISSING_DATA;

    uint32_t addr = u8_to_u32(&context->req[0]);
    int ct = u8_to_u16(&context->req[4]);

    // Limit the number of output bytes based on buffer size
    if (ct > context->max_resp_len)
      ct = context->max_resp_len;

    // Some registers can't handle byte accesses, so rather then just
    // use a simple memcpy here I will do 32-bit or 16-bit accesses
    // as long as both the address and count are aligned to 32 or 16 bits.
    if (!(addr & 3) && !(ct & 3)) {
      uint32_t *ptr = reinterpret_cast<uint32_t *>(addr);

      for (int i = 0; i < ct / 4; i++) {
        uint32_t x = *ptr++;
        u32_to_u8(x, &context->resp[4 * i]);
      }
    } else if (!(addr & 1) && !(ct & 1)) {
      uint16_t *ptr = reinterpret_cast<uint16_t *>(addr);
      for (int i = 0; i < ct / 2; i++) {
        uint16_t x = *ptr++;
        u16_to_u8(x, &context->resp[2 * i]);
      }
    } else
      memcpy(context->resp, reinterpret_cast<void *>(addr), ct);

    context->resp_len = ct;
    return DbgErrCode::OK;
  }
};

extern PeekCmd peekCmd;

#endif // PEEK_CMD_H_
