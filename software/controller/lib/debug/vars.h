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

#ifndef VARS_H
#define VARS_H

#include "debug.h"
#include <array>
#include <cstring>

// Defines the type of variable
enum class VarType {
  INT32 = 1,
  UINT32 = 2,
  FLOAT = 3,
};

// This class represents a variable that you can read/write using the
// debug serial port.
//
// We give each such variable a name which the debugger command line will
// use to access it.  We can also link it with a C++ variable whose value
// it will read or write.
//
// The fmt string gives the debug interface a suggestion on the best way
// to display the variable's data.
class DebugVarBase {
public:
  // @param type Type of the variable
  // @param name Name of the variable
  // @param help String that the Python code displays describing the variable.
  // @param fmt printf style format string.  This is a hint to the Python code
  // as to how the variable data should be displayed.
  DebugVarBase(VarType type, const char *name, const char *help,
               const char *fmt)
      : type_(type), name_(name), help_(help), fmt_(fmt), id_(var_count) {
    if (var_count < static_cast<uint16_t>(std::size(var_list)))
      var_list[id_] = this;
    var_count++;
  }

  static DebugVarBase *FindVar(uint16_t vid) {
    if (vid >= std::size(var_list))
      return nullptr;
    return var_list[vid];
  }

  virtual uint32_t GetValue() = 0;
  virtual void SetValue(uint32_t value) = 0;

  const char *GetName() const { return name_; }
  const char *GetFormat() const { return fmt_; }
  const char *GetHelp() const { return help_; }
  VarType GetType() const { return type_; }
  uint16_t GetId() const { return id_; }

private:
  const VarType type_;
  const char *const name_;
  const char *const help_;
  const char *const fmt_;
  const uint16_t id_;

  // List of all the variables in the system.
  // Increase size as necessary
  static DebugVarBase *var_list[100];
  static uint16_t var_count;
};

template <typename GetFn, typename SetFn>
class FnDebugVar : public DebugVarBase {
public:
  FnDebugVar(VarType type, const char *name, const char *help, const char *fmt,
             GetFn get_fn, SetFn set_fn)
      : DebugVarBase(type, name, help, fmt), get_fn_(get_fn), set_fn_(set_fn) {}

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
  DebugVar(const char *name, int32_t *data, const char *help = "",
           const char *fmt = "%d")
      : DebugVar(VarType::INT32, name, data, help, fmt) {}

  // Like above, but unsigned
  DebugVar(const char *name, uint32_t *data, const char *help = "",
           const char *fmt = "%u")
      : DebugVar(VarType::UINT32, name, data, help, fmt) {}

  // Like above, but float
  DebugVar(const char *name, float *data, const char *help = "",
           const char *fmt = "%.3f")
      : DebugVar(VarType::FLOAT, name, data, help, fmt) {}

  // Gets the current value of the variable as an uint32_t.
  uint32_t GetValue() override {
    uint32_t res;
    std::memcpy(&res, addr_, 4);
    return res;
  }

  // Sets the current value of the variable as an uint32_t.
  void SetValue(uint32_t value) override { std::memcpy(addr_, &value, 4); }

private:
  void *addr_;

  DebugVar(VarType type, const char *name, void *data, const char *help,
           const char *fmt)
      : DebugVarBase(type, name, help, fmt), addr_(data) {}
};

class DebugInt32 : public DebugVar {
public:
  explicit DebugInt32(const char *name, const char *help = "", int32_t init = 0,
                      const char *fmt = "%d")
      : DebugVar(name, &value_, help, fmt), value_(init) {}

  void Set(int32_t v) { value_ = v; }
  int32_t Get() { return value_; }

private:
  int32_t value_;
};

class DebugUInt32 : public DebugVar {
public:
  explicit DebugUInt32(const char *name, const char *help = "",
                       uint32_t init = 0, const char *fmt = "%u")
      : DebugVar(name, &value_, help, fmt), value_(init) {}

  void Set(uint32_t v) { value_ = v; }
  uint32_t Get() { return value_; }

private:
  uint32_t value_;
};

class DebugFloat : public DebugVar {
public:
  explicit DebugFloat(const char *name, const char *help = "", float init = 0,
                      const char *fmt = "%.3f")
      : DebugVar(name, &value_, help, fmt), value_(init) {}

  void Set(float v) { value_ = v; }
  float Get() { return value_; }

private:
  float value_;
};

#endif
