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

namespace Debug::Variable {

// Defines the type of variable
enum class Type {
  Int32 = 1,
  UInt32 = 2,
  Float = 3,
};

// Defines the possible access to variable
enum class Access {
  ReadOnly = 0,
  ReadWrite = 1,
};

static constexpr uint16_t InvalidID{std::numeric_limits<uint16_t>::max()};

// This class represents a variable that you can read/write using the
// debug serial port.
//
// We give each such variable a name which the debugger command line will
// use to access it.  We can also link it with a C++ variable whose value
// it will read or write.
//
// The fmt string gives the debug interface a suggestion on the best way
// to display the variable's data.
class Base {
public:
  // @param type Type of the variable
  // @param name Name of the variable
  // @param help String that the Python code displays describing the variable.
  // @param fmt printf style format string.  This is a hint to the Python code
  // as to how the variable data should be displayed.
  Base(Type type, const char *name, Access access, const char *units,
       const char *help, const char *fmt = "");

  virtual uint32_t GetValue() = 0;
  virtual void SetValue(uint32_t value) = 0;

  const char *GetName() const;
  void prepend_name(const char *prefix);
  void append_help(const char *text);

  const char *GetFormat() const;
  const char *GetHelp() const;
  const char *GetUnits() const;
  Type GetType() const;
  uint16_t GetId() const;
  Access GetAccess() const;
  bool WriteAllowed() const;

private:
  uint16_t id_{InvalidID};
  const Type type_;
  const Access access_;
  char name_[50]{0};
  char units_[20]{0};
  char help_[300]{0};
  char fmt_[10]{0};

  friend class Registry;
};

class Registry {
public:
  // this is the only way to access it
  static Registry &singleton() {
    // will privately initialize on first call
    static Registry SingletonInstance;
    // will always return
    return SingletonInstance;
  }

  void RegisterVar(Base *var);
  Base *FindVar(uint16_t vid);
  uint32_t GetVarCount() const;

private:
  // List of all the variables in the system.
  // Increase size as necessary
  Base *var_list_[100]{};
  uint16_t var_count_{0};

  // singleton assurance, because these are private
  Registry() = default;             // cannot default initialize
  Registry(Registry const &);       // cannot copy initialize
  void operator=(Registry const &); // cannot copy assign
};

} // namespace Debug::Variable
