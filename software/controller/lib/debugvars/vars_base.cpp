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

namespace Debug::Variable {

Base::Base(Type type, const char *name, Access access, const char *units, const char *help,
           const char *fmt)
    : type_(type), access_(access) {
  // \todo use stricpy instead?
  strcpy(name_, name);
  strcpy(help_, help);
  strcpy(units_, units);
  strcpy(fmt_, fmt);
  Registry::singleton().register_variable(this);
}

const char *Base::name() const { return name_; }

void Base::prepend_name(const char *prefix, bool strict) {
  // Assumes name_ has enough space allocated for the combined string.
  size_t length = strlen(prefix);
  constexpr char delimiter{'_'};
  if (!strict && prefix[length - 1] != delimiter) {
    memmove(name_ + 1, name_, strlen(name_) + 1);
    memcpy(name_, &delimiter, 1);
  }
  memmove(name_ + length, name_, strlen(name_) + 1);
  memcpy(name_, prefix, length);
}

void Base::append_help(const char *text, bool strict) {
  constexpr char delimiter{' '};
  // \todo use stricat instead?
  if (!strict && text[0] != delimiter) strncat(help_, &delimiter, 1);
  strcat(help_, text);
}

const char *Base::format() const { return fmt_; }

const char *Base::help() const { return help_; }

const char *Base::units() const { return units_; }

Type Base::type() const { return type_; }

uint16_t Base::id() const { return id_; }

Access Base::access() const { return access_; }

bool Base::write_allowed() const { return (access_ == Access::ReadWrite); }

}  // namespace Debug::Variable
