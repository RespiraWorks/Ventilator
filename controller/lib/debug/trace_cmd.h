
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

#ifndef TRACE_CMD_H
#define TRACE_CMD_H

#include "debug.h"

class TraceCmd : public DebugCmd {
public:
  TraceCmd();
  // The trace command is used to download data from the trace buffer.
  DbgErrCode HandleCmd(uint8_t *data, int *len, int max);
  DbgErrCode ReadTraceBuff(uint8_t *data, int *len, int max);
};

extern TraceCmd traceCmd;

#endif // TRACE_CMD_H_
