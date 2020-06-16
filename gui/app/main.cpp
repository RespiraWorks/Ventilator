#include "chrono.h"
#include "connected_device.h"
#include "controller_history.h"
#include "gui_state_container.h"
#include "periodic_closure.h"
#include "respira_connected_device.h"

#include <QCommandLineParser>
#include <QFontDatabase>
#include <QFontInfo>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QDir>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <cmath>
#include <iostream>
#include <memory>

#include "time_series_graph.h"

QObject *gui_state_instance(QQmlEngine *engine, QJSEngine *scriptEngine) {
  static GuiStateContainer state_container(
      /*history_window=*/DurationMs(30000));
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  // Since we are returning just a pointer, QQmlEngine does not know the object
  // is static and will try to take ownership of this object, unless we manually
  // tell the engine that the ownership will belong to Cpp side.
  engine->setObjectOwnership(&state_container, QQmlEngine::CppOwnership);
  return &state_container;
}

void install_fonts() {
  if (QFontDatabase::addApplicationFont(":/fonts/NotoSans-Regular.ttf") == -1)
    qWarning() << "Failed to load NatoSans-Regular.ttf";

  if (QFontDatabase::addApplicationFont(":/fonts/Oxygen-Regular.ttf") == -1)
    qWarning() << "Failed to load Oxygen-Regular.ttf";

  if (QFontDatabase::addApplicationFont(":/fonts/Oxygen-Bold.ttf") == -1)
    qWarning() << "Failed to load Oxygen-Bold.ttf";
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  app.setWindowIcon(QIcon(":/images/Logo.png"));

  QCommandLineParser parser;
  parser.setApplicationDescription("Ventilator GUI application");
  parser.addHelpOption();

  // Support this option so we can verify that the GUI can start up at all,
  // e.g. load all resources and so on.
  QCommandLineOption startupOnlyOption(
      QStringList() << "startup-only",
      QObject::tr("main", "Start up and exit successfully (for testing)"));

  QCommandLineOption serialPortOption(
      QStringList() << "serial-port",
      QObject::tr("main", "Serial port filename. "
                          "Uses pre-recorded test data if not set."));
  serialPortOption.setValueName("port");

  parser.addOption(startupOnlyOption);
  parser.addOption(serialPortOption);
  parser.process(app);

  GuiStateContainer *state_container =
      static_cast<GuiStateContainer *>(gui_state_instance(nullptr, nullptr));
  // Check out utils/mock-cycle-controller.py - it is
  // a script that bangs ControllerState at the set rate into serial
  // port.

  std::unique_ptr<ConnectedDevice> device;
  if (parser.isSet(serialPortOption)) {
    device = std::make_unique<RespiraConnectedDevice>(
        parser.value(serialPortOption));
  } else {
    // NOTE: The code below is specialized to this particular file.
    // The file also does not match the default (or, of course, current) mode
    // settings: settings used in the file are PIP 15, PEEP 5, RR 12, IE 2/3.
    std::vector<ControllerStatus> statuses;
    QFile file(":/sample-data/2020-05-14-pip15-peep5-rr12-ie23.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      std::cerr << "Failed to open sample data file";
      return 1;
    }
    int breath_id = 0;
    bool last_setpoint_was_5 = false;
    while (!file.atEnd()) {
      QString line{file.readLine()};
      line = line.trimmed();
      if (line.isEmpty() || line.startsWith("#"))
        continue;
      auto tokens = line.split(",").toVector();

      float fan_setpoint_cm_h2o = tokens[0].toFloat();
      if (fan_setpoint_cm_h2o == 5.0f) {
        if (!last_setpoint_was_5)
          breath_id++;
        last_setpoint_was_5 = true;
      } else {
        last_setpoint_was_5 = false;
      }

      ControllerStatus status = ControllerStatus_init_zero;
      status.sensor_readings.patient_pressure_cm_h2o = tokens[1].toFloat();
      status.sensor_readings.flow_ml_per_min = 1000 * tokens[4].toFloat();
      status.sensor_readings.volume_ml = 10 * tokens[5].toFloat();
      status.sensor_readings.breath_id = breath_id;
      statuses.push_back(status);
    }

    SteadyInstant base = SteadyClock::now();
    device = std::make_unique<FakeConnectedDevice>(
        [&](const GuiStatus &gui_status) {
          // For now, completely ignore gui_status.
          (void)gui_status;
        },
        [statuses, base](ControllerStatus *controller_status) {
          DurationMs elapsed = TimeAMinusB(SteadyClock::now(), base);
          // Lines in this file are 10ms apart.
          int i_status = elapsed.count() / 10;
          *controller_status = statuses[i_status % statuses.size()];
        });
  }

  // Run comm thread at the same time interval as Cycle Controller.
  std::mutex gui_status_mutex;
  GuiStatus gui_status = state_container->GetGuiStatus();
  QObject::connect(state_container, &GuiStateContainer::params_changed, [&]() {
    std::unique_lock<std::mutex> l(gui_status_mutex);
    gui_status = state_container->GetGuiStatus();
  });
  PeriodicClosure communicate(DurationMs(30), [&] {
    auto now = SteadyClock::now();
    ControllerStatus controller_status;
    if (device->ReceiveControllerStatus(&controller_status)) {
      QMetaObject::invokeMethod(state_container, [=]() {
        state_container->controller_status_changed(now, controller_status);
      });
    }
    GuiStatus status;
    {
      std::unique_lock<std::mutex> l(gui_status_mutex);
      status = gui_status;
    }
    device->SendGuiStatus(status);
  });
  communicate.Start();

  qmlRegisterType<TimeSeriesGraph>("Respira", 1, 0, "TimeSeriesGraph");
  qmlRegisterSingletonType<GuiStateContainer>(
      "Respira", 1, 0, "GuiStateContainer", &gui_state_instance);

  install_fonts();

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  if (parser.isSet(startupOnlyOption)) {
    return EXIT_SUCCESS;
  }

  return app.exec();
}
