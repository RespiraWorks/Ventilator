/* Copyright 2020-2021, RespiraWorks

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

#include "trace.h"

namespace Debug {

// (Re-)Start the trace
void Trace::Start() {
  if (!running_) {
    trace_buffer_.Flush();
  }
  running_ = true;
}

// Sample all trace variables every "period" calls
void Trace::MaybeSample() {
  if (!running_)
    return;

  if (cycles_count_ == 0) {
    if (!SampleAllVars()) {
      // Trace buffer is full, stop tracing.
      Stop();
    }
  }

  ++cycles_count_;
  if (cycles_count_ >= period_)
    cycles_count_ = 0;
}

bool Trace::SetTracedVarId(uint8_t index, uint16_t id) {
  if (index >= kMaxTraceVars) {
    return false;
  }
  traced_vars_[index] = DebugVar::FindVar(id);
  // like in the SetTraceVarId<int index> template, we need to flush the buffer
  // when the set of traced variables change.
  trace_buffer_.Flush();
  return true;
}

int16_t Trace::GetTracedVarId(uint8_t index) {
  if (index >= kMaxTraceVars) {
    return -1;
  }
  return traced_vars_[index] ? traced_vars_[index]->GetId() : -1;
}

[[nodiscard]] bool
Trace::GetNextTraceRecord(std::array<uint32_t, kMaxTraceVars> *record,
                          size_t *count) {
  // Grab one sample with interrupts disabled.
  // There's a chance the trace is still running, so we could get interrupted
  // by the high priority thread that adds to the buffer.
  // I want to make sure I read a full sample without being interrupted.
  *count = 0;
  BlockInterrupts block;
  for (auto *var : traced_vars_) {
    if (!var)
      continue;
    std::optional<uint32_t> dat = trace_buffer_.Get();
    if (!dat)
      return false;
    (*record)[(*count)++] = *dat;
  }
  return true;
}

bool Trace::SampleAllVars() {
  // If there are no enabled trace variables, or if there isn't enough space
  // in the buffer for a full sample, then signal to stop the trace.
  if (trace_buffer_.FreeCount() < GetNumActiveVars()) {
    return false;
  }
  // Sample each enabled variable and store the result to the buffer.
  for (auto *var : traced_vars_) {
    if (!var)
      continue;
    // Can't fail as we've already checked for sufficient space above.
    (void)trace_buffer_.Put(var->GetValue());
  }
  return true;
}

} // namespace Debug
