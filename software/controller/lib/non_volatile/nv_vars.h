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

#include "nvparams.h"
#include "vars.h"

namespace Debug::Variable {
class NonVolatile {
 public:
  NonVolatile() = default;
  NonVolatile(NVParams::Handler *nv_params, const uint16_t offset)
      : nv_params_(nv_params), offset_(offset){};

  bool linked() const { return nv_params_ != nullptr; };

  void write(const void *write_buff, uint16_t len) const {
    if (linked()) nv_params_->Set(offset_, write_buff, len);
  };

  void read(void *read_buff, uint16_t len) const {
    if (linked()) nv_params_->Get(offset_, read_buff, len);
  };

  void link(NVParams::Handler *nv_params, const uint16_t offset) {
    nv_params_ = nv_params;
    offset_ = offset;
  };

 protected:
  NVParams::Handler *nv_params_{nullptr};
  uint16_t offset_{0};
};

template <size_t N>
class NVFloatArray : public FloatArray<N>, public NonVolatile {
 public:
  NVFloatArray(const char *name, Access access, NVParams::Handler *nv_params, const uint16_t offset,
               const char *units = "", const char *help = "", const char *fmt = "%.3f")
      : FloatArray<N>(name, access, units, help, fmt), NonVolatile(nv_params, offset) {
    float nv_data[N] = {0.0f};
    read(&nv_data, 4 * N);
    FloatArray<N>::deserialize_value(&nv_data);
  }

  NVFloatArray(const char *name, Access access, const char *units, const char *help = "",
               const char *fmt = "%.3f")
      : FloatArray<N>(name, access, units, help, fmt){};

  NVFloatArray(const char *name, Access access, float initial_fill, const char *units = "",
               const char *help = "", const char *fmt = "%.3f")
      : FloatArray<N>(name, access, initial_fill, units, help, fmt){};

  NVFloatArray(const char *name, Access access, std::array<float, N> initial,
               const char *units = "", const char *help = "", const char *fmt = "%.3f")
      : FloatArray<N>(name, access, initial, units, help, fmt){};

  void deserialize_value(const void *write_buff) override {
    FloatArray<N>::deserialize_value(write_buff);
    // write checks that the var is linked, no need to check it here as well
    write(write_buff, 4 * N);
  }

  void set_data(const size_t index, const float value) override {
    FloatArray<N>::set_data(index, value);
    if (linked()) {
      nv_params_->Set(static_cast<uint16_t>(offset_ + index * 4), &value, 4);
    }
  }

  void fill(float value) override {
    FloatArray<N>::fill(value);
    uint8_t write_buff[4 * N];
    FloatArray<N>::serialize_value(&write_buff);
    write(write_buff, 4 * N);
  }

  void link(NVParams::Handler *nv_params, const uint16_t offset) {
    NonVolatile::link(nv_params, offset);
    float nv_data[N] = {0.0f};
    read(&nv_data, 4 * N);
    FloatArray<N>::deserialize_value(&nv_data);
  };
};
};  // namespace Debug::Variable
