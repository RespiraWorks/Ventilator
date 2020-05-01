#include "datasource.h"

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
  app.setWindowIcon(QIcon(":/Logo.png"));

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

  std::unique_ptr<ConnectedDevice> device =
      std::make_unique<FakeConnectedDevice>(
          [&](const GuiStatus &gui_status) {
            // For now, completely ignore gui_status.
            (void)gui_status;
          },
          [&](ControllerStatus *controller_status) {
            // Fill the status with fake data.
            controller_status->uptime_ms =
                TimeAMinusB(SteadyClock::now(), startup_time).count();
            auto *sensors = &controller_status->sensor_readings;
            sensors->pressure_cm_h2o =
                sin(controller_status->uptime_ms * 0.001);
            sensors->volume_ml = 0;
            sensors->flow_ml_per_min = 0;
          });

  // TODO: Bind the readable aspects of state_container to UI elements
  // TODO: Bind the writable aspects of state_container to parameters set in the
  // UI

  // TODO: Figure out whether asynchronously and periodically sending GUI status
  // and receiving controller status is the right thing to do here.
  PeriodicClosure send_gui_status(DurationMs(10), [&] {
    device->SendGuiStatus(state_container.GetGuiStatus());
  });
  send_gui_status.Start();

  PeriodicClosure maintain_history(DurationMs(10), [&] {
    ControllerStatus controller_status;
    device->ReceiveControllerStatus(&controller_status);
    state_container.AppendHistory(controller_status);
  });
  maintain_history.Start();

  auto generate_pressure = [&]() -> std::vector<std::tuple<float, float>> {
    // TODO: If this is invoked several times for several different graphs,
    // they will be slightly misaligned, because each gets its own value of
    // "now". This will stop being a problem once the TODO below about filling
    // in all 3 graphs at the same time is addressed.
    auto now = SteadyClock::now();

    // TODO: This makes a copy of the status history, and then it's copied
    // one more time into the DataSource. We should reduce the amount of copying
    // and allocation churn.
    std::vector<std::tuple<float, float>> res;
    for (const auto &[time, controller_status] :
         state_container.GetControllerStatusHistory()) {
      int neg_millis_ago = TimeAMinusB(time, now).count();
      res.push_back({neg_millis_ago * 0.001,
                     controller_status.sensor_readings.pressure_cm_h2o});
    }
    return res;
  };

  // TODO: Create lambdas for volume and flow as well
  // TODO: Throw away DataSource and make something nicer that can
  // fill in all 3 graphs with a single scan over GetControllerStatusHistory()
  // instead of 3 scans.
  DataSource pressureDataSource(generate_pressure);
  DataSource volumeDataSource(generate_pressure);
  DataSource flowDataSource(generate_pressure);

  QQuickView mainView;
  mainView.setTitle(QStringLiteral("Ventilator"));

  mainView.rootContext()->setContextProperty("pressureDataSource",
                                             &pressureDataSource);
  mainView.rootContext()->setContextProperty("volumeDataSource",
                                             &volumeDataSource);
  mainView.rootContext()->setContextProperty("flowDataSource", &flowDataSource);

  mainView.setSource(QUrl("qrc:/main.qml"));
  mainView.setResizeMode(QQuickView::SizeRootObjectToView);
  mainView.setColor(QColor("#000000"));

  mainView.show();

  if (parser.isSet(startupOnlyOption)) {
    return EXIT_SUCCESS;
  }

  return app.exec();
}
