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

QNanoQuickItemPainter *TimeSeriesGraph::createItemPainter() const {
  return new TimeSeriesGraphPainter();
}

QVector<QPointF> TimeSeriesGraph::GetDataset() const { return dataset_; };

float TimeSeriesGraph::GetRangeInSeconds() const { return range_in_secs_; };

QColor TimeSeriesGraph::GetLineColor() const { return line_color_; };

QColor TimeSeriesGraph::GetAreaColor() const { return area_color_; };

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

float TimeSeriesGraph::GetMinValue() const { return min_value_; };

void TimeSeriesGraph::SetMinValue(float value) {
  if (min_value_ != value) {
    min_value_ = value;
    emit MinValueChanged();
  }
}

float TimeSeriesGraph::GetMaxValue() const { return max_value_; };

void TimeSeriesGraph::SetMaxValue(float value) {
  if (max_value_ != value) {
    max_value_ = value;
    emit MinValueChanged();
  }
}
