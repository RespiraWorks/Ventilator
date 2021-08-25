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

// Prepends t into s. Assumes s has enough space allocated for the combined string.
void prepend(char* s, const char* t) {
  size_t len = strlen(t);
  memmove(s + len, s, strlen(s) + 1);
  memcpy(s, t, len);
}

DebugVarBase::DebugVarBase(VarType type, const char* name, VarAccess access, const char* units,
                           const char* help, const char* fmt)
    : type_(type), access_(access), units_(units), help_(help), fmt_(fmt) {
  strcpy(name_, name);
}

const char* DebugVarBase::GetName() const { return name_; }

void DebugVarBase::prepend_name(const char* prefix) { prepend(name_, prefix); }

const char* DebugVarBase::GetFormat() const { return fmt_; }

const char* DebugVarBase::GetHelp() const { return help_; }

const char* DebugVarBase::GetUnits() const { return units_; }

VarType DebugVarBase::GetType() const { return type_; }

uint16_t DebugVarBase::GetId() const { return id_; }

VarAccess DebugVarBase::GetAccess() const { return access_; }

bool DebugVarBase::WriteAllowed() const { return (access_ == VarAccess::ReadWrite); }
