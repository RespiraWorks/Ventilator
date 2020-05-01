#ifndef GUI_STATE_CONTAINER_H
#define GUI_STATE_CONTAINER_H

#include "chrono.h"
#include "controller_history.h"

#include <mutex>

// A thread-safe container for readable and writable state
// of the GUI.
//
// The rest of the GUI must bind itself to accessors and mutators
// of this class - e.g. render graphs from GetControllerStatusHistory(),
// and when a parameter is changed in the UI, call a mutator on this
// object.
//
// In other words, this is essentially an MVC "Model".
//
// GUI rendering and callbacks happen asynchronously to interaction
// with the controller, so thread safety is important, and it is easiest
// to centralize it in the current class, simply protecting everything
// by a single mutex.
class GuiStateContainer {
public:
  // Initializes the state container to keep the history of controller
  // statuses in a given time window.
  GuiStateContainer(DurationMs history_window)
      : startup_time_(SteadyClock::now()), history_(history_window) {}

  // Returns when the GUI started up.
  SteadyInstant GetStartupTime() { return startup_time_; }

  // Adds a data point of controller status to the history.
  void AppendHistory(const ControllerStatus &status) {
    std::unique_lock<std::mutex> l(mu_);
    history_.Append(SteadyClock::now(), status);
  }

  // Returns the current GuiStatus to be sent to the controller.
  GuiStatus GetGuiStatus() {
    std::unique_lock<std::mutex> l(mu_);
    return gui_status_;
  }

  // TODO: Add mutators of GuiStatus when there are any UI elements
  // that change parameters.

  // Returns the recent history of ControllerStatus.
  std::vector<std::tuple<SteadyInstant, ControllerStatus>>
  GetControllerStatusHistory() {
    std::unique_lock<std::mutex> l(mu_);
    return history_.GetHistory();
  }

private:
  const SteadyInstant startup_time_;

  std::mutex mu_;
  // TODO: Use thread safety annotations here and throughout the project.
  // https://clang.llvm.org/docs/ThreadSafetyAnalysis.html
  // They only work with clang, so we'll need a wrapper macro to have them
  // be no-ops on gcc.
  GuiStatus gui_status_ = GuiStatus_init_zero;
  ControllerHistory history_;
};

#endif // GUI_STATE_CONTAINER_H
