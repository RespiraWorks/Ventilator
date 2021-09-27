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

#include <QObject>
#include <QTime>
#include <QTimer>
/**
 * @brief Clock class that emit current datetime in a
 * given interval
 */
class SimpleClock : public QObject {
  Q_OBJECT
  Q_PROPERTY(int hour READ GetHour NOTIFY time_changed)
  Q_PROPERTY(int minute READ GetMinute NOTIFY time_changed)
  Q_PROPERTY(int second READ GetSecond NOTIFY time_changed)

 public:
  explicit SimpleClock(QObject *parent = nullptr) : QObject(parent) {
    connect(&timer_, &QTimer::timeout, this, &SimpleClock::update_timer);
    timer_.start(1000);
  }

  ~SimpleClock() { timer_.stop(); }

  int GetHour() const { return current_time_.hour(); }
  int GetMinute() const { return current_time_.minute(); }
  int GetSecond() const { return current_time_.second(); }

  void update_timer() {
    current_time_ = QTime::currentTime();
    emit time_changed();
  }

 signals:

  void updateIntervalChanged();
  void time_changed();
  void updated();

 private:
  QTime current_time_ = QTime::currentTime();
  QTimer timer_;
};
