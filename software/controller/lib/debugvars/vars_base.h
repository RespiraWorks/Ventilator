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
#include <limits>

// \todo use namespace instead of lengthy type/class names

// Defines the type of variable
enum class VarType {
  Int32 = 1,
  UInt32 = 2,
  Float = 3,
};

// Defines the possible access to variable
enum class VarAccess {
  ReadOnly = 0,
  ReadWrite = 1,
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
  static constexpr uint16_t InvalidID{std::numeric_limits<uint16_t>::max()};

  // @param type Type of the variable
  // @param name Name of the variable
  // @param help String that the Python code displays describing the variable.
  // @param fmt printf style format string.  This is a hint to the Python code
  // as to how the variable data should be displayed.
  DebugVarBase(VarType type, const char *name, VarAccess access, const char *units,
               const char *help, const char *fmt = "");

  virtual uint32_t GetValue() = 0;
  virtual void SetValue(uint32_t value) = 0;

  const char *GetName() const;
  void prepend_name(const char *prefix);

  const char *GetFormat() const;
  const char *GetHelp() const;
  const char *GetUnits() const;
  VarType GetType() const;
  uint16_t GetId() const;
  VarAccess GetAccess() const;
  bool WriteAllowed() const;

 private:
  uint16_t id_{InvalidID};
  char name_[50];  // should be long enough for most variable names
  const VarType type_;
  const VarAccess access_;
  const char *const units_;
  const char *const help_;
  const char *const fmt_;

  friend class DebugVarRegistry;
};

class DebugVarRegistry {
 public:
  // this is the only way to access it
  static DebugVarRegistry &singleton() {
    // will privately initialize on first call
    static DebugVarRegistry SingletonInstance;
    // will always return
    return SingletonInstance;
  }

  void RegisterVar(DebugVarBase *var);
  DebugVarBase *FindVar(uint16_t vid);
  uint32_t GetVarCount();

 private:
  // List of all the variables in the system.
  // Increase size as necessary
  DebugVarBase *var_list_[100]{};
  uint16_t var_count_{0};

  // singleton assurance, because these are private
  DebugVarRegistry() = default;                // cannot default initialize
  DebugVarRegistry(DebugVarRegistry const &);  // cannot copy initialize
  void operator=(DebugVarRegistry const &);    // cannot copy assign
};
