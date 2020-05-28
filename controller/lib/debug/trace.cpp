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

CircBuff<uint32_t, 0x4000> trace_buffer;
int32_t trace_var[TRACE_VAR_CT];
TraceControl trace_control;

// These variables are used control the trace function
static FnDebugVar varTraceCtrl(
    VarType::UINT32, "trace_ctrl", "Used to start/stop the trace function",
    "0x%08x", [] { return trace_control.GetFlags(); },
    [](uint32_t value) { trace_control.SetFlags(value); });

static FnDebugVar varTracePeriod(
    VarType::UINT32, "trace_period",
    "Period that data will be captured.  Loop cycle units", "%u",
    [] { return trace_control.GetPeriod(); },
    [](uint32_t value) { trace_control.SetPeriod(value); });

static FnDebugVar varTraceSamp(
    VarType::UINT32, "trace_samples", "Number of trace samples saved so far",
    "%u", [] { return trace_buffer.FullCt() / CountActiveVars(nullptr); },
    [](uint32_t value) { /* ignore */ });

static DebugVar vartrace_var1("trace_var1", &trace_var[0],
                              "Variable to be saved to the trace buffer");
static DebugVar vartrace_var2("trace_var2", &trace_var[1],
                              "Variable to be saved to the trace buffer");
static DebugVar vartrace_var3("trace_var3", &trace_var[2],
                              "Variable to be saved to the trace buffer");
static DebugVar vartrace_var4("trace_var4", &trace_var[3],
                              "Variable to be saved to the trace buffer");

// This function is called at the end of the high priority loop function.
// It captures any enabled data variables to the trace buffer.
bool TraceControl::SampleAllVars() {
  // Count the number of active trace variables with a
  // These will have a non-negative trace_var value.
  DebugVarBase *vptr[TRACE_VAR_CT];
  int vct = CountActiveVars(vptr);

  // If there are no enabled trace variables, or
  // if there isn't enough space in the buffer for
  // a full sample, then stop the trace
  if (!vct || (trace_buffer.FreeCt() < vct)) {
    return false;
  }

  // Sample each enabled varible and store the
  // result to the buffer
  for (int i = 0; i < vct; i++) {
    bool ok = trace_buffer.Put(vptr[i]->GetValue());

    // Note that this can't fail as we've already checked for sufficient
    // space above.  I still need to check it though, becuase it's marked
    // as no discard in the circular buffer.
    if (!ok)
      break;
  }

  return true;
}

// Fill the vptr array with pointers to all the active
// trace variables and return the number of active ones
int CountActiveVars(DebugVarBase *vptr[]) {
  int vct = 0;

  for (int i = 0; i < TRACE_VAR_CT; i++) {
    if (trace_var[i] < 0)
      continue;

    // Find the debug variable associated with this ID
    auto *var = DebugVar::FindVar(static_cast<uint16_t>(trace_var[i]));

    // If the ID wasn't valid, disable this trace variable
    if (!var) {
      trace_var[i] = -1;
    } else {
      if (vptr != nullptr)
        vptr[vct] = var;
      vct++;
    }
  }

  return vct;
}
