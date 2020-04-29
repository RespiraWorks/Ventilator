#include "datasource.h"

#include <QCommandLineParser>
#include <QtCore/QDir>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>
#include <cmath>

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

  auto generate_data = []() -> std::vector<std::tuple<float, float>> {
    std::vector<std::tuple<float, float>> res;
    for (int i = 0; i < 1000; ++i) {
      res.push_back({i, sin(0.01 * i)});
    }
    return res;
  };

  // This will need to be a source coming from the arduino
  DataSource pressureDataSource(generate_data);
  // This will need to be a source coming from the arduino
  DataSource volumeDataSource(generate_data);
  // This data source will be calulated from pressure and volume on the pi
  DataSource flowDataSource(generate_data);

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
