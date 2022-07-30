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

#if !defined(BARE_STM32)

#include "hal.h"

void HalApi::EarlyInit() {}
void HalApi::Init(Frequency cpu_frequency) {}
void HalApi::StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg) {}

#endif
