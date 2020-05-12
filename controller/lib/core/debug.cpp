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
