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

#include <stdint.h>

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QtCore/QObject>
#include <iostream>
#include <tuple>
#include <vector>

#include "alarm_manager.h"
#include "breath_signals.h"
#include "chrono.h"
#include "controller_history.h"
#include "logger.h"
#include "simple_clock.h"

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
  GuiStateContainer(DurationMs history_window, DurationMs granularity);

  bool get_is_using_fake_data() const;
  void set_is_using_fake_data(bool value);

  // Returns when the GUI started up.
  SteadyInstant GetStartupTime();

  // Returns the current GuiStatus to be sent to the controller.
  GuiStatus GetGuiStatus();

  // Returns the recent history of ControllerStatus.
  std::vector<std::tuple<SteadyInstant, ControllerStatus>> GetControllerStatusHistory();

  Q_PROPERTY(bool is_using_fake_data READ get_is_using_fake_data CONSTANT)
  // Measured parameters
  Q_PROPERTY(qreal measured_pressure READ get_measured_pressure NOTIFY measurements_changed)
  Q_PROPERTY(qreal measured_flow READ get_measured_flow NOTIFY measurements_changed)
  Q_PROPERTY(qreal measured_tv READ get_measured_tv NOTIFY measurements_changed)
  Q_PROPERTY(quint32 measured_rr READ get_measured_rr NOTIFY measurements_changed)
  Q_PROPERTY(quint32 measured_peep READ get_measured_peep NOTIFY measurements_changed)
  Q_PROPERTY(quint32 measured_pip READ get_measured_pip NOTIFY measurements_changed)
  Q_PROPERTY(qreal measured_ier READ get_measured_ier NOTIFY measurements_changed)
  Q_PROPERTY(qreal measured_fio2_percent READ get_measured_fio2_percent NOTIFY measurements_changed)

  // Graphs
  Q_PROPERTY(QVector<QPointF> pressureSeries READ GetPressureSeries NOTIFY PressureSeriesChanged)
  Q_PROPERTY(QVector<QPointF> flowSeries READ GetFlowSeries NOTIFY FlowSeriesChanged)
  Q_PROPERTY(QVector<QPointF> tidalSeries READ GetTidalSeries NOTIFY TidalSeriesChanged)
  Q_PROPERTY(AlarmManager *alarmManager READ GetAlarmManager NOTIFY AlarmManagerChanged)

  // Commanded parameters
  Q_PROPERTY(VentilationMode commanded_mode MEMBER commanded_mode_ NOTIFY params_changed)
  Q_PROPERTY(quint32 commanded_rr MEMBER commanded_rr_ NOTIFY params_changed)
  Q_PROPERTY(quint32 commanded_peep MEMBER commanded_peep_ NOTIFY params_changed)
  Q_PROPERTY(quint32 commanded_pip MEMBER commanded_pip_ NOTIFY params_changed)
  Q_PROPERTY(qreal commanded_i_time MEMBER commanded_i_time_ NOTIFY params_changed)
  Q_PROPERTY(qreal commanded_fio2_percent MEMBER commanded_fio2_percent_ NOTIFY params_changed)

  // Other properties
  Q_PROPERTY(int batteryPercentage READ get_battery_percentage NOTIFY battery_percentage_changed)
  Q_PROPERTY(SimpleClock *clock READ get_clock NOTIFY clock_changed)
  Q_PROPERTY(bool isDebugBuild READ IsDebugBuild NOTIFY IsDebugBuildChanged)

  QVector<QPointF> GetPressureSeries() const;

  bool IsDebugBuild() const;

  void SetPressureSeries(QVector<QPointF> &&series);

  QVector<QPointF> GetFlowSeries() const;

  void SetFlowSeries(QVector<QPointF> &&series);

  QVector<QPointF> GetTidalSeries() const;

  void SetTidalSeries(QVector<QPointF> &&series);

  AlarmManager *GetAlarmManager();

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
  void controller_status_changed(SteadyInstant now, const ControllerStatus &status);

  void UpdateGraphs();

 private:
  int get_battery_percentage() const;
  SimpleClock *get_clock() const;

  // ====================== Measured parameters ========================
  qreal get_measured_pressure() const;
  qreal get_measured_flow() const;
  qreal get_measured_tv() const;
  qreal get_measured_rr() const;
  qreal get_measured_peep() const;
  qreal get_measured_pip() const;
  qreal get_measured_ier() const;
  qreal get_measured_fio2_percent() const;

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
