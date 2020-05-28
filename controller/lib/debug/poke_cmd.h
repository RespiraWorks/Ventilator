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
 * This file implements a 'poke' command which allows us
 * to write to wrap values in memory.  Use with caution!
 *
 * The data passed to the command consists of a 32-bit
 * starting address and one or more data bytes to be
 * written to consecutive addresses starting at the
 * given address
 */

#ifndef POKE_CMD_H_
#define POKE_CMD_H_

#include "debug.h"
#include <string.h>

class PokeCmd : public DebugCmd {
public:
  PokeCmd() : DebugCmd(DbgCmdCode::POKE) {}
  DbgErrCode HandleCmd(CmdContext *context) override {

    // Total command length must be at least 5.  That's
    // four for the address and at least one data byte.
    if (context->req_len < 5)
      return DbgErrCode::MISSING_DATA;

    uint32_t addr = u8_to_u32(&context->req[0]);

    int ct = context->req_len - 4;

    // If both the address and count are multiples of 4, I write
    // 32-bit values.  This is useful when poking into registers
    // that need to be written as longs.
    if (!(addr & 3) && !(ct & 3)) {
      uint32_t *ptr = reinterpret_cast<uint32_t *>(addr);
      ct /= 4;
      for (int i = 0; i < ct; i++)
        *ptr++ = u8_to_u32(&context->req[4 + i * 4]);
    }

    // Same idea for multiples of 2
    else if (!(addr & 1) && !(ct & 1)) {
      uint16_t *ptr = reinterpret_cast<uint16_t *>(addr);
      ct /= 2;
      for (int i = 0; i < ct; i++)
        *ptr++ = u8_to_u16(&context->req[4 + i * 2]);
    }

    else {
      uint8_t *ptr = reinterpret_cast<uint8_t *>(addr);
      for (int i = 0; i < ct; i++)
        *ptr++ = context->req[4 + i];
    }

    context->resp_len = 0;
    return DbgErrCode::OK;
  }
};

extern PokeCmd pokeCmd;

#endif // POKE_CMD_H_
