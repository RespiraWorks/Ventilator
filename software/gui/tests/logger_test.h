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

#include <QCoreApplication>
#include <QStandardPaths>
#include <QtDebug>
#include <QtTest>

#include "logger.h"

class LoggerTest : public QObject {
  Q_OBJECT
 public:
  LoggerTest() = default;
  ~LoggerTest() = default;

 private slots:
  void initTestCase() {
    auto log_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(log_path);
    auto log_file = log_path + "/gui.log";
    printf("Saving logs in %s\n", log_file.toLatin1().data());
    CustomLogger::initLogger(spdlog::level::trace, true, log_file.toStdString());
  }
  void cleanupTestCase() { CustomLogger::closeLogger(); }

  void testWriteCritical() { CRIT("Critical test message"); }

  void testWriteError() { ERR("Error test message"); }

  void testWriteWarning() { WARN("Warning test message"); }

  void testWriteInfo() { INFO("Info test message"); }

  void testWriteDebug() { DBG("Debug test message"); }

  void testWriteTrace() { TRC("Trace test message"); }
};
