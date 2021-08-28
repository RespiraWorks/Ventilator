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

#include <stdint.h>
#include <string.h>

#include "commands.h"
#include "vars.h"

namespace Debug::Command {

ErrorCode VarHandler::Process(Context *context) {
  // The first byte of data is always required, this
  // gives the sub-command.
  if (context->request_length < 1) return ErrorCode::MissingData;

  Subcommand subcommand{context->request[0]};

  switch (subcommand) {
    // Return info about one of the variables.
    case Subcommand::GetInfo:
      return GetVarInfo(context);

    case Subcommand::Get:
      return GetVar(context);

    case Subcommand::Set:
      return SetVar(context);

    case Subcommand::GetCount:
      return GetVarCount(context);

    default:
      return ErrorCode::InvalidData;
  }
}

// Return info about one of the variables.
// The 16-bit variable ID is passed in.  These IDs are
// automatically assigned as variables are registered in the
// system starting with 0.  The Python code can read them
// all out until it gets an error code indicating that the
// passed ID is invalid.
ErrorCode VarHandler::GetVarInfo(Context *context) {
  // We expect a 16-bit ID to be passed
  if (context->request_length < 3) return ErrorCode::MissingData;

  uint16_t var_id = u8_to_u16(&context->request[1]);

  const auto *var = Variable::Registry::singleton().find(var_id);
  if (!var) return ErrorCode::UnknownVariable;

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
  size_t name_length = strlen(var->name());
  size_t format_length = strlen(var->format());
  size_t help_length = strlen(var->help());
  size_t unit_length = strlen(var->units());

  // Fail if the strings are too large to fit.
  if (context->max_response_length < 8 + name_length + format_length + help_length + unit_length)
    return ErrorCode::NoMemory;

  uint32_t count = 0;
  context->response[count++] = static_cast<uint8_t>(var->type());
  context->response[count++] = static_cast<uint8_t>(var->access());
  context->response[count++] = 0;
  context->response[count++] = 0;
  context->response[count++] = static_cast<uint8_t>(name_length);
  context->response[count++] = static_cast<uint8_t>(format_length);
  context->response[count++] = static_cast<uint8_t>(help_length);
  context->response[count++] = static_cast<uint8_t>(unit_length);
  memcpy(&context->response[count], var->name(), name_length);
  count += static_cast<uint32_t>(name_length);

  memcpy(&context->response[count], var->format(), format_length);
  count += static_cast<uint32_t>(format_length);

  memcpy(&context->response[count], var->help(), help_length);
  count += static_cast<uint32_t>(help_length);

  memcpy(&context->response[count], var->units(), unit_length);
  count += static_cast<uint32_t>(unit_length);

  context->response_length = count;
  *(context->processed) = true;
  return ErrorCode::None;
}

ErrorCode VarHandler::GetVar(Context *context) {
  // We expect a 16-bit ID to be passed
  if (context->request_length < 3) return ErrorCode::MissingData;

  uint16_t var_id = u8_to_u16(&context->request[1]);

  auto *var = Variable::Registry::singleton().find(var_id);
  if (!var) return ErrorCode::UnknownVariable;

  if (context->max_response_length < 4) return ErrorCode::NoMemory;

  u32_to_u8(var->get_value(), context->response);
  context->response_length = 4;
  *(context->processed) = true;
  return ErrorCode::None;
}

ErrorCode VarHandler::SetVar(Context *context) {
  // We expect a 16-bit ID to be passed
  if (context->request_length < 3) return ErrorCode::MissingData;

  uint16_t var_id = u8_to_u16(&context->request[1]);

  auto *var = Variable::Registry::singleton().find(var_id);
  if (!var) return ErrorCode::UnknownVariable;

  uint32_t count = context->request_length - 3;

  if (count < 4) return ErrorCode::MissingData;

  if (!var->write_allowed()) return ErrorCode::InternalError;

  var->set_value(u8_to_u32(context->request + 3));
  context->response_length = 0;
  *(context->processed) = true;
  return ErrorCode::None;
}

ErrorCode VarHandler::GetVarCount(Context *context) {
  if (context->max_response_length < 4) return ErrorCode::NoMemory;

  u32_to_u8(Variable::Registry::singleton().count(), context->response);
  context->response_length = 4;
  *(context->processed) = true;
  return ErrorCode::None;
}

}  // namespace Debug::Command
