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
#ifndef NEW_H
#define NEW_H

#include <stddef.h>

// Enable this header only for AVR_UNO -- the STM32 nucleo board we're using
// has an STL.
//
// TODO: Remove this file once we no longer support AVR_UNO.
#if defined(ARDUINO_AVR_UNO)

// Placement new operator.  Provided by <new> when we have an STL; otherwise
// stubbed out here.
inline void *operator new(size_t, void *__ptr) { return __ptr; }

#endif // ARDUINO_AVR_UNO

#endif // NEW_H
