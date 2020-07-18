#include "periodic_closure.h"

#include <iostream>

void PeriodicClosure::Start() {
  loop_thread_ = std::thread(&PeriodicClosure::Loop, this);
}

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
      if (stop_requested_cv_.wait_until(l, next_run) ==
          std::cv_status::timeout) {
        // Time for the next run.
        break;
      } else {
        // Either Stop() was called, or a spurious wake-up happened.
        // The next iteration of this loop will tell.
      }
    }
  }
}
