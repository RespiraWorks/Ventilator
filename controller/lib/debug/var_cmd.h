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

#ifndef VAR_CMD_H_
#define VAR_CMD_H_

#include "debug.h"

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

#endif // VAR_CMD_H_
