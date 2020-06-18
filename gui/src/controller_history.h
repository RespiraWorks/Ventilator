#ifndef CONTROLLER_HISTORY_H
#define CONTROLLER_HISTORY_H

#include "chrono.h"
#include "network_protocol.pb.h"

#include <deque>
#include <tuple>
#include <vector>

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
  // "granularity" signals how many points to keep: if a new point is less than
  // this much later than the latest point, it doesn't get added.
  ControllerHistory(DurationMs window, DurationMs granularity)
      : window_(window), granularity_(granularity) {}

  // Appends a ControllerStatus obtained at a given time point in GUI time.
  // We cannot use the controller's uptime, because if controller restarts,
  // uptime will appear to go backwards.
  // For a similar reason we also must use specifically a steady clock
  // (clock that never goes backwards) - as opposed to, say, the system clock.
  bool Append(SteadyInstant gui_now, const ControllerStatus &status) {
    if (!history_.empty() &&
        gui_now - std::get<SteadyInstant>(history_.back()) < granularity_) {
      return false;
    }
    history_.push_back({gui_now, status});
    // Kick out points that are too old.
    while (!history_.empty() &&
           gui_now - std::get<SteadyInstant>(history_.front()) > window_) {
      history_.pop_front();
    }
    return true;
  }

  int Size() const { return history_.size(); }

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
  DurationMs granularity_;
  std::deque<std::tuple<SteadyInstant, ControllerStatus>> history_;
};

#endif // CONTROLLER_HISTORY_H
