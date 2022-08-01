/* Copyright 2020-2022, RespiraWorks

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

#pragma once

class Callback {
 public:
  inline Callback() = default;
  inline Callback(void *instance, void (*function)(void *))
      : instance_(instance), function_(function) {}
  inline explicit operator bool() const { return (function_ != nullptr); }
  inline void operator()() const { function_(instance_); }

 private:
  void *instance_{nullptr};
  void (*function_)(void *){nullptr};
};
