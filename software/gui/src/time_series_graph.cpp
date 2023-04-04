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

#include "time_series_graph.h"

#include <QPainter>
#include <QPainterPath>

TimeSeriesGraph::TimeSeriesGraph(QQuickItem *parent) : QQuickPaintedItem(parent) {}

QVector<QPointF> TimeSeriesGraph::GetDataset() const { return dataset_; }

float TimeSeriesGraph::GetRangeInSeconds() const { return range_in_secs_; }

QColor TimeSeriesGraph::GetLineColor() const { return line_color_; }

QColor TimeSeriesGraph::GetAreaColor() const { return area_color_; }

bool TimeSeriesGraph::GetShowBaseline() const { return show_baseline_; }

float TimeSeriesGraph::GetBaselineValue() const { return baseline_value_; }

void TimeSeriesGraph::SetShowBaseline(bool value) {
  if (show_baseline_ != value) {
    show_baseline_ = value;
    emit ShowBaselineChanged();
  }
}

void TimeSeriesGraph::SetBaselineValue(float baseline) {
  if (baseline_value_ != baseline) {
    baseline_value_ = baseline;
    emit BaselineValueChanged();
  }
}

void TimeSeriesGraph::SetRangeInSeconds(float rangeInSeconds) {
  if (range_in_secs_ != rangeInSeconds) {
    range_in_secs_ = rangeInSeconds;
    emit RangeInSecondsChanged();
  }
}

void TimeSeriesGraph::SetLineColor(QColor color) {
  if (line_color_ != color) {
    line_color_ = color;
    emit LineColorChanged();
  }
}

void TimeSeriesGraph::SetAreaColor(QColor color) {
  if (area_color_ != color) {
    area_color_ = color;
    emit AreaColorChanged();
  }
}

void TimeSeriesGraph::SetDataset(QVector<QPointF> &dataset) {
  dataset_ = dataset;
  emit DatasetChanged();
  this->update();
}

float TimeSeriesGraph::GetMinValue() const { return min_value_; }

void TimeSeriesGraph::SetMinValue(float value) {
  if (min_value_ != value) {
    min_value_ = value;
    emit MinValueChanged();
  }
}

float TimeSeriesGraph::GetMaxValue() const { return max_value_; }

void TimeSeriesGraph::SetMaxValue(float value) {
  if (max_value_ != value) {
    max_value_ = value;
    emit MinValueChanged();
  }
}

float TimeSeriesGraph::calculateRealX(float timeX) {
  float result = width() * (1.0 + timeX / range_in_sec);
  return result;
}

float TimeSeriesGraph::calculateRealY(float value) {
  float ratio = (value - min_value_) / (max_value_ - min_value_);
  float result = height() - (ratio * height());
  return result;
}

void TimeSeriesGraph::paint(QPainter *painter) {
  QBrush brush(area_color_);
  QPen pen(line_color_);
  pen.setWidthF(2.0f);

  painter->setPen(Qt::NoPen);
  painter->setRenderHint(QPainter::Antialiasing);

  float w = size().width();
  float h = size().height();

  int size = dataset_.size();

  if (size < 2) return;

  QPainterPath path;
  path.moveTo(calculateRealX(dataset_[0].x()), calculateRealY(dataset_[0].y()));
  for (int i = 1; i < size; i++)
    path.lineTo(calculateRealX(dataset_[i].x()), calculateRealY(dataset_[i].y()));

  // Draw graph line
  painter->setPen(pen);
  painter->drawPath(path);

  // Draw graph background area
  auto path2 = path;
  path2.lineTo(w, h);
  path2.lineTo(calculateRealX(dataset_[0].x()), h);
  painter->setBrush(brush);
  painter->fillPath(path2, brush);

  // Draw baseline
  if (show_baseline_) {
    QPainterPath path3;
    path3.moveTo(calculateRealX(dataset_[0].x()), calculateRealY(baseline_value_));
    path3.lineTo(calculateRealX(dataset_[size - 1].x()), calculateRealY(baseline_value_));
    pen.setWidthF(1.0f);
    pen.setColor(baseline_color_);
    painter->setPen(pen);
    painter->drawPath(path3);
  }
}
