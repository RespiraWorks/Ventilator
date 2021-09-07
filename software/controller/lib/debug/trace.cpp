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

bool Trace::running() const { return running_; }

// (Re-)start the trace
void Trace::start() {
  if (!running_) {
    trace_buffer_.Flush();
  }
  running_ = true;
}

void Trace::stop() { running_ = false; }

uint32_t Trace::period() const { return period_; }

void Trace::set_period(uint32_t period) { period_ = period; }

// Sample all trace variables every "period" calls
void Trace::maybe_sample() {
  if (!running_) return;

  if (cycles_count_ == 0) {
    if (!sample_all_variables()) {
      // Trace buffer is full, stop tracing.
      stop();
    }
  }

  ++cycles_count_;
  if (cycles_count_ >= period_) cycles_count_ = 0;
}

void Trace::flush() { trace_buffer_.Flush(); }

size_t Trace::sample_count() { return trace_buffer_.FullCount() / active_variable_count(); }

uint16_t Trace::active_variable_count() {
  return static_cast<uint16_t>(std::count_if(traced_vars_.begin(), traced_vars_.end(),
                                             [](const Variable::Base *var) { return (var); }));
}

bool Trace::set_traced_variable(uint8_t index, uint16_t variable_registry_id) {
  if (variable_registry_id == Variable::InvalidID) {
    traced_vars_[index] = nullptr;
    return true;
  }
  auto var_ptr = Variable::Registry::singleton().find(variable_registry_id);
  if (!var_ptr || (var_ptr->size() != sizeof(uint32_t))) {
    // variable not found or type is not of correct size
    return false;
  }
  traced_vars_[index] = var_ptr;
  // like in the SetTraceVarId<int index> template, we need to flush the buffer
  // when the set of traced variables change.
  trace_buffer_.Flush();
  return true;
}

uint16_t Trace::traced_variable(uint8_t index) {
  if (index >= MaxVars) {
    return Variable::InvalidID;
  }
  return traced_vars_[index] ? traced_vars_[index]->id() : Variable::InvalidID;
}

[[nodiscard]] bool Trace::get_next_record(std::array<uint32_t, MaxVars> *record, size_t *count) {
  // Grab one sample with interrupts disabled.
  // There's a chance the trace is still running, so we could get interrupted
  // by the high priority thread that adds to the buffer.
  // I want to make sure I read a full sample without being interrupted.
  *count = 0;
  BlockInterrupts block;
  for (auto *var : traced_vars_) {
    if (!var) continue;
    std::optional<uint32_t> dat = trace_buffer_.Get();
    if (!dat) return false;
    (*record)[(*count)++] = *dat;
  }
  return true;
}

bool Trace::sample_all_variables() {
  // If there are no enabled trace variables, or if there isn't enough space
  // in the buffer for a full sample, then signal to stop the trace.
  if (trace_buffer_.FreeCount() < active_variable_count()) {
    return false;
  }
  // Sample each enabled variable and store the result to the buffer.
  for (auto *var : traced_vars_) {
    if (!var) continue;
    // Can't fail as we've already checked for sufficient space above.
    var->get_value(&temp_variable_value_);
    (void)trace_buffer_.Put(temp_variable_value_);
  }
  return true;
}

}  // namespace Debug
