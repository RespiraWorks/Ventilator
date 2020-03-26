#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtCore/QDir>
#include "datasource.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("Logo.png"));
    QQuickView mainView;

    QString extraImportPath(QStringLiteral("%1/../../../%2"));

    mainView.engine()->addImportPath(extraImportPath.arg(QGuiApplication::applicationDirPath(),
                                      QString::fromLatin1("qml")));
    QObject::connect(mainView.engine(), &QQmlEngine::quit, &mainView, &QWindow::close);
    mainView.setTitle(QStringLiteral("Ventilator"));

    DataSource pressureDataSource(&mainView); // This will need to be a source coming from the arduino

    DataSource volumeDataSource(&mainView); // This will need to be a source coming from the arduino

    mainView.rootContext()->setContextProperty("pressureDataSource", &pressureDataSource);
    mainView.rootContext()->setContextProperty("volumeDataSource", &volumeDataSource);

    mainView.setSource(QUrl("qrc:/main.qml"));
    mainView.setResizeMode(QQuickView::SizeRootObjectToView);
    mainView.setColor(QColor("#ffffff"));
    mainView.show();

    return app.exec();
}
