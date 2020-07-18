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
#ifndef ALGORITHM_H
#define ALGORITHM_H

// Include this file instead of <algorithm>.
//
// It works around the fact that Arduino.h defines min/max macros, which causes
// it to conflict with std::min/max.
//
// TODO: Remove this and use plain <algorithm> once the `nucleo` (i.e. stm32 +
// Arduino API) platform is gone.

#if !defined(TEST_MODE) && !defined(BARE_STM32)
#include <Arduino.h>
#endif

#undef min
#undef max

#include <algorithm>

#endif // ALGORITHM_H
