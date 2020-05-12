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

#include "hal.h"
#include "sprintf.h"
#include <stdarg.h>

// Format a debug string printf sytle and write it to the debug
// serial port.  Note that this blocks if there isn't enough
// room in the transmit buffer
int debugPrint(const char *fmt, ...) {
  char buff[300];

  // Note that this uses a local sprintf implementation because
  // the one from the standard libraries will potentially dynamically
  // allocate memory.
  va_list ap;
  va_start(ap, fmt);
  int len = RWvsnprintf(buff, sizeof(buff), fmt, ap);
  va_end(ap);

  uint16_t remain = static_cast<uint16_t>(len);
  int ndx = 0;
  while (remain) {
    uint16_t ct = Hal.debugWrite(&buff[ndx], remain);
    remain = static_cast<uint16_t>(remain - ct);
    ndx += ct;
  }

  return len;
}
