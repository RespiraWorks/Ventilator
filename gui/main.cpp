#include "RespiraConnectedDevice.h"
#include "chrono.h"
#include "connected_device.h"
#include "controller_history.h"
#include "gui_state_container.h"
#include "periodic_closure.h"

#include <QCommandLineParser>
#include <QtCore/QDir>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>
#include <cmath>
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  app.setWindowIcon(QIcon(":/images/Logo.png"));

  QCommandLineParser parser;
  parser.setApplicationDescription("Ventilator GUI application");
  parser.addHelpOption();
  // Support this option so we can verify that the GUI can start up at all,
  // e.g. load all resources and so on.
  QCommandLineOption startupOnlyOption(
      QStringList() << "startup-only",
      QApplication::translate("main",
                              "Start up and exit successfully (for testing)"));
  parser.addOption(startupOnlyOption);
  parser.process(app);

  GuiStateContainer state_container(
      /*history_window=*/DurationMs(30000));
  auto startup_time = state_container.GetStartupTime();

  // TODO add a command line argument to control if GUI is connected
  // to a real serial port or to a fake implementation.
  // TODO add a serial port name command line argument

  // For now uncomment this line and comment instantiation below to run
  // in real mode. Check out utils/mock-cycle-controller.py - it is
  // a script that bangs ControllerState at the set rate into serial
  // port.

  // std::unique_ptr<ConnectedDevice> device =
  //     std::make_unique<RespiraConnectedDevice>("/dev/pts/6");

  std::unique_ptr<ConnectedDevice> device =
      std::make_unique<FakeConnectedDevice>(
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
            sensors->pressure_cm_h2o =
                15 + 10 * sin(controller_status->uptime_ms * 0.001);
            sensors->flow_ml_per_min =
                120 * sin(controller_status->uptime_ms * 0.003);
            sensors->volume_ml =
                1000 + 500 * sin(controller_status->uptime_ms * 0.002);
          });

  PeriodicClosure communicate(DurationMs(100), [&] {
    device->SendGuiStatus(state_container.GetGuiStatus());
    ControllerStatus controller_status;
    if (device->ReceiveControllerStatus(&controller_status)) {
      state_container.AppendHistory(controller_status);
    }
  });
  communicate.Start();

  QQuickView mainView;
  mainView.setTitle(QStringLiteral("Ventilator"));

  mainView.rootContext()->setContextProperty("guiState", &state_container);

  mainView.setSource(QUrl("qrc:/main.qml"));
  mainView.setResizeMode(QQuickView::SizeRootObjectToView);
  mainView.setColor(QColor("#000000"));

  mainView.showFullScreen();

  if (parser.isSet(startupOnlyOption)) {
    return EXIT_SUCCESS;
  }

  return app.exec();
}
