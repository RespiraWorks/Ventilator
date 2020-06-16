#include <QCoreApplication>
#include <QtTest>

#include "breath_signals_test.h"
#include "max_pressure_alarm_test.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  int status = 0;
  {
    MaxPressureAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    BreathSignalsTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  return status;
}
