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

#include "trace.h"
#include "debug.h"
#include "vars.h"
#include <optional>
#include <stdint.h>

CircBuff<uint32_t, 0x4000> traceBuffer;
int32_t traceCtrl;
int32_t tracePeriod;
int32_t traceCount;
int32_t traceSamp;
int32_t traceVar[TRACE_VAR_CT];

class TraceCtrlVar : public DebugVar {
public:
  TraceCtrlVar()
      : DebugVar("trace_ctrl", &traceCtrl,
                 "Used to start/stop the trace function", "0x%08x") {}
  void SetValue(uint32_t value) override;
};

// These variables are used control the trace function
static TraceCtrlVar varTraceCtrl;
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
    bool ok = traceBuffer.Put(vptr[i]->GetValue());

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
int CountActiveVars(DebugVar *vptr[]) {
  int vct = 0;

  for (int i = 0; i < TRACE_VAR_CT; i++) {
    if (traceVar[i] < 0)
      continue;

    // Find the debug variable associated
    // with this ID
    vptr[vct] = DebugVar::FindVar(static_cast<uint16_t>(traceVar[i]));

    // If the ID wasn't valid, disable this trace variable
    if (!vptr[vct])
      traceVar[i] = -1;
    else
      vct++;
  }

  return vct;
}

void TraceCtrlVar::SetValue(uint32_t value) {

  // Temporarily replace the address with a local variable
  // so the setting doesn't take effect immediately
  // Then call the standard SetValue function
  int32_t new_ctrl;
  addr_ = &new_ctrl;

  DebugVar::SetValue(value);

  // When a new trace is started, flush the buffer first
  if ((new_ctrl & 1) && !(traceCtrl & 1)) {
    traceBuffer.Flush();
    traceSamp = 0;
  }
  traceCtrl = new_ctrl;
  addr_ = &traceCtrl;
}
