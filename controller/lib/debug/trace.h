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

#ifndef TRACE_H_
#define TRACE_H_

#include "circular_buffer.h"
#include "debug.h"
#include "vars.h"

#define TRACE_VAR_CT 4

// Trace buffer
extern CircBuff<uint32_t, 0x4000> trace_buffer;

class TraceControl {
public:
  uint32_t GetFlags() const { return flags_; }
  void SetFlags(uint32_t flags) {
    if ((flags & 1) && !(flags_ & 1)) {
      trace_buffer.Flush();
    }
    flags_ = flags;
  }

  uint32_t GetPeriod() { return period_; }
  void SetPeriod(uint32_t period) { period_ = period; }

  void MaybeSample() {
    if (!(flags_ & 1))
      return;
    if (++count_ < period_)
      return;
    count_ = 0;

    if (!SampleAllVars()) {
      // Trace buffer is full, stop tracing.
      SetFlags(0);
    }
  }

  void Flush() {
    flags_ = 0;
    trace_buffer.Flush();
  }

private:
  bool SampleAllVars();

  // A bit-mapped parameter which is used to start/stop the trace and read its
  // status. Right now only bit 0 is in use.  Set this to start capturing data
  // to the trace buffer.  It will auto-clear when the buffer is full
  uint32_t flags_ = 0;
  // The trace period gives the period of the trace data capture
  // in units of loop cycles.
  uint32_t period_ = 0;
  // Number of loop cycles elapsed since tracing was last enabled.
  uint32_t count_ = 0;
};

extern TraceControl trace_control;

// local functions
int CountActiveVars(DebugVarBase *vptr[]);

// These parameters give the variable IDs being captured
extern int32_t trace_var[TRACE_VAR_CT];

#endif // TRACE_H_
