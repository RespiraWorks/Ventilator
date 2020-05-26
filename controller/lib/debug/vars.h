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

// Defines the type of variable
enum class VarType {
  UNKNOWN = 0,
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
           const char *fmt = "%d")
      : DebugVar(VarType::INT32, name, data, help, fmt) {}

  // Like above, but float
  DebugVar(const char *name, float *data, const char *help = "",
           const char *fmt = "%.3f")
      : DebugVar(VarType::FLOAT, name, data, help, fmt) {}

  // Gets the current value of the variable.
  // @param buff The variable's value is stored here
  // @param len Return the length (in bytes) of the value here
  // @param max Maximum number of bytes we can write to buff
  // @return An error code, 0 on success
  virtual DbgErrCode GetValue(uint8_t *buff, int *len, int max);

  // Sets the current value of the variable.
  // @param buff Holds the data to be written to the variable
  // @param len Number of valid bytes in buffer
  // @return An error code, 0 on success
  virtual DbgErrCode SetValue(uint8_t *buff, int len);

  // Returns a representation of this variable as a 32-bit integer.
  // This is used when capturing the variables value to the trace buffer.
  // Only variables that can be represented by 32-bits can be traced.
  // For floats, this returns the raw bits of the float, the Python
  // program handles converting it back into a float.
  virtual uint32_t getDataForTrace();

  const char *GetName() const { return name_; }
  const char *GetFormat() const { return fmt_; }
  const char *GetHelp() const { return help_; }
  VarType GetType() const { return type_; }

  static DebugVar *FindVar(uint16_t vid) {
    if (vid >= ARRAY_CT(varList))
      return 0;
    return varList[vid];
  }

protected:
  const VarType type_;
  const char *const name_;
  const char *const fmt_;
  const char *const help_;
  void *addr_;

private:
  DebugVar(VarType type, const char *name, void *data, const char *help,
           const char *fmt)
      : type_(type), name_(name), fmt_(fmt), help_(help), addr_(data) {
    if (varCount < static_cast<int>(ARRAY_CT(varList)))
      varList[varCount++] = this;
  }

  // List of all the variables in the system.
  // Increase size as necessary
  static DebugVar *varList[100];
  static int varCount;
};

#endif
