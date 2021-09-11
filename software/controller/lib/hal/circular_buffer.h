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

#pragma once

#include <cstdint>
#include <optional>

#include "hal.h"

// This class is a generic circular buffer with fixed size.
//
// Note that this class is used from both the main line of code
// and the interrupt handlers, so it needs to be thread safe.
// I'm disabling interrupts during the critical sections to
// ensure that's the case.
template <class T, size_t N>
class CircularBuffer {
  // Uses an array of size N+1 to hold all N elements of the buffer, as
  // buffer_[head_] is by definition inaccessible.
  // This also makes the template safe against N = 0.
  volatile T buffer_[N + 1];
  volatile int head_{0}, tail_{0};

 public:
  CircularBuffer() = default;

  // Return number of elements available in the buffer to read.
  size_t FullCount() const {
    BlockInterrupts block;
    /// \TODO: ssize_t is likely compiler-dependent; this function could be improved
    ssize_t ct = head_ - tail_;
    if (ct < 0) ct += N + 1;
    return static_cast<size_t>(ct);
  }

  // Return number of free spaces in the buffer where more
  // elements can be written.
  size_t FreeCount() const { return N - FullCount(); }

  bool IsFull() const { return FullCount() == N; }

  // Get the oldest element from the buffer, popping it from the buffer.
  std::optional<T> Get() {
    BlockInterrupts block;

    if (head_ == tail_) {
      return std::nullopt;
    }

    T val = std::move(buffer_[tail_++]);
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
    auto new_head = head_ + 1;
    if (new_head > N) {
      new_head = 0;
    }

    if (new_head == tail_) {
      return false;
    }

    buffer_[head_] = std::move(dat);
    head_ = new_head;
    return true;
  }

  void Flush() {
    BlockInterrupts block;
    head_ = tail_ = 0;
  }
};
