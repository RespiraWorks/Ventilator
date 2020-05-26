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

#include "vars.h"
#include "debug.h"
#include <string.h>

// static member variables
DebugVar *DebugVar::varList[100];
int DebugVar::varCount = 0;

// Used to read a debug variable's value over the debug serial interface
DbgErrCode DebugVar::GetValue(uint8_t *buff, int *len, int max) {
  switch (type_) {
  // We can actually treat 32-bit ints and floats exactly the same way.
  // We're just grabbing their value directly from memory and treating it
  // as an integer so we can split it up and send it out.
  case VarType::INT32:
  case VarType::UINT32:
  case VarType::FLOAT:
    *len = 4;
    if (max < 4)
      return DbgErrCode::NO_MEMORY;

    u32_to_u8(*reinterpret_cast<uint32_t *>(addr_), buff);
    return DbgErrCode::OK;

  // For other types we just generate an error.
  // Anything exotic needs to override this function
  default:
    return DbgErrCode::INTERNAL;
  }
}

// Used to set a debug variable's value over the debug serial interface
DbgErrCode DebugVar::SetValue(uint8_t *buff, int len) {
  switch (type_) {
  // Like with the get, we actually handle both floats and ints the same here.
  case VarType::INT32:
  case VarType::UINT32:
  case VarType::FLOAT:
    if (len < 4)
      return DbgErrCode::MISSING_DATA;

    *reinterpret_cast<uint32_t *>(addr_) = u8_to_u32(buff);
    return DbgErrCode::OK;

  default:
    return DbgErrCode::INTERNAL;
  }
}

// Return the variable as a 32-bit integer.
// We just return zero for any type we don't understand
uint32_t DebugVar::getDataForTrace() {
  switch (type_) {

  // Like above, we treat 32-bit ints and floats the same here
  case VarType::INT32:
  case VarType::UINT32:
  case VarType::FLOAT:
    return *reinterpret_cast<uint32_t *>(addr_);

  default:
    return 0;
  }
}
