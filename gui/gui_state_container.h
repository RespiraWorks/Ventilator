#ifndef GUI_STATE_CONTAINER_H
#define GUI_STATE_CONTAINER_H

#include <QObject>
#include <stdint.h>

#include "chrono.h"
#include "controller_history.h"

#include <mutex>

#define RR_DEFAULT 0
#define PEEP_DEFAULT 0
#define PIP_DEFAULT 0
#define IER_DEFAULT 1.1

#define RR_ALARM_HIGH_DEFAULT 0
#define RR_ALARM_LOW_DEFAULT 0
#define TV_ALARM_HIGH_DEFAULT 0
#define TV_ALARM_LOW_DEFAULT 0


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

class GuiStateInterface : public QObject{
   Q_OBJECT
public:
    explicit GuiStateInterface (QObject* parent = 0) : QObject(parent) {}

    // Respiratory Rate

    Q_INVOKABLE void rr_inc(){
      rr_++;
    }

    Q_INVOKABLE void rr_dec(){
      if(rr_ > 0) rr_--;
    }

    Q_INVOKABLE quint32 rr_val(){
      return rr_;
    }

    Q_INVOKABLE quint32 rr_default(){
      return RR_DEFAULT;
    }

    // PIP

    Q_INVOKABLE void pip_inc(){
      pip_++;
    }

    Q_INVOKABLE void pip_dec(){
      if(pip_ > 0) pip_--;
    }

    Q_INVOKABLE quint32 pip_val(){
      return pip_;
    }

    Q_INVOKABLE quint32 pip_default(){
      return PIP_DEFAULT;
    }

    // PEEP

    Q_INVOKABLE void peep_inc(){
      peep_++;
    }

    Q_INVOKABLE void peep_dec(){
      if(peep_ > 0) peep_--;
    }

    Q_INVOKABLE quint32 peep_val(){
      return peep_;
    }

    Q_INVOKABLE quint32 peep_default(){
      return PEEP_DEFAULT;
    }

    // I:E ratio

    Q_INVOKABLE void ier_inc(){
      if(ier_ < 1.5) ier_+=0.1;
    }

    Q_INVOKABLE void ier_dec(){
      if(ier_ > 1.1) ier_-=0.1;
    }

    Q_INVOKABLE double ier_val(){
      return ier_;
    }

    Q_INVOKABLE double ier_default(){
      return IER_DEFAULT;
    }

    /*  Alarms */

    // RR High Alarm

    Q_INVOKABLE void rrAlarmHigh_inc(){
      rrAlarmHigh_++;
    }

    Q_INVOKABLE void rrAlarmHigh_dec(){
      if(rrAlarmHigh_ > 0) rrAlarmHigh_--;
    }

    Q_INVOKABLE quint32 rrAlarmHigh_val(){
      return rrAlarmHigh_;
    }

    Q_INVOKABLE quint32 rrAlarmHigh_default(){
      return RR_ALARM_HIGH_DEFAULT;
    }

    // RR Low Alarm

    Q_INVOKABLE void rrAlarmLow_inc(){
      rrAlarmLow_++;
    }

    Q_INVOKABLE void rrAlarmLow_dec(){
      if(rrAlarmLow_ > 0) rrAlarmLow_--;
    }

    Q_INVOKABLE quint32 rrAlarmLow_val(){
      return rrAlarmLow_;
    }

    Q_INVOKABLE quint32 rrAlarmLow_default(){
      return RR_ALARM_LOW_DEFAULT;
    }

    // TV High Alarm

    Q_INVOKABLE void tvAlarmHigh_inc(){
      tvAlarmHigh_++;
    }

    Q_INVOKABLE void tvAlarmHigh_dec(){
      if(tvAlarmHigh_ > 0) tvAlarmHigh_--;
    }

    Q_INVOKABLE quint32 tvAlarmHigh_val(){
      return tvAlarmHigh_;
    }

    Q_INVOKABLE quint32 tvAlarmHigh_default(){
      return TV_ALARM_HIGH_DEFAULT;
    }

    // RR Low Alarm

    Q_INVOKABLE void tvAlarmLow_inc(){
      tvAlarmLow_++;
    }

    Q_INVOKABLE void tvAlarmLow_dec(){
      if(tvAlarmLow_ > 0) tvAlarmLow_--;
    }

    Q_INVOKABLE quint32 tvAlarmLow_val(){
      return tvAlarmLow_;
    }

    Q_INVOKABLE quint32 tvAlarmLow_default(){
      return TV_ALARM_LOW_DEFAULT;
    }


private:
quint32 rr_ = RR_DEFAULT;
quint32 peep_ = PEEP_DEFAULT;
quint32 pip_ = PIP_DEFAULT;
double ier_ = IER_DEFAULT;
quint32 rrAlarmHigh_ = RR_ALARM_HIGH_DEFAULT;
quint32 rrAlarmLow_ = RR_ALARM_LOW_DEFAULT;
quint32 tvAlarmHigh_ = TV_ALARM_HIGH_DEFAULT;
quint32 tvAlarmLow_ = TV_ALARM_LOW_DEFAULT;
};

#endif // GUI_STATE_CONTAINER_H
