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

#pragma once

#include <cstdint>

class Buzzer {
 public:
  void initialize(uint32_t cpu_frequency_hz);
  void off();
  void on(float volume);

 private:
  /// \TODO parametrize buzzer frequency at initialization
  uint32_t freqency_hz_{2400};
};
