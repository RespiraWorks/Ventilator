#pragma once

#include "logs.h"
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
    printf("Saving logs in %s\n", log_path.toLatin1().data());
    QDir().mkpath(log_path);
    FileLogger::singleton().set_path(log_path + "/gui.log");
    qInstallMessageHandler(logOutputToFile);
  }
  void cleanupTestCase() {}

  void testWriteInfo() { qInfo("Info test message"); }

  void testWriteWarning() { qWarning("Warning test message"); }

  void testWriteCritical() { qCritical("Critical test message"); }
};
