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

#include <QColor>
#include <QPointF>
#include <QQuickItem>
#include <QVector>

#include "time_series_graph_painter.h"

/**
 * @brief The TimeSeriesGraph is an QuickItem used to display a time series.
 */
class TimeSeriesGraph : public QNanoQuickItem {
  Q_OBJECT

  Q_PROPERTY(QVector<QPointF> dataset READ GetDataset WRITE SetDataset NOTIFY DatasetChanged)
  Q_PROPERTY(float minValue READ GetMinValue WRITE SetMinValue NOTIFY MinValueChanged)
  Q_PROPERTY(float maxValue READ GetMaxValue WRITE SetMaxValue NOTIFY MaxValueChanged)
  Q_PROPERTY(float rangeInSeconds READ GetRangeInSeconds WRITE SetRangeInSeconds NOTIFY
                 RangeInSecondsChanged)
  Q_PROPERTY(QColor lineColor READ GetLineColor WRITE SetLineColor NOTIFY LineColorChanged)
  Q_PROPERTY(QColor areaColor READ GetAreaColor WRITE SetAreaColor NOTIFY AreaColorChanged)
  Q_PROPERTY(
      bool showBaseline READ GetShowBaseline WRITE SetShowBaseline NOTIFY ShowBaselineChanged)
  Q_PROPERTY(
      float baselineValue READ GetBaselineValue WRITE SetBaselineValue NOTIFY BaselineValueChanged)

 public:
  TimeSeriesGraph() = default;

  QNanoQuickItemPainter *createItemPainter() const;

  QVector<QPointF> GetDataset() const;

  float GetRangeInSeconds() const;

  QColor GetLineColor() const;

  QColor GetAreaColor() const;

  bool GetShowBaseline() const;

  float GetBaselineValue() const;

  void SetShowBaseline(bool value);

 public slots:

  void SetBaselineValue(float baseline);
  void SetRangeInSeconds(float rangeInSeconds);

  void SetLineColor(QColor color);

  void SetAreaColor(QColor color);

  void SetDataset(QVector<QPointF> &dataset);

  float GetMinValue() const;

  void SetMinValue(float value);

  float GetMaxValue() const;

  void SetMaxValue(float value);

 signals:
  void DatasetChanged();
  void MinValueChanged();
  void MaxValueChanged();
  void LineColorChanged();
  void AreaColorChanged();
  void RangeInSecondsChanged();
  void ShowBaselineChanged();
  void BaselineValueChanged();

 private:
  QVector<QPointF> dataset_;

  QColor line_color_ = QColor(255, 255, 255, 255);
  QColor area_color_ = QColor(255, 255, 255, 255);
  float max_value_ = 0;
  float min_value_ = 0;
  float range_in_secs_ = 30.0;
  bool show_baseline_ = true;
  float baseline_value_ = 0;
};
