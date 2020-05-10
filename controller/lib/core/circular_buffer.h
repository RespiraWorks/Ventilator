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

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h>
#include "hal.h"

// This class is a generic circular buffer for byte sized data.
//
// Note that this class is used from both the main line of code
// and the interrupt handlers, so it needs to be thread safe.
// I'm disabling interrupts during the critical sections to
// ensure that's the case.
template <int N> class CircBuff {
  volatile uint8_t buff[N];
  volatile int head, tail;

public:
  CircBuff() { head = tail = 0; }

  // Return number of bytes available in the buffer to read.
  int FullCt() {
    bool p = Hal.IntSuspend();
    int ct = head - tail;
    Hal.IntRestore(p);
    if (ct < 0)
      ct += N;
    return ct;
  }

  // Return number of free spaces in the buffer where more
  // bytes can be written.
  int FreeCt() { return N - 1 - FullCt(); }

  // Get the oldest byte from the buffer.
  // Returns -1 if the buffer is empty
  int Get() {
    int ret = -1;

    bool p = Hal.IntSuspend();

    if (head != tail) {
      ret = buff[tail++];
      if (tail >= N)
        tail = 0;
    }

    Hal.IntRestore(p);
    return ret;
  }

  // Add a byte to the buffer
  // Returnes true on success, false if the buffer is full
  bool Put(uint8_t dat) {
    bool ret = false;

    bool p = Hal.IntSuspend();

    int h = head + 1;
    if (h >= N)
      h = 0;

    if (h != tail) {
      buff[head] = dat;
      head = h;
      ret = true;
    }

    Hal.IntRestore(p);

    return ret;
  }
};

#endif
