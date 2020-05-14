#pragma once

#include <limits>

#include <QCoreApplication>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <applicationpluginmanager.h>

#include <VentilatorUI/surfacecontroller.hpp>

using namespace testing;

class SurfaceControllerInterfaceImplTest : public Test
{
    // Test interface
protected:
    void SetUp();
    void TearDown();

    VentilatorUI::SurfaceController * iface;
    ApplicationPluginManager *m_applicationPluginManager;
    GreenHouse::Context m_ctx;
};

void SurfaceControllerInterfaceImplTest ::SetUp()
{
    QDir pluginDir;
    pluginDir = QDir(qApp->applicationDirPath());
    pluginDir.cdUp();
    pluginDir.cdUp();
    pluginDir.cd(QStringLiteral("plugins"));
    m_applicationPluginManager = new ApplicationPluginManager(pluginDir.absoluteFilePath(QStringLiteral("config.json")),
    pluginDir.absolutePath());
    if (m_applicationPluginManager)
        m_applicationPluginManager->inject(&m_ctx);
    iface = m_ctx.get<VentilatorUI::SurfaceController>();
    if (!iface) {
        iface = new VentilatorUI::SurfaceController;
        m_ctx.store(iface);
    }
}

void SurfaceControllerInterfaceImplTest ::TearDown()
{
    delete m_applicationPluginManager;
}
