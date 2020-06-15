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
      QObject::tr("main", "Serial port filename. Uses fake data if not set."));
  serialPortOption.setValueName("port");

  parser.addOption(startupOnlyOption);
  parser.addOption(serialPortOption);
  parser.process(app);

  GuiStateContainer *state_container =
      static_cast<GuiStateContainer *>(gui_state_instance(nullptr, nullptr));
  auto startup_time = state_container->GetStartupTime();
  // Check out utils/mock-cycle-controller.py - it is
  // a script that bangs ControllerState at the set rate into serial
  // port.

  std::unique_ptr<ConnectedDevice> device;
  if (parser.isSet(serialPortOption)) {
    device = std::make_unique<RespiraConnectedDevice>(
        parser.value(serialPortOption));
  } else {
    device = std::make_unique<FakeConnectedDevice>(
        [&](const GuiStatus &gui_status) {
          // For now, completely ignore gui_status.
          (void)gui_status;
          /* std::cout << "uptime_ms = " << gui_status.uptime_ms << ", "
          << "desired_params = {"
          << "mode = " << gui_status.desired_params.mode << ", "
          << "peep = " << gui_status.desired_params.peep_cm_h2o << ", "
          << "rr = " << gui_status.desired_params.breaths_per_min << ", "
          << "pip = " << gui_status.desired_params.pip_cm_h2o << ", "
          << "ier = " <<
          gui_status.desired_params.inspiratory_expiratory_ratio
          << "}" << std::endl; */
        },
        [&](ControllerStatus *controller_status) {
          // Fill the status with fake data.
          controller_status->uptime_ms =
              TimeAMinusB(SteadyClock::now(), startup_time).count();
          auto *sensors = &controller_status->sensor_readings;
          sensors->patient_pressure_cm_h2o =
              15 + 10 * sin(controller_status->uptime_ms * 0.001);
          sensors->flow_ml_per_min =
              120 * sin(controller_status->uptime_ms * 0.003);
          sensors->volume_ml =
              1000 + 500 * sin(controller_status->uptime_ms * 0.002);
        });
  }

  // Run comm thread at the same time interval as Cycle Controller.
  PeriodicClosure communicate(DurationMs(30), [&] {
    ControllerStatus controller_status;
    if (device->ReceiveControllerStatus(&controller_status)) {
      state_container->AppendHistory(controller_status);
    }
    device->SendGuiStatus(state_container->GetGuiStatus());
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
