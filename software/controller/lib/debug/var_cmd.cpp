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

#include "commands.h"
#include "vars.h"
#include <stdint.h>
#include <string.h>

namespace Debug::Command {

Error_Code VarHandler::Process(Context *context) {

  // The first byte of data is always required, this
  // gives the sub-command.
  if (context->request_length < 1)
    return Error_Code_MissingData;

  VarAccess_Subcmd subcommand{VarAccess_Subcmd(context->request[0])};

  switch (subcommand) {
  // Return info about one of the variables.
  case VarAccess_Subcmd_GetInfo:
    return GetVarInfo(context);

  case VarAccess_Subcmd_Get:
    return GetVar(context);

  case VarAccess_Subcmd_Set:
    return SetVar(context);

  case VarAccess_Subcmd_GetCount:
    return GetVarCount(context);

  default:
    return Error_Code_InvalidData;
  }
}

// Return info about one of the variables.
// The 16-bit variable ID is passed in.  These IDs are
// automatically assigned as variables are registered in the
// system starting with 0.  The Python code can read them
// all out until it gets an error code indicating that the
// passed ID is invalid.
Error_Code VarHandler::GetVarInfo(Context *context) {

  // We expect a 16-bit ID to be passed
  if (context->request_length < 3)
    return Error_Code_MissingData;

  uint16_t var_id = u8_to_u16(&context->request[1]);

  const auto *var = DebugVar::FindVar(var_id);
  if (!var)
    return Error_Code_UnknownVariable;

  // The info I return consists of the following:
  // <type> - 1 byte variable type code
  // <access> - 1 byte gives the possible access to that variable (read only?)
  // <reserved> - 2 reserved bytes for things we think of later
  // <name len> - 1 byte gives length of variable name string
  // <fmt len>  - 1 byte gives length of formation string
  // <help len> - 1 byte gives length of help string
  // <unit len> - 1 byte gives length of unit string
  // <name> - variable length name string
  // <fmt>  - variable length format string
  // <help> - variable length help string
  // <unit> - variable length unit string
  // The strings are not null terminated.
  size_t name_length = strlen(var->GetName());
  size_t format_length = strlen(var->GetFormat());
  size_t help_length = strlen(var->GetHelp());
  size_t unit_length = strlen(var->GetUnit());

  // Fail if the strings are too large to fit.
  if (context->max_response_length <
      8 + name_length + format_length + help_length + unit_length)
    return Error_Code_NoMemory;

  uint32_t count = 0;
  context->response[count++] = static_cast<uint8_t>(var->GetType());
  context->response[count++] = static_cast<uint8_t>(var->GetAccess());
  context->response[count++] = 0;
  context->response[count++] = 0;
  context->response[count++] = static_cast<uint8_t>(name_length);
  context->response[count++] = static_cast<uint8_t>(format_length);
  context->response[count++] = static_cast<uint8_t>(help_length);
  context->response[count++] = static_cast<uint8_t>(unit_length);
  memcpy(&context->response[count], var->GetName(), name_length);
  count += static_cast<uint32_t>(name_length);

  memcpy(&context->response[count], var->GetFormat(), format_length);
  count += static_cast<uint32_t>(format_length);

  memcpy(&context->response[count], var->GetHelp(), help_length);
  count += static_cast<uint32_t>(help_length);

  memcpy(&context->response[count], var->GetUnit(), unit_length);
  count += static_cast<uint32_t>(unit_length);

  context->response_length = count;
  *(context->processed) = true;
  return Error_Code_None;
}

Error_Code VarHandler::GetVar(Context *context) {
  // We expect a 16-bit ID to be passed
  if (context->request_length < 3)
    return Error_Code_MissingData;

  uint16_t var_id = u8_to_u16(&context->request[1]);

  auto *var = DebugVar::FindVar(var_id);
  if (!var)
    return Error_Code_UnknownVariable;

  if (context->max_response_length < 4)
    return Error_Code_NoMemory;

  u32_to_u8(var->GetValue(), context->response);
  context->response_length = 4;
  *(context->processed) = true;
  return Error_Code_None;
}

Error_Code VarHandler::SetVar(Context *context) {
  // We expect a 16-bit ID to be passed
  if (context->request_length < 3)
    return Error_Code_MissingData;

  uint16_t var_id = u8_to_u16(&context->request[1]);

  auto *var = DebugVar::FindVar(var_id);
  if (!var)
    return Error_Code_UnknownVariable;

  uint32_t count = context->request_length - 3;

  if (count < 4)
    return Error_Code_MissingData;

  if (!var->WriteAllowed())
    return Error_Code_InternalError;

  var->SetValue(u8_to_u32(context->request + 3));
  context->response_length = 0;
  *(context->processed) = true;
  return Error_Code_None;
}

Error_Code VarHandler::GetVarCount(Context *context) {
  if (context->max_response_length < 4)
    return Error_Code_NoMemory;

  u32_to_u8(DebugVarBase::GetVarCount(), context->response);
  context->response_length = 4;
  *(context->processed) = true;
  return Error_Code_None;
}

} // namespace Debug::Command
