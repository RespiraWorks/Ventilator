#ifndef CONTROLLER_HISTORY_H
#define CONTROLLER_HISTORY_H

#include "../common/generated_libs/network_protocol/network_protocol.pb.h"
#include "chrono.h"

#include <deque>

// Maintains a history of recent ControllerStatus-es sufficient
// for rendering the UI.
//
// Non-thread-safe, needs external synchronization.
class ControllerHistory {
public:
  // Initializes the history to keep a window of given duration -
  // meaning, if the oldest point is more than this much older than
  // the point being added, it gets kicked out.
  //
  // TODO: Consider limiting also the total number of items, e.g. by
  // evicting items in the middle to maintain the overall thing more
  // or less uniformly sampled in time. This is harder to do efficiently
  // though.
  ControllerHistory(DurationMs window) : window_(window) {}

  // Appends a ControllerStatus obtained at a given time point in GUI time.
  // We cannot use the controller's uptime, because if controller restarts,
  // uptime will appear to go backwards.
  // For a similar reason we also must use specifically a steady clock
  // (clock that never goes backwards) - as opposed to, say, the system clock.
  void Append(SteadyInstant gui_now, const ControllerStatus &status) {
    history_.push_back({gui_now, status});
    // Kick out points that are too old.
    while (!history_.empty() &&
           gui_now - std::get<0>(history_.front()) > window_) {
      history_.pop_front();
    }
  }

  std::vector<std::tuple<SteadyInstant, ControllerStatus>> GetHistory() const {
    return {history_.begin(), history_.end()};
  }

  ControllerStatus GetLastStatus() const {
    if (history_.empty()) {
      return ControllerStatus_init_zero;
    }
    return std::get<1>(history_.back());
  }

private:
  DurationMs window_;
  std::deque<std::tuple<SteadyInstant, ControllerStatus>> history_;
};

#endif // CONTROLLER_HISTORY_H
