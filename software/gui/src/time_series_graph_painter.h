#ifndef TIME_SERIES_GRAPH_PAINTER_H_
#define TIME_SERIES_GRAPH_PAINTER_H_

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"
#include <QQuickItem>
#include <qnanocolor.h>

class TimeSeriesGraphPainter : public QNanoQuickItemPainter {
public:
  TimeSeriesGraphPainter();

  void paint(QNanoPainter *p);

  void synchronize(QNanoQuickItem *item);

private:
  QVector<QPointF> dataset_;

  float max_value_ = 100;
  float min_value_ = 0;
  float range_in_sec = 30;
  QNanoColor color_fill_ = QNanoColor(255, 255, 255, 90);
  QNanoColor color_gradient_end_ = QNanoColor(255, 255, 255, 90);
  QNanoColor color_line_ = QNanoColor(255, 255, 255, 255);
  QNanoLinearGradient bg;
  QNanoColor baseline_color_ = QNanoColor("#13345B");
  bool show_baseline_ = true;

  float baseline_value_ = 0;

  float calculateRealX(float timeX) {
    float result = width() * (1.0 + timeX / range_in_sec);
    return result;
  }

  float calculateRealY(float value) {
    float ratio = (value - min_value_) / (max_value_ - min_value_);
    float result = height() - (ratio * height());
    return result;
  }
};

#endif // TIME_SERIES_GRAPH_PAINTER_H_
