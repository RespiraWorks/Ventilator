#include "datasource.h"
#include <QCommandLineParser>
#include <QtCore/QDir>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/Logo.png"));

  QCommandLineParser parser;
  parser.setApplicationDescription("Ventilator GUI application");
  parser.addHelpOption();
  //  parser.addVersionOption();

  QCommandLineOption dieOption(
      QStringList() << "d"
                    << "die",
      QApplication::translate("main", "Die right away (for testing)"));
  parser.addOption(dieOption);

  parser.process(app);

  bool die_now = parser.isSet(dieOption);

  QQuickView mainView;
  QString extraImportPath(QStringLiteral("%1/../../../%2"));

  mainView.engine()->addImportPath(extraImportPath.arg(
      QGuiApplication::applicationDirPath(), QString::fromLatin1("qml")));
  QObject::connect(mainView.engine(), &QQmlEngine::quit, &mainView,
                   &QWindow::close);
  mainView.setTitle(QStringLiteral("Ventilator"));

  // This will need to be a source coming from the arduino
  DataSource pressureDataSource(&mainView);
  // This will need to be a source coming from the arduino
  DataSource volumeDataSource(&mainView);
  // This data source will be calulated from pressure and volume on the pi
  DataSource flowDataSource(&mainView);

  mainView.rootContext()->setContextProperty("pressureDataSource",
                                             &pressureDataSource);
  mainView.rootContext()->setContextProperty("volumeDataSource",
                                             &volumeDataSource);
  mainView.rootContext()->setContextProperty("flowDataSource", &flowDataSource);

  mainView.setSource(QUrl("qrc:/main.qml"));
  mainView.setResizeMode(QQuickView::SizeRootObjectToView);
  mainView.setColor(QColor("#000000"));

  if (parser.isSet("h") || die_now) {
    return EXIT_SUCCESS;
  }

  mainView.show();
  return app.exec();
}
