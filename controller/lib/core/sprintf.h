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

This file implements the HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.  Details of the processor's
peripherals can be found in the reference manual for that processor:
   https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

Details specific to the ARM processor used in this chip can be found in
the programmer's manual for the processor available here:
   https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf
*/

#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdarg.h>
#include <stddef.h>

int RWsprintf(char *str, const char *format, ...);
int RWsnprintf(char *str, size_t size, const char *format, ...);
int RWvsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
