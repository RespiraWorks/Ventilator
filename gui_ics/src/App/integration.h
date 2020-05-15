#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <QObject>

#include <QQuickItem>
#include <QQmlEngine>

class AppCommandLineParser;

namespace GreenHouse {
class Context;
class ViewManager;
}
///
/// \brief After generating a GreenHouse flow using GreenHouse Tooling, the generated file
/// integration.cpp is used to hook up application objects to the GreenHouse context in the
/// mainWindowCreated event handler. The Integration class provides the application with an
/// application plugin manager, command line parser, main GreenHouse context, and QML engine.
///
class Integration : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Integration)
public:
    explicit Integration(QQmlEngine *engine, AppCommandLineParser *cmd, QObject *parent = nullptr);
    ~Integration();

    Q_INVOKABLE void mainWindowCreated(QQuickItem *mainContainer, QQuickWindow *window);

private:
    AppCommandLineParser *m_cmd = nullptr;
    QQmlEngine *m_engine = nullptr;
    GreenHouse::Context *m_context = nullptr;
    GreenHouse::ViewManager *m_viewManager = nullptr;
};

#endif // INTEGRATION_H
