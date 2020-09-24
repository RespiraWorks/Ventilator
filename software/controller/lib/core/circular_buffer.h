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
#include <optional>
#include <stdint.h>

// This class is a generic circular buffer with fixed size.
//
// Note that this class is used from both the main line of code
// and the interrupt handlers, so it needs to be thread safe.
// I'm disabling interrupts during the critical sections to
// ensure that's the case.
template <class T, uint N> class CircBuff {
  // Uses an array of size N+1 to hold all N elements of the buffer, as
  // buff_[head_] is by definition inaccessible.
  // This also makes the template safe against N = 0.
  volatile T buff_[N + 1];
  volatile int head_, tail_;

public:
  CircBuff() { head_ = tail_ = 0; }

  // Return number of elements available in the buffer to read.
  int FullCt() const {
    BlockInterrupts block;
    int ct = head_ - tail_;
    if (ct < 0)
      ct += N + 1;
    return ct;
  }

  // Return number of free spaces in the buffer where more
  // elements can be written.
  int FreeCt() const { return N - FullCt(); }

  // Get the oldest element from the buffer, popping it from the buffer.
  std::optional<T> Get() {
    BlockInterrupts block;

    if (head_ == tail_) {
      return std::nullopt;
    }

    T val = std::move(buff_[tail_++]);
    if (tail_ > N) {
      tail_ = 0;
    }
    return val;
  }

  // Add an element to the buffer.
  //
  // Returns false if the buffer is full.
  [[nodiscard]] bool Put(T dat) {
    BlockInterrupts block;
    int h = head_ + 1;
    if (h > N) {
      h = 0;
    }

    if (h == tail_) {
      return false;
    }

    buff_[head_] = std::move(dat);
    head_ = h;
    return true;
  }

  void Flush() {
    BlockInterrupts block;
    head_ = tail_ = 0;
  }
};

#endif
