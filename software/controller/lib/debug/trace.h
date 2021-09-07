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

#pragma once

#include <cstdint>
#include <optional>

#include "circular_buffer.h"
#include "vars.h"

namespace Debug {

/*
 * Implements a data trace facility.
 *
 * The data trace is a feature which allows data to be written to a largish RAM buffer in real time
 * and read out by the debug interface later. The data can then be graphed or otherwise manipulated.
 *
 * This is extremely useful for tuning control systems because it allows data to be captured
 * precisely at high update rates, much higher than could be done using simple printouts over a
 * serial port.
 */
class Trace {
 public:
  static constexpr uint16_t MaxVars{4};

  // This circular buffer is as big as we consider reasonable, to give a good tracing capability:
  // 40% of the RAM available on our STM32
  static constexpr size_t BufferSize{0x4000};

  /// \returns false if manually stopped or stopped on its own when buffer was filled
  bool running() const;

  /// \brief starts acquisition, which will continue until buffer is full or explicitly stopped
  void start();

  /// \brief stops acquisition
  void stop();

  /// \returns number of cycles that Trace will wait before sampling variables
  uint32_t period() const;

  /// \param period number of cycles that Trace should wait before sampling variables
  void set_period(uint32_t period);

  /// \brief Called fromm looping function. It may capture data if cycle count has been reached.
  /// \post If cycle count was reached, the cycle counter will be reset.
  void maybe_sample();

  /// \brief clears trace buffer
  void flush();

  /* \returns number of acquired samples in time series,
   * i.e. not multiplied by traced variable count
   * */
  size_t sample_count();

  /// \returns number of valid variables selected for acquisition
  uint16_t active_variable_count();

  /* \brief selects position `index` to trace the variable spcified by `variable_registry_id`
   * \param index assumed to be a number number < MaxVars
   * \param variable_registry_id may be any variable ID, including Variable::InvalidID, which will
   *        disable tracing at that position
   * \returns false if variable could not be found in registry, or the type was of wrong size
   *          true if successfully set or disabled
   *          */
  bool set_traced_variable(uint8_t index, uint16_t variable_registry_id);

  /// \returns id of variable at index, Variable::InvalidID if variable is not valid
  uint16_t traced_variable(uint8_t index);

  /* Grabs the next sample of all traced variables from the trace buffer. Returns false if the
   * buffer has less data than the number of traced variables - this should never happen. Sets
   * *count to the number of elements actually set in *record. This will equal
   * active_variable_count() but is easier to use for testing.
   * */
  [[nodiscard]] bool get_next_record(std::array<uint32_t, MaxVars> *record, size_t *count);

 private:
  // This function is called at the end of the high priority loop function.
  // It captures any enabled data variables to the trace buffer.
  bool sample_all_variables();

  // It will auto-clear when the buffer is full, or when stopped.
  bool running_{false};

  // The trace period gives the period of the trace data capture in units of loop cycles.
  uint32_t period_{1};
  // Number of loop cycles elapsed since last sample was captured.
  uint32_t cycles_count_{0};

  std::array<Variable::Base *, MaxVars> traced_vars_ = {nullptr};

  // Pre-allocated because it will be reused for every capture
  uint32_t temp_variable_value_{0};
  CircularBuffer<uint32_t, BufferSize> trace_buffer_;
};

}  // namespace Debug
