#include "integration.h"

#include <appcommandlineparser.h>
#include <greenhousecontext.h>
#include <greenhouseviewmanager.h>

#include <VentilatorUI/setup.hpp>

#include <QQmlContext>
#include <QQuickWindow>

#include <QCoreApplication>

Integration::Integration(QQmlEngine *engine, AppCommandLineParser *cmd, QObject *parent)
  : QObject(parent)
  , m_cmd(cmd)
  , m_engine(engine)
{
    VentilatorUI::setup();
}

Integration::~Integration() { }

void Integration::mainWindowCreated(QQuickItem *mainContainer, QQuickWindow *window)
{
    if (m_context) {
        // NOTE: We are dealing with a UI reload scenario, if we already have a valid context
        // instance
        m_context->setRootItem(mainContainer);
        m_viewManager->recreateUi();
    } else {
        // TODO: Instantiate all presenter objects and feed them to the qml context
        m_context =
                new GreenHouse::Context(m_engine, QStringLiteral("VentilatorUI"), mainContainer);
        m_viewManager =
                new GreenHouse::ViewManager(m_context, QStringLiteral(":/uiDescriptor.json"), this);

        m_context->qmlContext()->setContextProperty(QStringLiteral("ViewManager"), m_viewManager);
        // TODO: Instantiate all presenter objects and feed them to the qml context

        m_viewManager->start();
    }
    if (window)
        window->show();
}
