#ifndef GUI_STATE_CONTAINER_H
#define GUI_STATE_CONTAINER_H

#include <QObject>
#include <stdint.h>

#include "alarm_manager.h"
#include "breath_signals.h"
#include "chrono.h"
#include "controller_history.h"
#include "simple_clock.h"

#include <iostream>
#include <tuple>
#include <vector>

#include "logger.h"
#include <QPointF>
#include <QVector>
#include <QtCore/QObject>

// A container for readable and writable state of the GUI.
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
    PRESSURE_CONTROL,
    PRESSURE_ASSIST,
    HIGH_FLOW_NASAL_CANNULA,
  };
  Q_ENUM(VentilationMode)

  // Initializes the state container to keep the history of controller
  // statuses in a given time window with given granularity.
  GuiStateContainer(DurationMs history_window, DurationMs granularity)
      : startup_time_(SteadyClock::now()),
        history_(history_window, granularity) {
    QObject::connect(this, &GuiStateContainer::params_changed, [this]() {
      // TODO: This could come from GUI alarm settings instead.
      // Source for +/-5 is this thread:
      // https://respiraworks.slack.com/archives/C011UMNUWGZ/p1592606104221700?thread_ts=1592603466.221100&cid=C011UMNUWGZ
      alarm_manager_.get_pip_exceeded_alarm()->SetThresholdCmH2O(
          commanded_pip_ + 5);
      alarm_manager_.get_pip_not_reached_alarm()->SetThresholdCmH2O(
          commanded_pip_ - 5);
    });
    // Set initial alarm parameters per above.
    params_changed();
  }

  bool get_is_using_fake_data() const { return is_using_fake_data_; }
  void set_is_using_fake_data(bool value) { is_using_fake_data_ = value; }

  // Returns when the GUI started up.
  SteadyInstant GetStartupTime() { return startup_time_; }

  // Returns the current GuiStatus to be sent to the controller.
  GuiStatus GetGuiStatus() {
    GuiStatus status = GuiStatus_init_zero;

    status.uptime_ms = TimeAMinusB(SteadyClock::now(), startup_time_).count();
    status.desired_params.mode = [&] {
      switch (commanded_mode_) {
      case VentilationMode::PRESSURE_CONTROL:
        return VentMode::VentMode_PRESSURE_CONTROL;
      case VentilationMode::PRESSURE_ASSIST:
        return VentMode::VentMode_PRESSURE_ASSIST;
      case VentilationMode::HIGH_FLOW_NASAL_CANNULA:
        return VentMode::VentMode_HIGH_FLOW_NASAL_CANNULA;
      default:
        // Should never happen.
        CRIT("Unexpected commanded_mode: {}", commanded_mode_);
        return VentMode::VentMode_PRESSURE_CONTROL;
      }
    }();
    status.desired_params.peep_cm_h2o = commanded_peep_;
    status.desired_params.breaths_per_min = commanded_rr_;
    status.desired_params.pip_cm_h2o = commanded_pip_;
    float breath_duration_sec = 60.0 / commanded_rr_;
    float commanded_e_time = breath_duration_sec - commanded_i_time_;
    status.desired_params.inspiratory_expiratory_ratio =
        commanded_i_time_ / commanded_e_time;
    status.desired_params.fio2 = commanded_fio2_percent_ * 0.01;

    return status;
  }

  // Returns the recent history of ControllerStatus.
  std::vector<std::tuple<SteadyInstant, ControllerStatus>>
  GetControllerStatusHistory() {
    return history_.GetHistory();
  }

  Q_PROPERTY(bool is_using_fake_data READ get_is_using_fake_data CONSTANT)
  // Measured parameters
  Q_PROPERTY(qreal measured_pressure READ get_measured_pressure NOTIFY
                 measurements_changed)
  Q_PROPERTY(
      qreal measured_flow READ get_measured_flow NOTIFY measurements_changed)
  Q_PROPERTY(qreal measured_tv READ get_measured_tv NOTIFY measurements_changed)
  Q_PROPERTY(
      quint32 measured_rr READ get_measured_rr NOTIFY measurements_changed)
  Q_PROPERTY(
      quint32 measured_peep READ get_measured_peep NOTIFY measurements_changed)
  Q_PROPERTY(
      quint32 measured_pip READ get_measured_pip NOTIFY measurements_changed)
  Q_PROPERTY(
      qreal measured_ier READ get_measured_ier NOTIFY measurements_changed)
  Q_PROPERTY(qreal measured_fio2_percent READ get_measured_fio2_percent NOTIFY
                 measurements_changed)

  // Graphs
  Q_PROPERTY(QVector<QPointF> pressureSeries READ GetPressureSeries NOTIFY
                 PressureSeriesChanged)
  Q_PROPERTY(
      QVector<QPointF> flowSeries READ GetFlowSeries NOTIFY FlowSeriesChanged)
  Q_PROPERTY(QVector<QPointF> tidalSeries READ GetTidalSeries NOTIFY
                 TidalSeriesChanged)
  Q_PROPERTY(AlarmManager *alarmManager READ GetAlarmManager NOTIFY
                 AlarmManagerChanged)

  // Commanded parameters
  Q_PROPERTY(VentilationMode commanded_mode MEMBER commanded_mode_ NOTIFY
                 params_changed)
  Q_PROPERTY(quint32 commanded_rr MEMBER commanded_rr_ NOTIFY params_changed)
  Q_PROPERTY(
      quint32 commanded_peep MEMBER commanded_peep_ NOTIFY params_changed)
  Q_PROPERTY(quint32 commanded_pip MEMBER commanded_pip_ NOTIFY params_changed)
  Q_PROPERTY(
      qreal commanded_i_time MEMBER commanded_i_time_ NOTIFY params_changed)
  Q_PROPERTY(qreal commanded_fio2_percent MEMBER commanded_fio2_percent_ NOTIFY
                 params_changed)

  // Other properties
  Q_PROPERTY(int batteryPercentage READ get_battery_percentage NOTIFY
                 battery_percentage_changed)
  Q_PROPERTY(SimpleClock *clock READ get_clock NOTIFY clock_changed)
  Q_PROPERTY(bool isDebugBuild READ IsDebugBuild NOTIFY IsDebugBuildChanged)

  QVector<QPointF> GetPressureSeries() const { return pressure_series_; }

  bool IsDebugBuild() const {
#ifdef QT_DEBUG
    return true;
#else
    return false;
#endif
  }
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

  AlarmManager *GetAlarmManager() { return &alarm_manager_; }

signals:
  void measurements_changed();
  void params_changed();
  void battery_percentage_changed();
  void clock_changed();
  void PressureSeriesChanged();
  void FlowSeriesChanged();
  void TidalSeriesChanged();
  void IsDebugBuildChanged();
  void AlarmManagerChanged();

public slots:
  // Adds a data point of controller status to the history.
  void controller_status_changed(SteadyInstant now,
                                 const ControllerStatus &status) {
    breath_signals_.Update(now, status);
    alarm_manager_.Update(now, status, breath_signals_);
    if (history_.Append(now, status)) {
      UpdateGraphs();
      measurements_changed();
    }
  }

  void UpdateGraphs();

private:
  int get_battery_percentage() const {
    return battery_percentage_;
    // TODO: Figure our how battery will be implemented
    // and how to get estimation.
  }
  SimpleClock *get_clock() const { return const_cast<SimpleClock *>(&clock_); }

  // ====================== Measured parameters ========================
  qreal get_measured_pressure() const {
    return history_.GetLastStatus().sensor_readings.patient_pressure_cm_h2o;
  }
  qreal get_measured_flow() const {
    return 0.001 * history_.GetLastStatus().sensor_readings.flow_ml_per_min;
  }
  qreal get_measured_tv() const {
    return history_.GetLastStatus().sensor_readings.volume_ml;
  }
  qreal get_measured_rr() const {
    return (commanded_mode_ == VentilationMode::PRESSURE_CONTROL)
               ? commanded_rr_
               : breath_signals_.rr().value_or(commanded_rr_);
  }
  qreal get_measured_peep() const {
    return breath_signals_.peep().value_or(commanded_peep_);
  }
  qreal get_measured_pip() const {
    return breath_signals_.pip().value_or(commanded_pip_);
  }
  qreal get_measured_ier() const {
    float breath_duration_sec = 60.0 / get_measured_rr();
    float commanded_e_time = breath_duration_sec - commanded_i_time_;
    return commanded_i_time_ / commanded_e_time;
  }
  qreal get_measured_fio2_percent() const {
    return 100 * history_.GetLastStatus().sensor_readings.fio2;
  }

  const SteadyInstant startup_time_ = SteadyClock::now();
  bool is_using_fake_data_ = false;
  ControllerHistory history_;
  BreathSignals breath_signals_;
  int battery_percentage_ = 70;
  SimpleClock clock_;

  QVector<QPointF> pressure_series_;
  QVector<QPointF> flow_series_;
  QVector<QPointF> tidal_series_;

  // Commanded parameters
  // Initialize to default parameters like in
  // https://github.com/RespiraWorks/Ventilator/blob/89b817af/controller/src/main.cpp#L84
  VentilationMode commanded_mode_ = VentilationMode::PRESSURE_CONTROL;
  quint32 commanded_rr_ = 12;
  quint32 commanded_pip_ = 15;
  quint32 commanded_peep_ = 5;
  qreal commanded_i_time_ = 1.0;
  // https://respiraworks.slack.com/archives/C011UMNUWGZ/p1592608246223200?thread_ts=1592603466.221100&cid=C011UMNUWGZ
  qreal commanded_fio2_percent_ = 21.0;

  AlarmManager alarm_manager_;
};

#endif // GUI_STATE_CONTAINER_H
