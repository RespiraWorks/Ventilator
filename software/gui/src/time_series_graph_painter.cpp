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

#include "time_series_graph_painter.h"

#include <QDebug>
#include <QVarLengthArray>
#include <QtMath>
#include <algorithm>
#include <cmath>

#include "time_series_graph.h"

TimeSeriesGraphPainter::TimeSeriesGraphPainter() {}

void TimeSeriesGraphPainter::paint(QNanoPainter *m_painter) {
  float w = width();
  float h = height();

  int size = dataset_.size();

  if (size < 2) return;

  // Draw graph line
  m_painter->beginPath();
  m_painter->moveTo(calculateRealX(dataset_[0].x()), calculateRealY(dataset_[0].y()));
  for (int i = 1; i < size; i++)
    m_painter->lineTo(calculateRealX(dataset_[i].x()), calculateRealY(dataset_[i].y()));

  m_painter->setFillStyle(color_fill_);
  m_painter->setStrokeStyle(color_line_);
  m_painter->setLineWidth(2.0f);
  m_painter->stroke();

  // Draw graph background area
  m_painter->beginPath();
  m_painter->moveTo(calculateRealX(dataset_[0].x()), calculateRealY(dataset_[0].y()));
  for (int i = 1; i < size; i++)
    m_painter->lineTo(calculateRealX(dataset_[i].x()), calculateRealY(dataset_[i].y()));

  m_painter->lineTo(w, h);
  m_painter->lineTo(calculateRealX(dataset_[0].x()), h);

  m_painter->fill();

  // Draw baseline
  if (show_baseline_) {
    m_painter->beginPath();
    m_painter->moveTo(calculateRealX(dataset_[0].x()), calculateRealY(baseline_value_));
    m_painter->lineTo(calculateRealX(dataset_[size - 1].x()), calculateRealY(baseline_value_));
    m_painter->setLineWidth(1.0f);
    m_painter->setStrokeStyle(baseline_color_);
    m_painter->stroke();
  }
}

void TimeSeriesGraphPainter::synchronize(QNanoQuickItem *item) {
  TimeSeriesGraph *realItem = static_cast<TimeSeriesGraph *>(item);

  if (!realItem) return;

  dataset_ = realItem->GetDataset();
  baseline_value_ = realItem->GetBaselineValue();
  show_baseline_ = realItem->GetShowBaseline();
  min_value_ = realItem->GetMinValue();
  max_value_ = realItem->GetMaxValue();
  range_in_sec = realItem->GetRangeInSeconds();
  color_line_ = QNanoColor(realItem->GetLineColor().red(), realItem->GetLineColor().green(),
                           realItem->GetLineColor().blue(), realItem->GetLineColor().alpha());
  color_fill_ = QNanoColor(realItem->GetAreaColor().red(), realItem->GetAreaColor().green(),
                           realItem->GetAreaColor().blue(), realItem->GetAreaColor().alpha());
}

float TimeSeriesGraphPainter::calculateRealX(float timeX) {
  float result = width() * (1.0 + timeX / range_in_sec);
  return result;
}

float TimeSeriesGraphPainter::calculateRealY(float value) {
  float ratio = (value - min_value_) / (max_value_ - min_value_);
  float result = height() - (ratio * height());
  return result;
}
