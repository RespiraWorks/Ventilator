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

#pragma once

#include <array>
#include <cstring>

#include "vars_base.h"

namespace Debug::Variable {

template <typename GetFn, typename SetFn>
class FnVar32 : public Base {
 public:
  FnVar32(Type type, const char *name, Access access, const char *units, GetFn get_fn, SetFn set_fn,
          const char *help, const char *fmt = "")
      : Base(type, name, access, units, help, fmt), get_fn_(get_fn), set_fn_(set_fn) {}

  void serialize_value(void *write_buff) override { get_fn_(write_buff); }
  void deserialize_value(const void *read_buf) override { set_fn_(read_buf); }
  size_t byte_size() const override { return sizeof(uint32_t); }

 private:
  GetFn get_fn_;
  SetFn set_fn_;
};

class Primitive32 : public Base {
 public:
  // 32-bit integer variable.
  // @param data Pointer to an actual variable in C++ code that this will access
  Primitive32(const char *name, Access access, int32_t *data, const char *units,
              const char *help = "", const char *fmt = "%d")
      : Primitive32(Type::Int32, name, access, data, units, help, fmt) {}

  // Like above, but unsigned
  Primitive32(const char *name, Access access, uint32_t *data, const char *units,
              const char *help = "", const char *fmt = "%u")
      : Primitive32(Type::UInt32, name, access, data, units, help, fmt) {}

  // Like above, but float
  Primitive32(const char *name, Access access, float *data, const char *units,
              const char *help = "", const char *fmt = "%.3f")
      : Primitive32(Type::Float, name, access, data, units, help, fmt) {}

  // Gets the current value of the variable as an uint32_t.
  void serialize_value(void *write_buff) override {
    std::memcpy(write_buff, address_, byte_size());
  }

  // Sets the current value of the variable as an uint32_t.
  void deserialize_value(const void *read_buf) override {
    std::memcpy(address_, read_buf, byte_size());
  }

  size_t byte_size() const override { return 4; }

 private:
  void *address_;

  Primitive32(Type type, const char *name, Access access, void *data, const char *units,
              const char *help, const char *fmt = "")
      : Base(type, name, access, units, help, fmt), address_(data) {}
};

class Int32 : public Primitive32 {
 public:
  explicit Int32(const char *name, Access access, int32_t init, const char *units,
                 const char *help = "", const char *fmt = "%d")
      : Primitive32(name, access, &value_, units, help, fmt), value_(init) {}

  void set(int32_t v) { value_ = v; }
  int32_t get() const { return value_; }

 private:
  int32_t value_;
};

class UInt32 : public Primitive32 {
 public:
  explicit UInt32(const char *name, Access access, uint32_t init, const char *units,
                  const char *help = "", const char *fmt = "%u")
      : Primitive32(name, access, &value_, units, help, fmt), value_(init) {}

  void set(uint32_t v) { value_ = v; }
  uint32_t get() const { return value_; }

 private:
  uint32_t value_;
};

class Float : public Primitive32 {
 public:
  explicit Float(const char *name, Access access, float init, const char *units,
                 const char *help = "", const char *fmt = "%.3f")
      : Primitive32(name, access, &value_, units, help, fmt), value_(init) {}

  void set(float v) { value_ = v; }
  float get() const { return value_; }

 private:
  float value_;
};

template <size_t N>
class FloatArray : public Base {
 public:
  FloatArray(const char *name, Access access, const char *units, const char *help = "",
             const char *fmt = "%.3f")
      : Base(Type::FloatArray, name, access, units, help, fmt) {}

  FloatArray(const char *name, Access access, float initial_fill, const char *units,
             const char *help = "", const char *fmt = "%.3f")
      : Base(Type::FloatArray, name, access, units, help, fmt) {
    data_.fill(initial_fill);
  }

  FloatArray(const char *name, Access access, std::array<float, N> initial, const char *units,
             const char *help = "", const char *fmt = "%.3f")
      : Base(Type::FloatArray, name, access, units, help, fmt), data_(initial) {}

  void serialize_value(void *write_buff) override {
    std::memcpy(write_buff, data_.data(), byte_size());
  }

  void deserialize_value(const void *read_buf) override {
    std::memcpy(data_.data(), read_buf, byte_size());
  }

  size_t size() const { return data_.size(); }

  size_t byte_size() const override { return 4 * N; }

  virtual void fill(float value) { data_.fill(value); }

  float get_data(const size_t index) const {
    if (index < N) {
      return data_[index];
    }
    return 0.0f;
  }

  virtual void set_data(const size_t index, const float value) {
    if (index < N) {
      data_[index] = value;
    }
  }

 protected:
  std::array<float, N> data_;
};

template <size_t N>
class String : public Base {
 public:
  String(const char *name, Access access, const char *help = "", const char *fmt = "%s",
         const char *units = "")
      : Base(Type::String, name, access, units, help, fmt) {}

  String(const char *name, Access access, const char *initial, size_t size, const char *help = "",
         const char *fmt = "%s", const char *units = "")
      : Base(Type::String, name, access, units, help, fmt) {
    set(initial, size);
  }

  void serialize_value(void *write_buff) override {
    std::memcpy(write_buff, data.data(), byte_size());
  }

  void deserialize_value(const void *read_buf) override {
    std::memcpy(data.data(), read_buf, byte_size());
  }

  size_t byte_size() const override { return N; }

  void set(const char *new_value, size_t size) {
    size = std::min(size, N);
    std::memcpy(data.data(), new_value, size);
  }

  const char *get() const { return data.data(); }

  std::array<char, N> data = {0};
};

// \TODO nasty math because handler assumes 32-bit types with endian conversion
//       should be able to simplify once the serial protocol is generated

#define DEBUG_STRING(var_name, name, access, static_string, ...)         \
  Debug::Variable::String<(sizeof(static_string) / 4 + 1) * 4> var_name( \
      name, access, static_string, sizeof(static_string), ##__VA_ARGS__)

}  // namespace Debug::Variable
