#include <QLoggingCategory>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>

#include "integration.h"

#include <appcommandlineparser.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("Ventilator"));
    app.setApplicationVersion(QStringLiteral("1.0.0"));
    app.setOrganizationName(QStringLiteral("ICS"));
    app.setOrganizationDomain(QStringLiteral("com.ics"));

    AppCommandLineParser cmdParser(app.arguments());
    QLoggingCategory::setFilterRules(cmdParser.loggingFilters());

    QQmlApplicationEngine engine;
    Integration *integration = new Integration(&engine, &cmdParser, &engine);
    engine.rootContext()->setContextProperty(QStringLiteral("Integration"), integration);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
