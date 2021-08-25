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

#include "vars_base.h"

#include <cstring>

DebugVarBase::DebugVarBase(VarType type, const char* name, VarAccess access, const char* units,
                           const char* help, const char* fmt)
    : type_(type), access_(access) {
  // \todo use stricpy instead?
  strcpy(name_, name);
  strcpy(help_, help);
  strcpy(units_, units);
  strcpy(fmt_, fmt);
}

const char* DebugVarBase::GetName() const { return name_; }

void DebugVarBase::prepend_name(const char* prefix) {
  // Assumes name_ has enough space allocated for the combined string.
  size_t len = strlen(prefix);
  memmove(name_ + len, name_, strlen(name_) + 1);
  memcpy(name_, prefix, len);
}

void DebugVarBase::append_help(const char* text) {
  // \todo use stricat instead?
  strcat(help_, text);
}

const char* DebugVarBase::GetFormat() const { return fmt_; }

const char* DebugVarBase::GetHelp() const { return help_; }

const char* DebugVarBase::GetUnits() const { return units_; }

VarType DebugVarBase::GetType() const { return type_; }

uint16_t DebugVarBase::GetId() const { return id_; }

VarAccess DebugVarBase::GetAccess() const { return access_; }

bool DebugVarBase::WriteAllowed() const { return (access_ == VarAccess::ReadWrite); }
