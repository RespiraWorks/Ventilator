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
           const char *fmt = "%d");

  // Like above, but unsigned
  DebugVar(const char *name, uint32_t *data, const char *help = "",
           const char *fmt = "%d");

  // float variable.  The default get/set functions will probably be fine
  // @param name Name of the variable
  // @param data Pointer to the actual variable in C++ code that this will
  // access
  // @param fmt printf style format string.  This is a hint to the Python code
  // as to how
  //            the variable data should be displayed.
  // @param help String that the Python code displays describing the variable.
  DebugVar(const char *name, float *data, const char *help = "",
           const char *fmt = "%.3f");

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

  const char *getName() const { return name; }
  const char *getFormat() const { return fmt; }
  const char *getHelp() const { return help; }
  VarType getType() const { return type; }

  static DebugVar *findVar(uint16_t vid) {
    if (vid >= ARRAY_CT(varList))
      return 0;
    return varList[vid];
  }

protected:
  VarType type;
  const char *name;
  const char *fmt;
  const char *help;
  void *addr;

private:
  // List of all the variables in the system.
  // Increase size as necessary
  static DebugVar *varList[100];
  static int varCount;

  void RegisterVar() {
    if (varCount < static_cast<int>(ARRAY_CT(varList)))
      varList[varCount++] = this;
  }
};

// Command handler for variable access
//
// The first byte of data passed to the command gives a sub-command
// which defines what the command does and the structure of it's data.
//
// Sub-commands:
//  0 - Used to read info about a variable.  The debug interface calls
//      this repeatedly on startup to enumerate the variables currently
//      supported by the code.  This way new debug variables can be added
//      on the fly without modifying the Python code to match.
//
//      Input data to this command is a 16-bit variable ID (assigned
//      dynamically as variables are created).  The output is info about
//      the variable.  See the code below for details of the output format
//
//  1 - Read the variables value.
//
//  2 - Set the variables value.
//
class VarCmd : public DebugCmd {
public:
  VarCmd() : DebugCmd(DbgCmdCode::VAR) {}

  DbgErrCode HandleCmd(uint8_t *data, int *len, int max);

  // Return info about one of the variables.
  // The 16-bit variable ID is passed in.  These IDs are
  // automatically assigned as variables are registered in the
  // system starting with 0.  The Python code can read them
  // all out until it gets an error code indicating that the
  // passed ID is invalid.
  DbgErrCode GetVarInfo(uint8_t *data, int *len, int max);

  DbgErrCode GetVar(uint8_t *data, int *len, int max);

  DbgErrCode SetVar(uint8_t *data, int *len, int max);
};

extern VarCmd varCmd;

#endif
