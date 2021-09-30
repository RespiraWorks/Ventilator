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
