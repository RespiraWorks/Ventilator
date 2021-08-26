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

#include <cstring>

#include "vars_base.h"

namespace Debug::Variable {

template <typename GetFn, typename SetFn>
class FnVar : public Base {
 public:
  FnVar(Type type, const char *name, Access access, const char *units, GetFn get_fn, SetFn set_fn,
        const char *help, const char *fmt = "")
      : Base(type, name, access, units, help, fmt), get_fn_(get_fn), set_fn_(set_fn) {
    Registry::singleton().RegisterVar(this);
  }

  uint32_t GetValue() override { return get_fn_(); }
  void SetValue(uint32_t value) override { set_fn_(value); }

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
  uint32_t GetValue() override {
    uint32_t res;
    std::memcpy(&res, address_, 4);
    return res;
  }

  // Sets the current value of the variable as an uint32_t.
  void SetValue(uint32_t value) override { std::memcpy(address_, &value, 4); }

 private:
  void *address_;

  Primitive32(Type type, const char *name, Access access, void *data, const char *units,
              const char *help, const char *fmt = "")
      : Base(type, name, access, units, help, fmt), address_(data) {
    Registry::singleton().RegisterVar(this);
  }
};

class Int32 : public Primitive32 {
 public:
  explicit Int32(const char *name, Access access, int32_t init, const char *units,
                 const char *help = "", const char *fmt = "%d")
      : Primitive32(name, access, &value_, units, help, fmt), value_(init) {}

  void Set(int32_t v) { value_ = v; }
  int32_t Get() const { return value_; }

 private:
  int32_t value_;
};

class UInt32 : public Primitive32 {
 public:
  explicit UInt32(const char *name, Access access, uint32_t init, const char *units,
                  const char *help = "", const char *fmt = "%u")
      : Primitive32(name, access, &value_, units, help, fmt), value_(init) {}

  void Set(uint32_t v) { value_ = v; }
  uint32_t Get() const { return value_; }

 private:
  uint32_t value_;
};

class Float : public Primitive32 {
 public:
  explicit Float(const char *name, Access access, float init, const char *units,
                 const char *help = "", const char *fmt = "%.3f")
      : Primitive32(name, access, &value_, units, help, fmt), value_(init) {}

  void Set(float v) { value_ = v; }
  float Get() const { return value_; }

 private:
  float value_;
};

}  // namespace Debug::Variable
