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
 * This file implements a data trace facility.
 *
 * The data trace is a feature which allows data to be written to a largish
 * RAM buffer in real time and read out by the debug inteface later.  The
 * data can then be graphed or otherwise manipulated.
 *
 * This is extremely useful for tuning control systems because it allows
 * data to be captured precisely at high update rates, much higher then
 * could be done using simple printouts over a serial port.
 */

#include "debug.h"
#include "vars.h"
#include <optional>
#include <stdint.h>

// local functions
static int CountActiveVars(DebugVar *vptr[]);

// Trace buffer
static CircBuff<uint32_t, 0x4000> traceBuffer;

// The trace control variable is a bit-mapped parameter which
// is used to start/stop the trace and read its status.
// Right now only bit 0 is in use.  Set this to start capturing
// data to the trace buffer.  It will auto-clear when the buffer
// is full
static int32_t traceCtrl;

// The trace period gives the period of the trace data capture
// in units of loop cycles.
static int32_t tracePeriod;

// Trace count is used to count the period
static int32_t traceCount;

// This gives the number of samples collected so far.
// A sample is one group of trace variables, so if there are 3
// trace variables enabled then each sample is 3 x 32-bit values.
static int32_t traceSamp;

// These parameters give the variable IDs being captured
#define TRACE_VAR_CT 4
static int32_t traceVar[TRACE_VAR_CT];

class TraceCtrlVar : public DebugVar {
public:
  TraceCtrlVar(const char *name, int32_t *data, const char *help,
               const char *fmt)
      : DebugVar(name, data, help, fmt) {}
  DbgErrCode SetValue(uint8_t *buff, int len);
};

// These variables are used control the trace function
static TraceCtrlVar varTraceCtrl("trace_ctrl", &traceCtrl,
                                 "Used to start/stop the trace function",
                                 "0x%08x");
static DebugVar
    varTracePeriod("trace_period", &tracePeriod,
                   "Period that data will be captured.  Loop cycle units");
static DebugVar varTraceSamp("trace_samples", &traceSamp,
                             "Number of trace samples saved so far");
static DebugVar varTraceVar1("trace_var1", &traceVar[0],
                             "Variable to be saved to the trace buffer");
static DebugVar varTraceVar2("trace_var2", &traceVar[1],
                             "Variable to be saved to the trace buffer");
static DebugVar varTraceVar3("trace_var3", &traceVar[2],
                             "Variable to be saved to the trace buffer");
static DebugVar varTraceVar4("trace_var4", &traceVar[3],
                             "Variable to be saved to the trace buffer");

// This function is called at the end of the high priority loop function.
// It captures any enabled data variables to the trace buffer.
void TraceSample() {

  // Make sure the trace is running, if not we're done
  if (!(traceCtrl & 1))
    return;

  // Count down the trace period
  if (++traceCount < tracePeriod)
    return;
  traceCount = 0;

  // Count the number of active trace variables with a
  // These will have a non-negative traceVar value.
  DebugVar *vptr[TRACE_VAR_CT];
  int vct = CountActiveVars(vptr);

  // If there are no enabled trace variables, or
  // if there isn't enough space in the buffer for
  // a full sample, then stop the trace
  if (!vct || (traceBuffer.FreeCt() < vct)) {
    traceCtrl = 0;
    return;
  }

  // Sample each enabled varible and store the
  // result to the buffer
  for (int i = 0; i < vct; i++) {
    bool ok = traceBuffer.Put(vptr[i]->getDataForTrace());

    // Note that this can't fail as we've already checked for sufficient
    // space above.  I still need to check it though, becuase it's marked
    // as no discard in the circular buffer.
    if (!ok)
      break;
  }

  traceSamp++;
}

// Fill the vptr array with pointers to all the active
// trace variables and return the number of active ones
static int CountActiveVars(DebugVar *vptr[]) {
  int vct = 0;

  for (int i = 0; i < TRACE_VAR_CT; i++) {
    if (traceVar[i] < 0)
      continue;

    // Find the debug variable associated
    // with this ID
    vptr[vct] = DebugVar::findVar(static_cast<uint16_t>(traceVar[i]));

    // If the ID wasn't valid, disable this trace variable
    if (!vptr[vct])
      traceVar[i] = -1;
    else
      vct++;
  }

  return vct;
}

class TraceCmd : public DebugCmd {
public:
  TraceCmd() : DebugCmd(DbgCmdCode::TRACE) {
    // Disable all trace varaibles initially
    for (int i = 0; i < TRACE_VAR_CT; i++)
      traceVar[i] = -1;
  }

  // The trace command is used to download data from the trace buffer.
  DbgErrCode HandleCmd(uint8_t *data, int *len, int max) {

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

  DbgErrCode ReadTraceBuff(uint8_t *data, int *len, int max) {
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
};

DbgErrCode TraceCtrlVar::SetValue(uint8_t *buff, int len) {

  // Temporarily replace the address with a local variable
  // so the setting doesn't take effect immediately
  // Then call the standard SetValue function
  int32_t new_ctrl;
  addr = &new_ctrl;

  DbgErrCode err = DebugVar::SetValue(buff, len);

  addr = &traceCtrl;

  if (err != DbgErrCode::OK)
    return err;

  // When a new trace is started, flush the buffer first
  if ((new_ctrl & 1) && !(traceCtrl & 1)) {
    traceBuffer.Flush();
    traceSamp = 0;
  }
  traceCtrl = new_ctrl;

  return DbgErrCode::OK;
}

TraceCmd traceCmd;
