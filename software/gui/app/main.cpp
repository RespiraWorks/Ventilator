#include "chrono.h"
#include "connected_device.h"
#include "controller_history.h"
#include "gui_state_container.h"
#include "latching_alarm.h"
#include "periodic_closure.h"
#include "respira_connected_device.h"

#include "logger.h"
#include <QStandardPaths>

#include "time_series_graph.h"
#include <QCommandLineParser>
#include <QDebug>
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

QObject *gui_state_instance(QQmlEngine *engine, QJSEngine *scriptEngine) {
  static GuiStateContainer state_container(
      /*history_window=*/DurationMs(30000),
      // 100ms looks a little janky, 50ms is fine.
      /*granularity=*/DurationMs(50));
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
    WARN("Failed to load NatoSans-Regular.ttf");

  if (QFontDatabase::addApplicationFont(":/fonts/Oxygen-Regular.ttf") == -1)
    WARN("Failed to load Oxygen-Regular.ttf");

  if (QFontDatabase::addApplicationFont(":/fonts/Oxygen-Bold.ttf") == -1)
    WARN("Failed to load Oxygen-Bold.ttf");
}

void init_logger(bool debug_mode) {
  auto log_path =
      QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
  QDir().mkpath(log_path);
  auto log_file = log_path + "/gui.log";
  printf("Saving logs in %s\n", log_file.toLatin1().data());
  if (debug_mode) {
    CustomLogger::initLogger(spdlog::level::trace, true,
                             log_file.toStdString());
  } else {
    CustomLogger::initLogger(spdlog::level::info, false,
                             log_file.toStdString());
  }
}

int main(int argc, char *argv[]) {
  QGuiApplication::setOrganizationName("RespiraWorks");
  QGuiApplication::setApplicationName("VentilatorUI");

#ifdef QT_DEBUG
  init_logger(true);
#else
  init_logger(false);
#endif

  INFO("VentilatorUI starting");
  INFO("  git version: {}", GIT_VERSION);

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
    state_container->set_is_using_fake_data(false);
    device = std::make_unique<RespiraConnectedDevice>(
        parser.value(serialPortOption));
  } else {
    state_container->set_is_using_fake_data(true);
    // NOTE: The code below is specialized to this particular file.
    std::vector<ControllerStatus> statuses;
    QFile file(":/sample-data/gui-sample-data.dat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      CRIT("Failed to open sample data file");
      return EXIT_FAILURE;
    }
    while (!file.atEnd()) {
      QString line{file.readLine()};
      line = line.trimmed();
      if (line.isEmpty() || line.startsWith("#") || line.startsWith("time"))
        continue;
      auto tokens = line.split(" ").toVector();

      ControllerStatus status = ControllerStatus_init_zero;
      status.sensor_readings.patient_pressure_cm_h2o = tokens[1].toFloat();
      // net flow in the file is in ml/sec.
      status.sensor_readings.flow_ml_per_min = 60 * tokens[2].toFloat();
      status.sensor_readings.volume_ml = tokens[3].toFloat();
      status.sensor_readings.breath_id = tokens[4].toInt();
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
  qmlRegisterUncreatableType<AlarmPriority>("Respira", 1, 0, "AlarmPriority",
                                            "is an enum");
  qmlRegisterUncreatableType<AlarmManager>(
      "Respira", 1, 0, "AlarmManager",
      "AlarmManager cannot be instantiated from QML");
  qmlRegisterUncreatableType<LatchingAlarm>(
      "Respira", 1, 0, "LatchingAlarm",
      "LatchingAlarm cannot be instantiated from QML");
  qmlRegisterSingletonType<GuiStateContainer>(
      "Respira", 1, 0, "GuiStateContainer", &gui_state_instance);

  install_fonts();

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  if (parser.isSet(startupOnlyOption)) {
    CRIT("VentilatorUI stated with startup-only. Shutting down immediately.");
    return EXIT_SUCCESS;
  }

  return app.exec();
}
