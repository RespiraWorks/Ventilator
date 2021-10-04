/* Copyright 2020-2021, RespiraWorks

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

#include "periodic_closure.h"

#include <iostream>

// Initializes a PeriodicClosure to run the given lambda with a given
// interval.
//
// Note that you have to explicitly call Start().
PeriodicClosure::PeriodicClosure(DurationMs interval, std::function<void()> fn)
    : interval_(interval), fn_(std::move(fn)) {}

// Calls Stop().
PeriodicClosure::~PeriodicClosure() { Stop(); }

void PeriodicClosure::Start() { loop_thread_ = std::thread(&PeriodicClosure::Loop, this); }

void PeriodicClosure::Stop() {
  if (!loop_thread_.joinable()) {
    // Already stopped.
    return;
  }

  {
    std::unique_lock<std::mutex> l(mu_);
    stop_requested_ = true;
  }
  // notify_one() should be called without holding the mutex.
  stop_requested_cv_.notify_one();
  loop_thread_.join();
}

void PeriodicClosure::Loop() {
  while (true) {
    fn_();

    auto next_run = SteadyClock::now() + interval_;
    std::unique_lock<std::mutex> l(mu_);
    while (true) {
      if (stop_requested_) {
        // We're done.
        return;
      }
      if (stop_requested_cv_.wait_until(l, next_run) == std::cv_status::timeout) {
        // Time for the next run.
        break;
      } else {
        // Either Stop() was called, or a spurious wake-up happened.
        // The next iteration of this loop will tell.
      }
    }
  }
}
