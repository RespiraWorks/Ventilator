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

#pragma once

#include <condition_variable>
#include <functional>
#include <thread>

#include "chrono.h"

// Executes a given lambda with a given interval between finishing one execution
// and scheduling the next one. The interval is a lower bound.
//
// TODO: This class desperately needs unit tests, testing e.g.:
//  - The closure is invoked with about the given interval
//  - The closure is never invoked after Stop() completes
//  - Stop() works correctly whether it's called while the closure runs or not
//  - What happens if you never call Start()
//  - etc.
class PeriodicClosure {
 public:
  // Initializes a PeriodicClosure to run the given lambda with a given
  // interval.
  //
  // Note that you have to explicitly call Start().
  PeriodicClosure(DurationMs interval, std::function<void()> fn);

  // Calls Stop().
  ~PeriodicClosure();

  // Begins periodically running the given lambda.
  void Start();
  // Signals that periodical execution must cease, and waits for it to cease.
  // After this completes, the lambda will never execute again.
  // Blocks, but normally only for as long as it takes the lambda to complete,
  // i.e. if the lambda is quick, this will return soon even if interval is
  // large.
  void Stop();

 private:
  void Loop();

  const DurationMs interval_;
  const std::function<void()> fn_;

  std::thread loop_thread_;

  std::mutex mu_;
  bool stop_requested_ = false;
  std::condition_variable stop_requested_cv_;
};
