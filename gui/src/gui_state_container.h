#ifndef GUI_STATE_CONTAINER_H
#define GUI_STATE_CONTAINER_H

#include <QObject>
#include <stdint.h>

#include "chrono.h"
#include "controller_history.h"
#include "simple_clock.h"

#include <tuple>
#include <vector>

#include <QPointF>
#include <QVector>
#include <QtCore/QObject>

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
// TODO(jkff, paulovap): This class embodies the "God object" antipattern. We
// should split it into several parts, with GuiStateContainer being only the
// entry point for them.
class GuiStateContainer : public QObject {
  Q_OBJECT

public:
  enum VentilationMode {
    COMMAND_PRESSURE,
    PRESSURE_ASSIST,
    HIGH_FLOW_NASAL_CANNULA,
  };
  Q_ENUM(VentilationMode)

  // Initializes the state container to keep the history of controller
  // statuses in a given time window.
  GuiStateContainer(DurationMs history_window)
      : startup_time_(SteadyClock::now()), history_(history_window) {
    // Initialize to default parameters like in
    // https://github.com/RespiraWorks/VentilatorSoftware/blob/89b817af/controller/src/main.cpp#L84

    auto *params = &gui_status_.desired_params;
    params->mode = VentMode_PRESSURE_CONTROL;
    params->peep_cm_h2o = 5;
    params->breaths_per_min = 12;
    params->pip_cm_h2o = 15;
    params->inspiratory_expiratory_ratio = 0.66;
  }

  // Returns when the GUI started up.
  SteadyInstant GetStartupTime() { return startup_time_; }

  // Returns the current GuiStatus to be sent to the controller.
  GuiStatus GetGuiStatus() {
    gui_status_.uptime_ms =
        TimeAMinusB(SteadyClock::now(), startup_time_).count();
    return gui_status_;
  }

  // Returns the recent history of ControllerStatus.
  std::vector<std::tuple<SteadyInstant, ControllerStatus>>
  GetControllerStatusHistory() {
    return history_.GetHistory();
  }

  Q_PROPERTY(
      qreal pressureReadout READ get_pressure_readout NOTIFY readouts_changed)
  Q_PROPERTY(qreal flowReadout READ get_flow_readout NOTIFY readouts_changed)
  Q_PROPERTY(qreal tvReadout READ get_tv_readout NOTIFY readouts_changed)

  Q_PROPERTY(
      VentilationMode mode READ get_mode WRITE set_mode NOTIFY params_changed)
  Q_PROPERTY(quint32 rr READ get_rr WRITE set_rr NOTIFY params_changed)
  Q_PROPERTY(quint32 peep READ get_peep WRITE set_peep NOTIFY params_changed)
  Q_PROPERTY(quint32 pip READ get_pip WRITE set_pip NOTIFY params_changed)
  Q_PROPERTY(qreal ier READ get_ier WRITE set_ier NOTIFY params_changed)
  Q_PROPERTY(int batteryPercentage READ get_battery_percentage NOTIFY
                 battery_percentage_changed)
  Q_PROPERTY(SimpleClock *clock READ get_clock NOTIFY clock_changed)

  Q_PROPERTY(QVector<QPointF> pressureSeries READ GetPressureSeries NOTIFY
                 PressureSeriesChanged)
  Q_PROPERTY(
      QVector<QPointF> flowSeries READ GetFlowSeries NOTIFY FlowSeriesChanged)
  Q_PROPERTY(QVector<QPointF> tidalSeries READ GetTidalSeries NOTIFY
                 TidalSeriesChanged)

  QVector<QPointF> GetPressureSeries() const { return pressure_series_; }

  void SetPressureSeries(QVector<QPointF> &&series) {
    pressure_series_ = series;
    emit PressureSeriesChanged();
  }

  QVector<QPointF> GetFlowSeries() const { return flow_series_; }

  void SetFlowSeries(QVector<QPointF> &&series) {
    flow_series_ = series;
    emit FlowSeriesChanged();
  }

  QVector<QPointF> GetTidalSeries() const { return tidal_series_; }

  void SetTidalSeries(QVector<QPointF> &&series) {
    tidal_series_ = series;
    emit TidalSeriesChanged();
  }

signals:
  void readouts_changed();
  void params_changed(const GuiStatus &status);
  void battery_percentage_changed();
  void clock_changed();
  void PressureSeriesChanged();
  void FlowSeriesChanged();
  void TidalSeriesChanged();

public slots:
  // Adds a data point of controller status to the history.
  void controller_status_changed(SteadyInstant now,
                                 const ControllerStatus &status) {
    history_.Append(now, status);
    readouts_changed();
  }

  void update();

private:
  int get_battery_percentage() const {
    return battery_percentage_;
    // TODO: Figure our how battery will be implemented
    // and how to get estimation.
  }
  SimpleClock *get_clock() const { return const_cast<SimpleClock *>(&clock_); }
  qreal get_pressure_readout() const {
    return history_.GetLastStatus().sensor_readings.patient_pressure_cm_h2o;
  }
  qreal get_flow_readout() const {
    return 0.001 * history_.GetLastStatus().sensor_readings.flow_ml_per_min;
  }
  qreal get_tv_readout() const {
    return history_.GetLastStatus().sensor_readings.volume_ml;
  }

  VentilationMode get_mode() const {
    switch (gui_status_.desired_params.mode) {
    case VentMode::VentMode_PRESSURE_CONTROL:
      return VentilationMode::COMMAND_PRESSURE;
    case VentMode::VentMode_PRESSURE_ASSIST:
      return VentilationMode::PRESSURE_ASSIST;
    case VentMode::VentMode_HIGH_FLOW_NASAL_CANNULA:
      return VentilationMode::HIGH_FLOW_NASAL_CANNULA;
    default:
      // Should never happen.
      return VentilationMode::COMMAND_PRESSURE;
    }
  }
  void set_mode(VentilationMode mode) {
    gui_status_.desired_params.mode = [&] {
      switch (mode) {
      case VentilationMode::COMMAND_PRESSURE:
        return VentMode::VentMode_PRESSURE_CONTROL;
      case VentilationMode::PRESSURE_ASSIST:
        return VentMode::VentMode_PRESSURE_ASSIST;
      case VentilationMode::HIGH_FLOW_NASAL_CANNULA:
        return VentMode::VentMode_HIGH_FLOW_NASAL_CANNULA;
      default:
        // Should never happen, keep unchanged.
        return gui_status_.desired_params.mode;
      }
    }();
    params_changed(gui_status_);
  }

  quint32 get_rr() const { return gui_status_.desired_params.breaths_per_min; }

  void set_rr(quint32 value) {
    gui_status_.desired_params.breaths_per_min = value;
    params_changed(gui_status_);
  }

  quint32 get_peep() const { return gui_status_.desired_params.peep_cm_h2o; }
  void set_peep(quint32 value) {
    gui_status_.desired_params.peep_cm_h2o = value;
    params_changed(gui_status_);
  }

  quint32 get_pip() const { return gui_status_.desired_params.pip_cm_h2o; }
  void set_pip(quint32 value) {
    gui_status_.desired_params.pip_cm_h2o = value;
    params_changed(gui_status_);
  }

  qreal get_ier() const {
    return gui_status_.desired_params.inspiratory_expiratory_ratio;
  }
  void set_ier(qreal value) {
    gui_status_.desired_params.inspiratory_expiratory_ratio = value;
    params_changed(gui_status_);
  }

  const SteadyInstant startup_time_;
  GuiStatus gui_status_ = GuiStatus_init_zero;
  ControllerHistory history_;
  int battery_percentage_ = 70;
  SimpleClock clock_;

  QVector<QPointF> pressure_series_;
  QVector<QPointF> flow_series_;
  QVector<QPointF> tidal_series_;
};

#endif // GUI_STATE_CONTAINER_H
