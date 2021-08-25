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

#include "vars_base.h"

template <typename GetFn, typename SetFn>
class FnDebugVar : public DebugVarBase {
 public:
  FnDebugVar(VarType type, const char *name, VarAccess access, const char *units, GetFn get_fn,
             SetFn set_fn, const char *help, const char *fmt = "")
      : DebugVarBase(type, name, access, units, help, fmt), get_fn_(get_fn), set_fn_(set_fn) {
    DebugVarRegistry::singleton().RegisterVar(this);
  }

  uint32_t GetValue() override { return get_fn_(); }
  void SetValue(uint32_t value) override { set_fn_(value); }

 private:
  GetFn get_fn_;
  SetFn set_fn_;
};

class DebugVar : public DebugVarBase {
 public:
  // 32-bit integer variable.
  // @param data Pointer to an actual variable in C++ code that this will access
  DebugVar(const char *name, VarAccess access, int32_t *data, const char *units,
           const char *help = "", const char *fmt = "%d")
      : DebugVar(VarType::Int32, name, access, data, units, help, fmt) {}

  // Like above, but unsigned
  DebugVar(const char *name, VarAccess access, uint32_t *data, const char *units,
           const char *help = "", const char *fmt = "%u")
      : DebugVar(VarType::UInt32, name, access, data, units, help, fmt) {}

  // Like above, but float
  DebugVar(const char *name, VarAccess access, float *data, const char *units,
           const char *help = "", const char *fmt = "%.3f")
      : DebugVar(VarType::Float, name, access, data, units, help, fmt) {}

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

  DebugVar(VarType type, const char *name, VarAccess access, void *data, const char *units,
           const char *help, const char *fmt = "")
      : DebugVarBase(type, name, access, units, help, fmt), address_(data) {
    DebugVarRegistry::singleton().RegisterVar(this);
  }
};

class DebugInt32 : public DebugVar {
 public:
  explicit DebugInt32(const char *name, VarAccess access, int32_t init, const char *units,
                      const char *help = "", const char *fmt = "%d")
      : DebugVar(name, access, &value_, units, help, fmt), value_(init) {}

  void Set(int32_t v) { value_ = v; }
  int32_t Get() const { return value_; }

 private:
  int32_t value_;
};

class DebugUInt32 : public DebugVar {
 public:
  explicit DebugUInt32(const char *name, VarAccess access, uint32_t init, const char *units,
                       const char *help = "", const char *fmt = "%u")
      : DebugVar(name, access, &value_, units, help, fmt), value_(init) {}

  void Set(uint32_t v) { value_ = v; }
  uint32_t Get() const { return value_; }

 private:
  uint32_t value_;
};

class DebugFloat : public DebugVar {
 public:
  explicit DebugFloat(const char *name, VarAccess access, float init, const char *units,
                      const char *help = "", const char *fmt = "%.3f")
      : DebugVar(name, access, &value_, units, help, fmt), value_(init) {}

  void Set(float v) { value_ = v; }
  float Get() const { return value_; }

 private:
  float value_;
};
