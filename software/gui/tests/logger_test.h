#pragma once

#include "logs.h"
#include <QCoreApplication>
#include <QtTest>

class LoggerTest : public QObject {
  Q_OBJECT
public:
  LoggerTest() = default;
  ~LoggerTest() = default;

private slots:
  void initTestCase() {
    qInstallMessageHandler(logOutputToFile);
  }
  void cleanupTestCase() {}

  void testWriteInfo() {
    qInfo() << "Info test message";
  }

};
