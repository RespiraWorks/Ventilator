
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

#include "trace_cmd.h"
#include "debug.h"
#include "trace.h"

TraceCmd::TraceCmd() : DebugCmd(DbgCmdCode::TRACE) {
  // Disable all trace varaibles initially
  for (int i = 0; i < TRACE_VAR_CT; i++)
    traceVar[i] = -1;
}

// The trace command is used to download data from the trace buffer.
DbgErrCode TraceCmd::HandleCmd(uint8_t *data, int *len, int max) {

  // The first byte of data is always required, this
  // gives the sub-command.
  if (*len < 1)
    return DbgErrCode::MISSING_DATA;

  switch (data[0]) {

  // Sub-command 0 is used to flush the trace buffer
  // It also disables the trace
  case 0: {
    traceCtrl = 0;
    traceSamp = 0;

    std::optional<uint32_t> tmp;
    while ((tmp = traceBuffer.Get()) != std::nullopt) {
    }
    *len = 0;
    return DbgErrCode::OK;
  }

  // Sub-command 1 is used to read data from the buffer
  case 1:
    return ReadTraceBuff(data, len, max);

  default:
    return DbgErrCode::RANGE;
  }
}

DbgErrCode TraceCmd::ReadTraceBuff(uint8_t *data, int *len, int max) {
  // See how many active trace variables there are
  // This gives us our sample size;
  DebugVar *vptr[TRACE_VAR_CT];
  int vct = CountActiveVars(vptr);

  // If there aren't any active variables, I'm done
  if (!vct) {
    *len = 0;
    return DbgErrCode::OK;
  }

  // See how many samples I can return
  // First, find out how many I could based on the max value
  max /= static_cast<int>((vct * sizeof(uint32_t)));

  // If there's not room for even one sample, return an error.
  // That really shouldn't happen
  if (!max)
    return DbgErrCode::NO_MEMORY;

  // Find the total number of samples in the buffer
  int tot = traceBuffer.FullCt() / vct;
  if (tot > max)
    tot = max;

  for (int i = 0; i < tot; i++) {
    BlockInterrupts block;
    // Grab one sample with interrupts disabled.
    // There's a chance the trace is still running, so we
    // could get interrupted by the high priority thread
    // that adds to the buffer.  I want to make sure I
    // read a full sample without being interrupted
    for (int j = 0; j < vct; j++) {

      // This shouldn't fail since I've already confirmed
      // the number of elements in the buffer.  If it does
      // fail it's a bug.
      std::optional<uint32_t> dat = traceBuffer.Get();
      u32_to_u8(*dat, data);
      data += sizeof(uint32_t);
    }
    traceSamp--;
  }

  *len = static_cast<int>(tot * vct * sizeof(uint32_t));
  return DbgErrCode::OK;
}

TraceCmd traceCmd;
