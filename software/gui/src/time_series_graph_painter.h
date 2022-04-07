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

#include <QQuickItem>

#include "qnanocolor.h"
#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

class TimeSeriesGraphPainter : public QNanoQuickItemPainter {
 public:
  TimeSeriesGraphPainter();

  void paint(QNanoPainter *p);

  void synchronize(QNanoQuickItem *item);

 private:
  QVector<QPointF> dataset_;

  float max_value_{100};
  float min_value_{0};
  float range_in_sec{30};
  QNanoColor color_fill_{QNanoColor(255, 255, 255, 90)};
  QNanoColor color_gradient_end_{QNanoColor(255, 255, 255, 90)};
  QNanoColor color_line_{QNanoColor(255, 255, 255, 255)};
  QNanoLinearGradient bg;
  QNanoColor baseline_color_{QNanoColor("#13345B")};
  bool show_baseline_{true};

  float baseline_value_{0};

  float calculateRealX(float timeX);
  float calculateRealY(float value);
};
