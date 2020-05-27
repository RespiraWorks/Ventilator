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

#include "var_cmd.h"
#include "debug.h"
#include "vars.h"
#include <stdint.h>
#include <string.h>

DbgErrCode VarCmd::HandleCmd(uint8_t *data, int *len, int max) {

  // The first byte of data is always required, this
  // gives the sub-command.
  if (*len < 1)
    return DbgErrCode::MISSING_DATA;

  switch (data[0]) {

  // Return info about one of the variables.
  case 0:
    return GetVarInfo(data, len, max);

  case 1:
    return GetVar(data, len, max);

  case 2:
    return SetVar(data, len, max);

  default:
    return DbgErrCode::RANGE;
  }
}

// Return info about one of the variables.
// The 16-bit variable ID is passed in.  These IDs are
// automatically assigned as variables are registered in the
// system starting with 0.  The Python code can read them
// all out until it gets an error code indicating that the
// passed ID is invalid.
DbgErrCode VarCmd::GetVarInfo(uint8_t *data, int *len, int max) {

  // We expect a 16-bit ID to be passed
  if (*len < 3)
    return DbgErrCode::MISSING_DATA;

  uint16_t vid = u8_to_u16(&data[1]);

  const DebugVar *var = DebugVar::FindVar(vid);
  if (!var)
    return DbgErrCode::BAD_VARID;

  // The info I return consists of the following:
  // <type> - 1 byte variable type code
  // <reserved> - 3 reserved bytes for things we think of later
  // <name len> - 1 byte gives length of variable name string
  // <fmt len>  - 1 byte gives length of formation string
  // <help len> - 1 byte gives length of help string
  // <reserved> - 1 reserved byte
  // <name> - variable length name string
  // <fmt>  - variable length format string
  // <help> - variable length format string
  // The strings are not null terminated.
  int nLen = static_cast<int>(strlen(var->GetName()));
  int fLen = static_cast<int>(strlen(var->GetFormat()));
  int hLen = static_cast<int>(strlen(var->GetHelp()));

  if (nLen > 255)
    nLen = 255;
  if (fLen > 255)
    fLen = 255;
  if (hLen > 255)
    hLen = 255;

  // Fail if the strings are too large to fit.
  if (max < 8 + nLen + fLen + hLen)
    return DbgErrCode::NO_MEMORY;

  int n = 0;
  data[n++] = static_cast<uint8_t>(var->GetType());
  data[n++] = 0;
  data[n++] = 0;
  data[n++] = 0;
  data[n++] = static_cast<uint8_t>(nLen);
  data[n++] = static_cast<uint8_t>(fLen);
  data[n++] = static_cast<uint8_t>(hLen);
  data[n++] = 0;
  memcpy(&data[n], var->GetName(), nLen);
  n += nLen;

  memcpy(&data[n], var->GetFormat(), fLen);
  n += fLen;

  memcpy(&data[n], var->GetHelp(), hLen);
  n += hLen;
  *len = n;
  return DbgErrCode::OK;
}

DbgErrCode VarCmd::GetVar(uint8_t *data, int *len, int max) {
  // We expect a 16-bit ID to be passed
  if (*len < 3)
    return DbgErrCode::MISSING_DATA;

  uint16_t vid = u8_to_u16(&data[1]);

  DebugVar *var = DebugVar::FindVar(vid);
  if (!var)
    return DbgErrCode::BAD_VARID;

  if (max < 4)
    return DbgErrCode::NO_MEMORY;

  u32_to_u8(var->GetValue(), data);
  *len = 4;
  return DbgErrCode::OK;
}

DbgErrCode VarCmd::SetVar(uint8_t *data, int *len, int max) {
  // We expect a 16-bit ID to be passed
  if (*len < 3)
    return DbgErrCode::MISSING_DATA;

  uint16_t vid = u8_to_u16(&data[1]);

  DebugVar *var = DebugVar::FindVar(vid);
  if (!var)
    return DbgErrCode::BAD_VARID;

  int ct = *len - 3;

  if (ct < 4)
    return DbgErrCode::MISSING_DATA;

  var->SetValue(u8_to_u32(data + 3));
  *len = 0;
  return DbgErrCode::OK;
}

VarCmd varCmd;
