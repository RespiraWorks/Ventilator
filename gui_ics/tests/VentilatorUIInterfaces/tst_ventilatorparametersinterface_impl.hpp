#pragma once

#include <limits>

#include <QCoreApplication>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <applicationpluginmanager.h>

#include <VentilatorUI/ventilatorparametersinterface.hpp>

using namespace testing;

class VentilatorParametersInterfaceInterfaceImplTest : public Test
{
    // Test interface
protected:
    void SetUp();
    void TearDown();

    VentilatorUI::VentilatorParametersInterface *iface;
    ApplicationPluginManager *m_applicationPluginManager;
    GreenHouse::Context m_ctx;
};

void VentilatorParametersInterfaceInterfaceImplTest ::SetUp()
{
    QDir pluginDir;
    pluginDir = QDir(qApp->applicationDirPath());
    pluginDir.cdUp();
    pluginDir.cdUp();
    pluginDir.cd(QStringLiteral("plugins"));
    m_applicationPluginManager = new ApplicationPluginManager(
            pluginDir.absoluteFilePath(QStringLiteral("config.json")), pluginDir.absolutePath());
    if (m_applicationPluginManager)
        m_applicationPluginManager->inject(&m_ctx);
    iface = m_ctx.get<VentilatorUI::VentilatorParametersInterface>();
    if (!iface) {
        iface = new VentilatorUI::VentilatorParametersInterface;
        m_ctx.store(iface);
    }
}

void VentilatorParametersInterfaceInterfaceImplTest ::TearDown()
{
    delete m_applicationPluginManager;
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, tidalV_Imp)
{
    auto oldValue = iface->tidalV();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setTidalV(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalV(), oldValue);
    ASSERT_EQ(iface->tidalV(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, tidalVpopup_Imp)
{
    auto oldValue = iface->tidalVpopup();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVpopupChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setTidalVpopup(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalVpopup(), oldValue);
    ASSERT_EQ(iface->tidalVpopup(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, peep_Imp)
{
    auto oldValue = iface->peep();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setPeep(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peep(), oldValue);
    ASSERT_EQ(iface->peep(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, peepPopup_Imp)
{
    auto oldValue = iface->peepPopup();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepPopupChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setPeepPopup(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peepPopup(), oldValue);
    ASSERT_EQ(iface->peepPopup(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, rrMin_Imp)
{
    auto oldValue = iface->rrMin();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setRrMin(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMin(), oldValue);
    ASSERT_EQ(iface->rrMin(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, rrMinPopup_Imp)
{
    auto oldValue = iface->rrMinPopup();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinPopupChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setRrMinPopup(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMinPopup(), oldValue);
    ASSERT_EQ(iface->rrMinPopup(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, acpMode_Imp)
{
    auto oldValue = iface->acpMode();
    auto newValue = QStringLiteral("Lorem Ipsum Test STR");

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::acpModeChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setAcpMode(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->acpMode(), oldValue);
    ASSERT_EQ(iface->acpMode(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceImplTest, alertFlag_Imp)
{
    auto oldValue = iface->alertFlag();
    auto newValue = !oldValue;

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::alertFlagChanged,
                     [&]() { changeSignalEmitted = true; });

    iface->setAlertFlag(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->alertFlag(), oldValue);
    ASSERT_EQ(iface->alertFlag(), newValue);
}
