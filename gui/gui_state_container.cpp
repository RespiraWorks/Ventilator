#include "gui_state_container.h"

#include <QtCharts/QXYSeries>

QT_CHARTS_USE_NAMESPACE

void GuiStateContainer::update(QAbstractSeries *pressure_series,
                               QAbstractSeries *flow_series,
                               QAbstractSeries *tv_series) {
  auto now = SteadyClock::now();

  QVector<QPointF> pressure_points, flow_points, tv_points;
  for (const auto &[time, controller_status] : GetControllerStatusHistory()) {
    int neg_millis_ago = TimeAMinusB(time, now).count();
    pressure_points.append(
        QPointF(neg_millis_ago * 0.001,
                controller_status.sensor_readings.patient_pressure_cm_h2o));
    flow_points.append(
        QPointF(neg_millis_ago * 0.001,
                controller_status.sensor_readings.flow_ml_per_min));
    tv_points.append(QPointF(neg_millis_ago * 0.001,
                             controller_status.sensor_readings.volume_ml));
  }
  qobject_cast<QXYSeries *>(pressure_series)
      ->replace(std::move(pressure_points));
  qobject_cast<QXYSeries *>(flow_series)->replace(std::move(flow_points));
  qobject_cast<QXYSeries *>(tv_series)->replace(std::move(tv_points));
}
