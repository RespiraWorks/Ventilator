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

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include "hal.h"
#include <stdint.h>

// This class is a generic circular buffer with fixed size.
//
// Note that this class is used from both the main line of code
// and the interrupt handlers, so it needs to be thread safe.
// I'm disabling interrupts during the critical sections to
// ensure that's the case.
template <class T, int N> class CircBuff {
  volatile T buff[N];
  volatile int head, tail;

public:
  CircBuff() { head = tail = 0; }

  // Return number of elements available in the buffer to read.
  int FullCt() {
    BlockInterrupts block;
    int ct = head - tail;
    if (ct < 0)
      ct += N;
    return ct;
  }

  // Return number of free spaces in the buffer where more
  // elements can be written.
  int FreeCt() { return N - 1 - FullCt(); }

  // Get the oldest element from the buffer.
  // Returns -1 if the buffer is empty
  bool Get(T *val) {
    BlockInterrupts block;

    if (head == tail)
      return false;

    *val = buff[tail++];
    if (tail >= N)
      tail = 0;
    return true;
  }

  // Add an element to the buffer
  // Returns true on success, false if the buffer is full
  bool Put(T dat) {
    BlockInterrupts block;
    bool ret = false;
    int h = head + 1;
    if (h >= N)
      h = 0;

    if (h != tail) {
      buff[head] = dat;
      head = h;
      ret = true;
    }
    return ret;
  }

  void Flush() {
    BlockInterrupts block;
    head = tail = 0;
  }
};

#endif
