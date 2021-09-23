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
  Interpolant() = default;

  // Because we store data in EEPROM, the table has to be initialized after the
  // initialization of nvparams (which reads EEPROM).
  // Be sure to initialize all interpolants after having read the EEPROM.
  void init(const char *name, NVParams::Handler *nv_params, uint16_t offset, const char *units = "",
            const char *help = "", const char *fmt = "%.3f") {
    table_.emplace(name, Debug::Variable::Access::ReadWrite, nv_params, offset, units, help, fmt);
  };

  float get_value(const float input) {
    if (!table_.has_value()) return 0.0f;

    if (input <= 0.0f) return table_->data[0];

    if (input >= 1.0f) return table_->data[N - 1];

    // Interpolant table is for regularly spaced inputs between 0 and 1, meaning table_[n]
    // represents the expected output value when input = n/(N-1). Solving for n yields
    size_t index = static_cast<size_t>(input * static_cast<float>(N - 1));
    // Part of input that is not taken into account because of the rounding of index:
    float remainder = input * static_cast<float>(N - 1) - static_cast<float>(index);

    return table_->data[index] + remainder * (table_->data[index + 1] - table_->data[index]);
  };

 private:
  std::optional<Debug::Variable::NVFloatArray<N>> table_;
};
