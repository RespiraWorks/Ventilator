#ifndef TIME_SERIES_GRAPH_H_
#define TIME_SERIES_GRAPH_H_

#include "time_series_graph_painter.h"
#include <QColor>
#include <QPointF>
#include <QQuickItem>
#include <QVector>

/**
 * @brief The TimeSeriesGraph is an QuickItem used to display a time series.
 */
class TimeSeriesGraph : public QNanoQuickItem {
  Q_OBJECT

  Q_PROPERTY(QVector<QPointF> dataset READ GetDataset WRITE SetDataset NOTIFY
                 DatasetChanged)
  Q_PROPERTY(
      float minValue READ GetMinValue WRITE SetMinValue NOTIFY MinValueChanged)
  Q_PROPERTY(
      float maxValue READ GetMaxValue WRITE SetMaxValue NOTIFY MaxValueChanged)
  Q_PROPERTY(float rangeInSeconds READ GetRangeInSeconds WRITE SetRangeInSeconds
                 NOTIFY RangeInSecondsChanged)
  Q_PROPERTY(QColor lineColor READ GetLineColor WRITE SetLineColor NOTIFY
                 LineColorChanged)
  Q_PROPERTY(QColor areaColor READ GetAreaColor WRITE SetAreaColor NOTIFY
                 AreaColorChanged)
  Q_PROPERTY(bool showBaseline READ GetShowBaseline WRITE SetShowBaseline NOTIFY
                 ShowBaselineChanged)
  Q_PROPERTY(float baselineValue READ GetBaselineValue WRITE SetBaselineValue
                 NOTIFY BaselineValueChanged)

public:
  TimeSeriesGraph(){};

  QNanoQuickItemPainter *createItemPainter() const {
    return new TimeSeriesGraphPainter();
  }

  QVector<QPointF> GetDataset() const { return dataset_; };

  float GetRangeInSeconds() const { return range_in_secs_; };

  QColor GetLineColor() const { return line_color_; };

  QColor GetAreaColor() const { return area_color_; };

  bool GetShowBaseline() const { return show_baseline_; }

  float GetBaselineValue() const { return baseline_value_; }

  void SetShowBaseline(bool value) {
    if (show_baseline_ != value) {
      show_baseline_ = value;
      emit ShowBaselineChanged();
    }
  }

public slots:

  void SetBaselineValue(float baseline) {
    if (baseline_value_ != baseline) {
      baseline_value_ = baseline;
      emit BaselineValueChanged();
    }
  }
  void SetRangeInSeconds(float rangeInSeconds) {
    if (range_in_secs_ != rangeInSeconds) {
      range_in_secs_ = rangeInSeconds;
      emit RangeInSecondsChanged();
    }
  }

  void SetLineColor(QColor color) {
    if (line_color_ != color) {
      line_color_ = color;
      emit LineColorChanged();
    }
  }

  void SetAreaColor(QColor color) {
    if (area_color_ != color) {
      area_color_ = color;
      emit AreaColorChanged();
    }
  }

  void SetDataset(QVector<QPointF> &dataset) {
    dataset_ = dataset;
    emit DatasetChanged();
    this->update();
  }

  float GetMinValue() const { return min_value_; };

  void SetMinValue(float value) {
    if (min_value_ != value) {
      min_value_ = value;
      emit MinValueChanged();
    }
  }

  float GetMaxValue() const { return max_value_; };

  void SetMaxValue(float value) {
    if (max_value_ != value) {
      max_value_ = value;
      emit MinValueChanged();
    }
  }

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

#endif // TIME_SERIES_GRAPH_H_
