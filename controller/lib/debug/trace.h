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

// Called from the main loop
void TraceSample();

// local functions
int CountActiveVars(DebugVar *vptr[]);

// Trace buffer
extern CircBuff<uint32_t, 0x4000> traceBuffer;

// The trace control variable is a bit-mapped parameter which
// is used to start/stop the trace and read its status.
// Right now only bit 0 is in use.  Set this to start capturing
// data to the trace buffer.  It will auto-clear when the buffer
// is full
extern int32_t traceCtrl;

// The trace period gives the period of the trace data capture
// in units of loop cycles.
extern int32_t tracePeriod;

// Trace count is used to count the period
extern int32_t traceCount;

// These parameters give the variable IDs being captured
extern int32_t traceVar[TRACE_VAR_CT];

// This gives the number of samples collected so far.
// A sample is one group of trace variables, so if there are 3
// trace variables enabled then each sample is 3 x 32-bit values.
extern int32_t traceSamp;

#endif // TRACE_H_
