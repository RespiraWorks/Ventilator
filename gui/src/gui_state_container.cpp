#include "gui_state_container.h"

void GuiStateContainer::update() {
  auto now = SteadyClock::now();
  QVector<QPointF> pressure_points, flow_points, tv_points;

  int history_size = history_.Size();

  pressure_points.reserve(history_size);
  flow_points.reserve(history_size);
  tv_points.reserve(history_size);

  for (const auto &[time, controller_status] : GetControllerStatusHistory()) {
    int neg_millis_ago = TimeAMinusB(time, now).count();
    pressure_points.append(
        QPointF(neg_millis_ago * 0.001,
                controller_status.sensor_readings.patient_pressure_cm_h2o));
    flow_points.append(
        QPointF(neg_millis_ago * 0.001,
                // The graph should be in L/min, but the data is ml/min
                0.001 * controller_status.sensor_readings.flow_ml_per_min));
    tv_points.append(QPointF(neg_millis_ago * 0.001,
                             controller_status.sensor_readings.volume_ml));
  }
  SetPressureSeries(std::move(pressure_points));
  SetFlowSeries(std::move(flow_points));
  SetTidalSeries(std::move(tv_points));
}
