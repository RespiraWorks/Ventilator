
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

// These variables are used control the trace function
static FnDebugVar varTraceCtrl(
    VarType::UINT32, "trace_ctrl", "Used to start/stop the trace function",
    "0x%08x", [] { return trace.GetFlags(); },
    [](uint32_t value) { trace.SetFlags(value); });

static FnDebugVar varTracePeriod(
    VarType::UINT32, "trace_period",
    "Period that data will be captured.  Loop cycle units", "%u",
    [] { return trace.GetPeriod(); },
    [](uint32_t value) { trace.SetPeriod(value); });

static FnDebugVar varTraceSamp(
    VarType::UINT32, "trace_samples", "Number of trace samples saved so far",
    "%u", [] { return trace.GetNumSamples(); },
    [](uint32_t value) { /* ignore */ });

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

DbgErrCode TraceCmd::HandleCmd(CmdContext *context) {

  // The first byte of data is always required, this
  // gives the sub-command.
  if (context->req_len < 1)
    return DbgErrCode::MISSING_DATA;

  switch (context->req[0]) {

  // Sub-command 0 is used to flush the trace buffer
  // It also disables the trace
  case 0: {
    trace.SetFlags(0);
    context->resp_len = 0;
    return DbgErrCode::OK;
  }

  // Sub-command 1 is used to read data from the buffer
  case 1:
    return ReadTraceBuff(context);

  default:
    return DbgErrCode::RANGE;
  }
}

DbgErrCode TraceCmd::ReadTraceBuff(CmdContext *context) {
  // See how many active trace variables there are
  // This gives us our sample size;
  int vct = trace.GetNumActiveVars();

  // If there aren't any active variables, I'm done
  if (!vct) {
    context->resp_len = 0;
    return DbgErrCode::OK;
  }

  // See how many samples I can return
  // First, find out how many I could based on the max value
  int max = context->max_resp_len / static_cast<int>((vct * sizeof(uint32_t)));

  // If there's not room for even one sample, return an error.
  // That really shouldn't happen
  if (!max)
    return DbgErrCode::NO_MEMORY;

  // Find the total number of samples in the buffer
  int tot = trace.GetNumSamples();
  if (tot > max)
    tot = max;

  uint8_t *resp = context->resp;
  std::array<uint32_t, TRACE_VAR_CT> record;
  for (int i = 0; i < tot; i++) {
    // This shouldn't fail since I've already confirmed
    // the number of elements in the buffer.  If it does
    // fail it's a bug.
    if (!trace.GetNextTraceRecord(&record, &vct))
      break;
    for (int j = 0; j < vct; j++) {
      u32_to_u8(record[j], resp);
      resp += sizeof(uint32_t);
    }
  }

  context->resp_len = static_cast<int>(tot * vct * sizeof(uint32_t));
  return DbgErrCode::OK;
}

TraceCmd traceCmd;
