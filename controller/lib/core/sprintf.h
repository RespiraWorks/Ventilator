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

#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdarg.h>
#include <stddef.h>

int RWsprintf(char *str, const char *format, ...);
int RWsnprintf(char *str, size_t size, const char *format, ...);
int RWvsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
