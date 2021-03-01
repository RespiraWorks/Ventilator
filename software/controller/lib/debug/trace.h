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

#ifndef TRACE_H_
#define TRACE_H_

#include "circular_buffer.h"
#include "vars.h"
#include <optional>
#include <stdint.h>

namespace Debug {

static constexpr uint32_t kMaxTraceVars{4};

/*
 * Implements a data trace facility.
 *
 * The data trace is a feature which allows data to be written to a largish
 * RAM buffer in real time and read out by the debug inteface later.  The
 * data can then be graphed or otherwise manipulated.
 *
 * This is extremely useful for tuning control systems because it allows
 * data to be captured precisely at high update rates, much higher then
 * could be done using simple printouts over a serial port.
 */
class Trace {
public:
  // Gets/sets the trace flags. At the moment, flags can only be 0 (disabled) or
  // 1 (enabled).
  bool GetStatus() const { return running_; }

  void Start() {
    if (!running_) {
      trace_buffer_.Flush();
    }
    running_ = true;
  }

  void Stop() { running_ = false; }

  uint32_t GetPeriod() const { return period_; }
  void SetPeriod(uint32_t period) { period_ = period; }

  void MaybeSample() {
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

  void Flush() {
    running_ = false;
    trace_buffer_.Flush();
  }

  size_t GetNumSamples() {
    return trace_buffer_.FullCount() / GetNumActiveVars();
  }

  size_t GetNumActiveVars() {
    return static_cast<int>(
        std::count_if(traced_vars_.begin(), traced_vars_.end(),
                      [](const DebugVarBase *var) { return (var); }));
  }

  template <int index> void SetTracedVarId(int32_t id) {
    static_assert(index >= 0 && index < kMaxTraceVars);
    traced_vars_[index] = DebugVar::FindVar(static_cast<uint16_t>(id));
    // The layout of the trace buffer is just a bunch of uint32_t's one per
    // each variable of each sample cycle. In order to be able to interpret
    // the buffer unambiguously, the set of traced variables must be the same
    // throughout the buffer. So, if the set changes, we need to flush.
    trace_buffer_.Flush();
  }

  template <int index> int32_t GetTracedVarId() {
    static_assert(index >= 0 && index < kMaxTraceVars);
    return traced_vars_[index] ? traced_vars_[index]->GetId() : -1;
  }

  // Grabs the next sample of all traced variables from the trace buffer.
  // Returns false if the buffer has less data than the number of traced
  // variables - this should never happen.
  // Sets *count to the number of elements actually set in *record.
  // This will equal GetNumActiveVars() but is easier to use for testing.
  [[nodiscard]] bool
  GetNextTraceRecord(std::array<uint32_t, kMaxTraceVars> *record,
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

private:
  // This function is called at the end of the high priority loop function.
  // It captures any enabled data variables to the trace buffer.
  bool SampleAllVars() {
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

  // Set this to start capturing data to the trace buffer.
  // It will auto-clear when the buffer is full, or when stopped.
  bool running_{false};
  // The trace period gives the period of the trace data capture
  // in units of loop cycles.
  uint32_t period_{1};
  // Number of loop cycles elapsed since last sample was captured.
  uint32_t cycles_count_{0};

  std::array<DebugVarBase *, kMaxTraceVars> traced_vars_ = {nullptr};

  // This circular buffer is as big as we consider reasonable, to give a good
  // tracing capability: 40% of the RAM available on our STM32
  CircularBuffer<uint32_t, 0x4000> trace_buffer_;
};

} // namespace Debug

#endif // TRACE_H_
