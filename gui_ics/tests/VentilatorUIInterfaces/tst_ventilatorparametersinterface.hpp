#pragma once

#include <limits>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <VentilatorUI/ventilatorparametersinterface.hpp>

using namespace testing;

class VentilatorParametersInterfaceInterfaceTest : public Test
{
    // Test interface
protected:
    void SetUp();
    void TearDown();

    VentilatorUI::VentilatorParametersInterface *iface;
};

void VentilatorParametersInterfaceInterfaceTest::SetUp()
{
    iface = new VentilatorUI::VentilatorParametersInterface;
}

void VentilatorParametersInterfaceInterfaceTest::TearDown()
{
    delete iface;
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, startPreUseTest)
{

    iface->startPreUseTest();
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, setPressureMode)
{

    iface->setPressureMode(-1);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, adjustTidalV)
{

    iface->adjustTidalV(-1);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, adjustPeep)
{

    iface->adjustPeep(-1);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, adjustRrMin)
{

    iface->adjustRrMin(-1);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, tidalV)
{
    auto oldValue = iface->tidalV();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVChanged, [&]() { changeSignalEmitted = true; });

    iface->setTidalV(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalV(), oldValue);
    ASSERT_EQ(iface->tidalV(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, tidalVpopup)
{
    auto oldValue = iface->tidalVpopup();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVpopupChanged, [&]() { changeSignalEmitted = true; });

    iface->setTidalVpopup(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalVpopup(), oldValue);
    ASSERT_EQ(iface->tidalVpopup(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, peep)
{
    auto oldValue = iface->peep();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepChanged, [&]() { changeSignalEmitted = true; });

    iface->setPeep(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peep(), oldValue);
    ASSERT_EQ(iface->peep(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, peepPopup)
{
    auto oldValue = iface->peepPopup();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepPopupChanged, [&]() { changeSignalEmitted = true; });

    iface->setPeepPopup(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peepPopup(), oldValue);
    ASSERT_EQ(iface->peepPopup(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, rrMin)
{
    auto oldValue = iface->rrMin();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinChanged, [&]() { changeSignalEmitted = true; });

    iface->setRrMin(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMin(), oldValue);
    ASSERT_EQ(iface->rrMin(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, rrMinPopup)
{
    auto oldValue = iface->rrMinPopup();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinPopupChanged, [&]() { changeSignalEmitted = true; });

    iface->setRrMinPopup(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMinPopup(), oldValue);
    ASSERT_EQ(iface->rrMinPopup(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, alertFlag)
{
    auto oldValue = iface->alertFlag();
    auto newValue = !oldValue;

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::alertFlagChanged, [&]() { changeSignalEmitted = true; });

    iface->setAlertFlag(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->alertFlag(), oldValue);
    ASSERT_EQ(iface->alertFlag(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, preUseTestPassed)
{
    auto oldValue = iface->preUseTestPassed();
    auto newValue = !oldValue;

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::preUseTestPassedChanged, [&]() { changeSignalEmitted = true; });

    iface->setPreUseTestPassed(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->preUseTestPassed(), oldValue);
    ASSERT_EQ(iface->preUseTestPassed(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, showSettings)
{
    auto oldValue = iface->showSettings();
    auto newValue = !oldValue;

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::showSettingsChanged, [&]() { changeSignalEmitted = true; });

    iface->setShowSettings(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->showSettings(), oldValue);
    ASSERT_EQ(iface->showSettings(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, acpMode)
{
    auto oldValue = iface->acpMode();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::acpModeChanged, [&]() { changeSignalEmitted = true; });

    iface->setAcpMode(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->acpMode(), oldValue);
    ASSERT_EQ(iface->acpMode(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, acpModeName)
{
    auto oldValue = iface->acpModeName();
    auto newValue = QStringLiteral("Lorem Ipsum Test STR");

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::acpModeNameChanged, [&]() { changeSignalEmitted = true; });

    iface->setAcpModeName(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->acpModeName(), oldValue);
    ASSERT_EQ(iface->acpModeName(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, currentRate)
{
    auto oldValue = iface->currentRate();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::currentRateChanged, [&]() { changeSignalEmitted = true; });

    iface->setCurrentRate(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->currentRate(), oldValue);
    ASSERT_EQ(iface->currentRate(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, currentPeep)
{
    auto oldValue = iface->currentPeep();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::currentPeepChanged, [&]() { changeSignalEmitted = true; });

    iface->setCurrentPeep(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->currentPeep(), oldValue);
    ASSERT_EQ(iface->currentPeep(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, currentTidalV)
{
    auto oldValue = iface->currentTidalV();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::currentTidalVChanged, [&]() { changeSignalEmitted = true; });

    iface->setCurrentTidalV(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->currentTidalV(), oldValue);
    ASSERT_EQ(iface->currentTidalV(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, currentAdditionalValue)
{
    auto oldValue = iface->currentAdditionalValue();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::currentAdditionalValueChanged, [&]() { changeSignalEmitted = true; });

    iface->setCurrentAdditionalValue(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->currentAdditionalValue(), oldValue);
    ASSERT_EQ(iface->currentAdditionalValue(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, powerSourceType)
{
    auto oldValue = iface->powerSourceType();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::powerSourceTypeChanged, [&]() { changeSignalEmitted = true; });

    iface->setPowerSourceType(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->powerSourceType(), oldValue);
    ASSERT_EQ(iface->powerSourceType(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, powerSourceAsset)
{
    auto oldValue = iface->powerSourceAsset();
    auto newValue = QStringLiteral("Lorem Ipsum Test STR");

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::powerSourceAssetChanged, [&]() { changeSignalEmitted = true; });

    iface->setPowerSourceAsset(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->powerSourceAsset(), oldValue);
    ASSERT_EQ(iface->powerSourceAsset(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, batteryPercentageText)
{
    auto oldValue = iface->batteryPercentageText();
    auto newValue = QStringLiteral("Lorem Ipsum Test STR");

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::batteryPercentageTextChanged, [&]() { changeSignalEmitted = true; });

    iface->setBatteryPercentageText(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->batteryPercentageText(), oldValue);
    ASSERT_EQ(iface->batteryPercentageText(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, batteryPercentageAsset)
{
    auto oldValue = iface->batteryPercentageAsset();
    auto newValue = QStringLiteral("Lorem Ipsum Test STR");

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::batteryPercentageAssetChanged, [&]() { changeSignalEmitted = true; });

    iface->setBatteryPercentageAsset(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->batteryPercentageAsset(), oldValue);
    ASSERT_EQ(iface->batteryPercentageAsset(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, batteryLevel)
{
    auto oldValue = iface->batteryLevel();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::batteryLevelChanged, [&]() { changeSignalEmitted = true; });

    iface->setBatteryLevel(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->batteryLevel(), oldValue);
    ASSERT_EQ(iface->batteryLevel(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, alertText)
{
    auto oldValue = iface->alertText();
    auto newValue = QStringLiteral("Lorem Ipsum Test STR");

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::alertTextChanged, [&]() { changeSignalEmitted = true; });

    iface->setAlertText(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->alertText(), oldValue);
    ASSERT_EQ(iface->alertText(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, tidalVLow)
{
    auto oldValue = iface->tidalVLow();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVLowChanged, [&]() { changeSignalEmitted = true; });

    iface->setTidalVLow(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalVLow(), oldValue);
    ASSERT_EQ(iface->tidalVLow(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, tidalVHigh)
{
    auto oldValue = iface->tidalVHigh();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVHighChanged, [&]() { changeSignalEmitted = true; });

    iface->setTidalVHigh(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalVHigh(), oldValue);
    ASSERT_EQ(iface->tidalVHigh(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, peepLow)
{
    auto oldValue = iface->peepLow();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepLowChanged, [&]() { changeSignalEmitted = true; });

    iface->setPeepLow(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peepLow(), oldValue);
    ASSERT_EQ(iface->peepLow(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, peepHigh)
{
    auto oldValue = iface->peepHigh();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepHighChanged, [&]() { changeSignalEmitted = true; });

    iface->setPeepHigh(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peepHigh(), oldValue);
    ASSERT_EQ(iface->peepHigh(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, tidalVStep)
{
    auto oldValue = iface->tidalVStep();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::tidalVStepChanged, [&]() { changeSignalEmitted = true; });

    iface->setTidalVStep(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->tidalVStep(), oldValue);
    ASSERT_EQ(iface->tidalVStep(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, peepStep)
{
    auto oldValue = iface->peepStep();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::peepStepChanged, [&]() { changeSignalEmitted = true; });

    iface->setPeepStep(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->peepStep(), oldValue);
    ASSERT_EQ(iface->peepStep(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, rrMinLow)
{
    auto oldValue = iface->rrMinLow();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinLowChanged, [&]() { changeSignalEmitted = true; });

    iface->setRrMinLow(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMinLow(), oldValue);
    ASSERT_EQ(iface->rrMinLow(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, rrMinHigh)
{
    auto oldValue = iface->rrMinHigh();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinHighChanged, [&]() { changeSignalEmitted = true; });

    iface->setRrMinHigh(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMinHigh(), oldValue);
    ASSERT_EQ(iface->rrMinHigh(), newValue);
}

TEST_F(VentilatorParametersInterfaceInterfaceTest, rrMinStep)
{
    auto oldValue = iface->rrMinStep();
    auto newValue = std::numeric_limits<int>::max();

    bool changeSignalEmitted = false;
    QObject::connect(iface, &VentilatorUI::VentilatorParametersInterface::rrMinStepChanged, [&]() { changeSignalEmitted = true; });

    iface->setRrMinStep(newValue);
    ASSERT_TRUE(changeSignalEmitted);
    ASSERT_NE(iface->rrMinStep(), oldValue);
    ASSERT_EQ(iface->rrMinStep(), newValue);
}
