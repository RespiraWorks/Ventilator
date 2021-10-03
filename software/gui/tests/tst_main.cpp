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

#include <QCoreApplication>
#include <QtTest>

#include "alarm_manager_test.h"
#include "breath_signals_test.h"
#include "controller_history_test.h"
#include "gui_state_container_test.h"
#include "latching_alarm_test.h"
#include "logger_test.h"
#include "patient_detached_alarm_test.h"
#include "periodic_closure_test.h"
#include "pip_exceeded_alarm_test.h"
#include "pip_not_reached_alarm_test.h"
#include "respira_connected_device_test.h"
#include "simple_clock_test.h"
#include "time_series_graph_painter_test.h"
#include "time_series_graph_test.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  int status = 0;

  {
    AlarmManagerTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    BreathSignalsTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    ControllerHistoryTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    GuiStateContainerTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    LatchingAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    LoggerTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    PatientDetachedAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    PeriodicClosureTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    PipExceededAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    PipNotReachedAlarmTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    RespiraConnectedDeviceTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  {
    SimpleClockTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  //  {
  //    TimeSeriesGraphPainterTest tc;
  //    status += QTest::qExec(&tc, argc, argv);
  //  }

  {
    TimeSeriesGraphTest tc;
    status += QTest::qExec(&tc, argc, argv);
  }

  return status;
}
