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
class DebugVar {
public:
  // 32-bit integer variable.  The default get/set functions will probably be
  // fine
  // @param name Name of the variable
  // @param data Pointer to the actual variable in C++ code that this will
  // access
  // @param help String that the Python code displays describing the variable.
  // @param fmt printf style format string.  This is a hint to the Python code
  // as to how the variable data should be displayed.
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
  virtual uint32_t GetValue() {
    uint32_t res;
    std::memcpy(&res, addr_, 4);
    return res;
  }

  // Sets the current value of the variable as an uint32_t.
  virtual void SetValue(uint32_t value) { std::memcpy(addr_, &value, 4); }

  const char *GetName() const { return name_; }
  const char *GetFormat() const { return fmt_; }
  const char *GetHelp() const { return help_; }
  VarType GetType() const { return type_; }
  uint16_t GetId() const { return id_; }

  static DebugVar *FindVar(uint16_t vid) {
    if (vid >= std::size(var_list))
      return nullptr;
    return var_list[vid];
  }

protected:
  const VarType type_;
  const char *const name_;
  const char *const fmt_;
  const char *const help_;
  void *addr_;
  const uint16_t id_;

private:
  DebugVar(VarType type, const char *name, void *data, const char *help,
           const char *fmt)
      : type_(type), name_(name), fmt_(fmt), help_(help), addr_(data),
        id_(var_count) {
    if (var_count < static_cast<uint16_t>(std::size(var_list)))
      var_list[id_] = this;
    var_count++;
  }

  // List of all the variables in the system.
  // Increase size as necessary
  static DebugVar *var_list[100];
  static uint16_t var_count;
};

#endif
