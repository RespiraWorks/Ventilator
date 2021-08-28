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

#include <array>

#include "vars_base.h"

namespace Debug::Variable {

void Registry::register_variable(Base *var) {
  if (var_count_ >= static_cast<uint16_t>(std::size(var_list_))) return;
  var_list_[var_count_] = var;
  var->id_ = var_count_;
  var_count_++;
}

Base *Registry::find(uint16_t vid) {
  if (vid >= std::size(var_list_)) return nullptr;
  return var_list_[vid];
}

uint16_t Registry::count() const { return var_count_; }

}  // namespace Debug::Variable
