#include <QCoreApplication>
#include <QtTest>

#include "breath_signals_test.h"
#include "latching_alarm_test.h"
#include "logger_test.h"
#include "patient_detached_alarm_test.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  int status = 0;

  {
    LoggerTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    LatchingAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    BreathSignalsTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    PatientDetachedAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  return status;
}
