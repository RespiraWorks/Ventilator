
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
 * This file implements a command that allows the contents of the
 * print buffer data to be read.  The print buffer buffer is
 * where strings go when you call debug.Print
 */

#include "debug.h"
#include <optional>
#include <string.h>

class PrintBuffReadCmd : public DebugCmd {
public:
  PrintBuffReadCmd() : DebugCmd(DbgCmdCode::PRINT_BUFF_READ) {}

  DbgErrCode HandleCmd(uint8_t *data, int *len, int max) {
    // No data needs to be passed in to this command.

    // Read bytes from the print buffer until I hit my
    // max or the buffer is empty.

    int i;
    for (i = 0; i < max; i++) {
      std::optional<uint8_t> ch = debug.PrintBuffGet();
      if (ch == std::nullopt)
        break;
      *data++ = *ch;
    }

    *len = i;
    return DbgErrCode::OK;
  }
};

extern PrintBuffReadCmd pbReadCmd;
