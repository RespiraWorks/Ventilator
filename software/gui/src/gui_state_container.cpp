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

#include "gui_state_container.h"

GuiStateContainer::GuiStateContainer(DurationMs history_window, DurationMs granularity)
    : startup_time_(SteadyClock::now()), history_(history_window, granularity) {
  QObject::connect(this, &GuiStateContainer::params_changed, [this]() {
    // TODO: This could come from GUI alarm settings instead.
    // Source for +/-5 is this thread:
    // https://respiraworks.slack.com/archives/C011UMNUWGZ/p1592606104221700?thread_ts=1592603466.221100&cid=C011UMNUWGZ
    alarm_manager_.get_pip_exceeded_alarm()->SetThresholdCmH2O(commanded_pip_ + 5);
    alarm_manager_.get_pip_not_reached_alarm()->SetThresholdCmH2O(commanded_pip_ - 5);
  });
  // Set initial alarm parameters per above.
  params_changed();
}

bool GuiStateContainer::get_is_using_fake_data() const { return is_using_fake_data_; }
void GuiStateContainer::set_is_using_fake_data(bool value) { is_using_fake_data_ = value; }

SteadyInstant GuiStateContainer::GetStartupTime() { return startup_time_; }

GuiStatus GuiStateContainer::GetGuiStatus() {
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
  status.desired_params.inspiratory_expiratory_ratio = commanded_i_time_ / commanded_e_time;
  status.desired_params.fio2 = commanded_fio2_percent_ * 0.01;

  return status;
}

std::vector<std::tuple<SteadyInstant, ControllerStatus>>
GuiStateContainer::GetControllerStatusHistory() {
  return history_.GetHistory();
}

QVector<QPointF> GuiStateContainer::GetPressureSeries() const { return pressure_series_; }

bool GuiStateContainer::IsDebugBuild() const {
#ifdef QT_DEBUG
  return true;
#else
  return false;
#endif
}

void GuiStateContainer::SetPressureSeries(QVector<QPointF> &&series) {
  pressure_series_ = series;
  emit PressureSeriesChanged();
}

QVector<QPointF> GuiStateContainer::GetFlowSeries() const { return flow_series_; }

void GuiStateContainer::SetFlowSeries(QVector<QPointF> &&series) {
  flow_series_ = series;
  emit FlowSeriesChanged();
}

QVector<QPointF> GuiStateContainer::GetTidalSeries() const { return tidal_series_; }

void GuiStateContainer::SetTidalSeries(QVector<QPointF> &&series) {
  tidal_series_ = series;
  emit TidalSeriesChanged();
}

AlarmManager *GuiStateContainer::GetAlarmManager() { return &alarm_manager_; }

void GuiStateContainer::controller_status_changed(SteadyInstant now,
                                                  const ControllerStatus &status) {
  breath_signals_.Update(now, status);
  alarm_manager_.Update(now, status, breath_signals_);
  if (history_.Append(now, status)) {
    UpdateGraphs();
    measurements_changed();
  }
}

void GuiStateContainer::UpdateGraphs() {
  auto now = SteadyClock::now();
  QVector<QPointF> pressure_points, flow_points, tv_points;

  int history_size = history_.Size();

  pressure_points.reserve(history_size);
  flow_points.reserve(history_size);
  tv_points.reserve(history_size);

  for (const auto &[time, controller_status] : GetControllerStatusHistory()) {
    int neg_millis_ago = TimeAMinusB(time, now).count();
    pressure_points.append(
        QPointF(neg_millis_ago * 0.001, controller_status.sensor_readings.patient_pressure_cm_h2o));
    flow_points.append(QPointF(neg_millis_ago * 0.001,
                               // The graph should be in L/min, but the data is ml/min
                               0.001 * controller_status.sensor_readings.flow_ml_per_min));
    tv_points.append(QPointF(neg_millis_ago * 0.001, controller_status.sensor_readings.volume_ml));
  }
  SetPressureSeries(std::move(pressure_points));
  SetFlowSeries(std::move(flow_points));
  SetTidalSeries(std::move(tv_points));
}

int GuiStateContainer::get_battery_percentage() const {
  return battery_percentage_;
  // TODO: Figure our how battery will be implemented
  // and how to get estimation.
}
SimpleClock *GuiStateContainer::get_clock() const { return const_cast<SimpleClock *>(&clock_); }

qreal GuiStateContainer::get_measured_pressure() const {
  return history_.GetLastStatus().sensor_readings.patient_pressure_cm_h2o;
}

qreal GuiStateContainer::get_measured_flow() const {
  return 0.001 * history_.GetLastStatus().sensor_readings.flow_ml_per_min;
}
qreal GuiStateContainer::get_measured_tv() const {
  return history_.GetLastStatus().sensor_readings.volume_ml;
}

qreal GuiStateContainer::get_measured_rr() const {
  return (commanded_mode_ == VentilationMode::PRESSURE_CONTROL)
             ? commanded_rr_
             : breath_signals_.rr().value_or(commanded_rr_);
}

qreal GuiStateContainer::get_measured_peep() const {
  return breath_signals_.peep().value_or(commanded_peep_);
}

qreal GuiStateContainer::get_measured_pip() const {
  return breath_signals_.pip().value_or(commanded_pip_);
}

qreal GuiStateContainer::get_measured_ier() const {
  float breath_duration_sec = 60.0 / get_measured_rr();
  float commanded_e_time = breath_duration_sec - commanded_i_time_;
  return commanded_i_time_ / commanded_e_time;
}

qreal GuiStateContainer::get_measured_fio2_percent() const {
  return 100 * history_.GetLastStatus().sensor_readings.fio2;
}
