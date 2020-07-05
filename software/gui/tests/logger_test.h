#pragma once

#include "logger.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QtDebug>
#include <QtTest>

class LoggerTest : public QObject {
  Q_OBJECT
public:
  LoggerTest() = default;
  ~LoggerTest() = default;

private slots:
  void initTestCase() {
    auto log_path =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(log_path);
    auto log_file = log_path + "/gui.log";
    printf("Saving logs in %s\n", log_file.toLatin1().data());
    CustomLogger::initLogger(spdlog::level::trace, true,
                             log_file.toStdString());
  }
  void cleanupTestCase() { CustomLogger::closeLogger(); }

  void testWriteCritical() { CRIT("Critical test message"); }

  void testWriteError() { ERR("Error test message"); }

  void testWriteWarning() { WARN("Warning test message"); }

  void testWriteInfo() { INFO("Info test message"); }

  void testWriteDebug() { DBG("Debug test message"); }

  void testWriteTrace() { TRC("Trace test message"); }
};
