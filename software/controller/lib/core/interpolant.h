/* Copyright 2021, RespiraWorks

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

#include <stdint.h>

#include "nvparams.h"
#include "vars.h"

// This class abstracts a lookup table that is stored in EEPROM, accessible through the debug
// interface, and used for linear interpolation with input between 0 and 1.
template <size_t N>
class Interpolant {
 public:
  Interpolant(const char *name, float initial_fill, const char *units = "", const char *help = "",
              const char *fmt = "%.3f")
      : cal_table_(name, Debug::Variable::Access::ReadWrite, initial_fill, units, help, fmt){};

  Interpolant(const char *name, std::array<float, N> initial, const char *units = "",
              const char *help = "", const char *fmt = "%.3f")
      : cal_table_(name, Debug::Variable::Access::ReadWrite, initial, units, help, fmt){};

  Interpolant(const char *name, NVParams::Handler *nv_params, const uint16_t offset,
              const char *units = "", const char *help = "", const char *fmt = "%.3f")
      : cal_table_(name, Debug::Variable::Access::ReadWrite, nv_params, offset, units, help, fmt){};

  float get_value(const float input) {
    if (input <= 0.0f) return cal_table_.get_data(0);

    if (input >= 1.0f) return cal_table_.get_data(N - 1);

    // Interpolant cal_table_ is for regularly spaced inputs between 0 and 1, meaning cal_table_[n]
    // represents the expected output value when input = n/(N-1). Solving for n yields
    size_t index = static_cast<size_t>(input * static_cast<float>(N - 1));
    // Part of input that is above index/(N-1):
    float remainder = input * static_cast<float>(N - 1) - static_cast<float>(index);

    return cal_table_.get_data(index) +
           remainder * (cal_table_.get_data(index + 1) - cal_table_.get_data(index));
  };

  void link(NVParams::Handler *nv_params, const uint16_t offset) {
    cal_table_.link(nv_params, offset);
  };

 private:
  Debug::Variable::NVFloatArray<N> cal_table_;
};
