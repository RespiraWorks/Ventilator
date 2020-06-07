#include "time_series_graph_painter.h"
#include "qnanocolor.h"
#include "time_series_graph.h"
#include <QDebug>
#include <QVarLengthArray>
#include <QtMath>
#include <algorithm>
#include <math.h>
TimeSeriesGraphPainter::TimeSeriesGraphPainter() {}

void TimeSeriesGraphPainter::paint(QNanoPainter *m_painter) {
  float w = width();
  float h = height();

  int size = dataset_.size();

  if (size < 2)
    return;

  // Draw graph line
  m_painter->beginPath();
  m_painter->moveTo(calculateRealX(dataset_[0].x()),
                    calculateRealY(dataset_[0].y()));
  for (int i = 1; i < size; i++)
    m_painter->lineTo(calculateRealX(dataset_[i].x()),
                      calculateRealY(dataset_[i].y()));

  m_painter->setFillStyle(color_fill_);
  m_painter->setStrokeStyle(color_line_);
  m_painter->setLineWidth(2.0f);
  m_painter->stroke();

  // Draw graph background area
  m_painter->beginPath();
  m_painter->moveTo(calculateRealX(dataset_[0].x()),
                    calculateRealY(dataset_[0].y()));
  for (int i = 1; i < size; i++)
    m_painter->lineTo(calculateRealX(dataset_[i].x()),
                      calculateRealY(dataset_[i].y()));

  m_painter->lineTo(w, h);
  m_painter->lineTo(calculateRealX(dataset_[0].x()), h);

  m_painter->fill();

  // Draw baseline
  if (show_baseline_) {
    m_painter->beginPath();
    m_painter->moveTo(calculateRealX(dataset_[0].x()),
                      calculateRealY(baseline_value_));
    m_painter->lineTo(calculateRealX(dataset_[size - 1].x()),
                      calculateRealY(baseline_value_));
    m_painter->setLineWidth(1.0f);
    m_painter->setStrokeStyle(baseline_color_);
    m_painter->stroke();
  }
}

void TimeSeriesGraphPainter::synchronize(QNanoQuickItem *item) {
  TimeSeriesGraph *realItem = static_cast<TimeSeriesGraph *>(item);

  if (!realItem)
    return;

  dataset_ = realItem->GetDataset();
  baseline_value_ = realItem->GetBaselineValue();
  show_baseline_ = realItem->GetShowBaseline();
  min_value_ = realItem->GetMinValue();
  max_value_ = realItem->GetMaxValue();
  range_in_sec = realItem->GetRangeInSeconds();
  color_line_ =
      QNanoColor(realItem->GetColor().red(), realItem->GetColor().green(),
                 realItem->GetColor().blue(), realItem->GetColor().alpha());

  color_fill_ = QNanoColor(color_line_);
  color_fill_.setAlpha(0x1A);
}
