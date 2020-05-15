#include "ventilatorparametersinterface.hpp"
#include <QDebug>

namespace VentilatorUI {
VentilatorParametersInterface::VentilatorParametersInterface(QObject *parent)
  : QObject(parent)
{
}

void VentilatorParametersInterface::startPreUseTest()
{
}

void VentilatorParametersInterface::setPressureMode(int mode)
{
}

void VentilatorParametersInterface::adjustTidalV(int value)
{
}

void VentilatorParametersInterface::adjustPeep(int value)
{
}

void VentilatorParametersInterface::adjustRrMin(int value)
{
}

void VentilatorParametersInterface::setTidalV(int tidalV)
{
    if (GreenHouse::different(m_tidalV, tidalV)) {
        GreenHouse::cleanupProperty(m_tidalV, this);
        m_tidalV = tidalV;
        emit tidalVChanged();
    }
}

void VentilatorParametersInterface::setTidalVpopup(int tidalVpopup)
{
    if (GreenHouse::different(m_tidalVpopup, tidalVpopup)) {
        GreenHouse::cleanupProperty(m_tidalVpopup, this);
        m_tidalVpopup = tidalVpopup;
        emit tidalVpopupChanged();
    }
}

void VentilatorParametersInterface::setPeep(int peep)
{
    if (GreenHouse::different(m_peep, peep)) {
        GreenHouse::cleanupProperty(m_peep, this);
        m_peep = peep;
        emit peepChanged();
    }
}

void VentilatorParametersInterface::setPeepPopup(int peepPopup)
{
    if (GreenHouse::different(m_peepPopup, peepPopup)) {
        GreenHouse::cleanupProperty(m_peepPopup, this);
        m_peepPopup = peepPopup;
        emit peepPopupChanged();
    }
}

void VentilatorParametersInterface::setRrMin(int rrMin)
{
    if (GreenHouse::different(m_rrMin, rrMin)) {
        GreenHouse::cleanupProperty(m_rrMin, this);
        m_rrMin = rrMin;
        emit rrMinChanged();
    }
}

void VentilatorParametersInterface::setRrMinPopup(int rrMinPopup)
{
    if (GreenHouse::different(m_rrMinPopup, rrMinPopup)) {
        GreenHouse::cleanupProperty(m_rrMinPopup, this);
        m_rrMinPopup = rrMinPopup;
        emit rrMinPopupChanged();
    }
}

void VentilatorParametersInterface::setAlertFlag(bool alertFlag)
{
    if (GreenHouse::different(m_alertFlag, alertFlag)) {
        GreenHouse::cleanupProperty(m_alertFlag, this);
        m_alertFlag = alertFlag;
        emit alertFlagChanged();
    }
}

void VentilatorParametersInterface::setPreUseTestPassed(bool preUseTestPassed)
{
    if (GreenHouse::different(m_preUseTestPassed, preUseTestPassed)) {
        GreenHouse::cleanupProperty(m_preUseTestPassed, this);
        m_preUseTestPassed = preUseTestPassed;
        emit preUseTestPassedChanged();
    }
}

void VentilatorParametersInterface::setShowSettings(bool showSettings)
{
    if (GreenHouse::different(m_showSettings, showSettings)) {
        GreenHouse::cleanupProperty(m_showSettings, this);
        m_showSettings = showSettings;
        emit showSettingsChanged();
    }
}

void VentilatorParametersInterface::setAcpMode(int acpMode)
{
    if (GreenHouse::different(m_acpMode, acpMode)) {
        GreenHouse::cleanupProperty(m_acpMode, this);
        m_acpMode = acpMode;
        emit acpModeChanged();
    }
}

void VentilatorParametersInterface::setAcpModeName(const QString &acpModeName)
{
    if (GreenHouse::different(m_acpModeName, acpModeName)) {
        GreenHouse::cleanupProperty(m_acpModeName, this);
        m_acpModeName = acpModeName;
        emit acpModeNameChanged();
    }
}

void VentilatorParametersInterface::setCurrentRate(int currentRate)
{
    if (GreenHouse::different(m_currentRate, currentRate)) {
        GreenHouse::cleanupProperty(m_currentRate, this);
        m_currentRate = currentRate;
        emit currentRateChanged();
    }
}

void VentilatorParametersInterface::setCurrentPeep(int currentPeep)
{
    if (GreenHouse::different(m_currentPeep, currentPeep)) {
        GreenHouse::cleanupProperty(m_currentPeep, this);
        m_currentPeep = currentPeep;
        emit currentPeepChanged();
    }
}

void VentilatorParametersInterface::setCurrentTidalV(int currentTidalV)
{
    if (GreenHouse::different(m_currentTidalV, currentTidalV)) {
        GreenHouse::cleanupProperty(m_currentTidalV, this);
        m_currentTidalV = currentTidalV;
        emit currentTidalVChanged();
    }
}

void VentilatorParametersInterface::setCurrentAdditionalValue(int currentAdditionalValue)
{
    if (GreenHouse::different(m_currentAdditionalValue, currentAdditionalValue)) {
        GreenHouse::cleanupProperty(m_currentAdditionalValue, this);
        m_currentAdditionalValue = currentAdditionalValue;
        emit currentAdditionalValueChanged();
    }
}

void VentilatorParametersInterface::setPowerSourceType(int powerSourceType)
{
    if (GreenHouse::different(m_powerSourceType, powerSourceType)) {
        GreenHouse::cleanupProperty(m_powerSourceType, this);
        m_powerSourceType = powerSourceType;
        emit powerSourceTypeChanged();
    }
}

void VentilatorParametersInterface::setPowerSourceAsset(const QString &powerSourceAsset)
{
    if (GreenHouse::different(m_powerSourceAsset, powerSourceAsset)) {
        GreenHouse::cleanupProperty(m_powerSourceAsset, this);
        m_powerSourceAsset = powerSourceAsset;
        emit powerSourceAssetChanged();
    }
}

void VentilatorParametersInterface::setBatteryPercentageText(const QString &batteryPercentageText)
{
    if (GreenHouse::different(m_batteryPercentageText, batteryPercentageText)) {
        GreenHouse::cleanupProperty(m_batteryPercentageText, this);
        m_batteryPercentageText = batteryPercentageText;
        emit batteryPercentageTextChanged();
    }
}

void VentilatorParametersInterface::setBatteryPercentageAsset(const QString &batteryPercentageAsset)
{
    if (GreenHouse::different(m_batteryPercentageAsset, batteryPercentageAsset)) {
        GreenHouse::cleanupProperty(m_batteryPercentageAsset, this);
        m_batteryPercentageAsset = batteryPercentageAsset;
        emit batteryPercentageAssetChanged();
    }
}

void VentilatorParametersInterface::setBatteryLevel(int batteryLevel)
{
    if (GreenHouse::different(m_batteryLevel, batteryLevel)) {
        GreenHouse::cleanupProperty(m_batteryLevel, this);
        m_batteryLevel = batteryLevel;
        emit batteryLevelChanged();
    }
}

void VentilatorParametersInterface::setAlertText(const QString &alertText)
{
    if (GreenHouse::different(m_alertText, alertText)) {
        GreenHouse::cleanupProperty(m_alertText, this);
        m_alertText = alertText;
        emit alertTextChanged();
    }
}

void VentilatorParametersInterface::setTidalVLow(int tidalVLow)
{
    if (GreenHouse::different(m_tidalVLow, tidalVLow)) {
        GreenHouse::cleanupProperty(m_tidalVLow, this);
        m_tidalVLow = tidalVLow;
        emit tidalVLowChanged();
    }
}

void VentilatorParametersInterface::setTidalVHigh(int tidalVHigh)
{
    if (GreenHouse::different(m_tidalVHigh, tidalVHigh)) {
        GreenHouse::cleanupProperty(m_tidalVHigh, this);
        m_tidalVHigh = tidalVHigh;
        emit tidalVHighChanged();
    }
}

void VentilatorParametersInterface::setPeepLow(int peepLow)
{
    if (GreenHouse::different(m_peepLow, peepLow)) {
        GreenHouse::cleanupProperty(m_peepLow, this);
        m_peepLow = peepLow;
        emit peepLowChanged();
    }
}

void VentilatorParametersInterface::setPeepHigh(int peepHigh)
{
    if (GreenHouse::different(m_peepHigh, peepHigh)) {
        GreenHouse::cleanupProperty(m_peepHigh, this);
        m_peepHigh = peepHigh;
        emit peepHighChanged();
    }
}

void VentilatorParametersInterface::setTidalVStep(int tidalVStep)
{
    if (GreenHouse::different(m_tidalVStep, tidalVStep)) {
        GreenHouse::cleanupProperty(m_tidalVStep, this);
        m_tidalVStep = tidalVStep;
        emit tidalVStepChanged();
    }
}

void VentilatorParametersInterface::setPeepStep(int peepStep)
{
    if (GreenHouse::different(m_peepStep, peepStep)) {
        GreenHouse::cleanupProperty(m_peepStep, this);
        m_peepStep = peepStep;
        emit peepStepChanged();
    }
}

void VentilatorParametersInterface::setRrMinLow(int rrMinLow)
{
    if (GreenHouse::different(m_rrMinLow, rrMinLow)) {
        GreenHouse::cleanupProperty(m_rrMinLow, this);
        m_rrMinLow = rrMinLow;
        emit rrMinLowChanged();
    }
}

void VentilatorParametersInterface::setRrMinHigh(int rrMinHigh)
{
    if (GreenHouse::different(m_rrMinHigh, rrMinHigh)) {
        GreenHouse::cleanupProperty(m_rrMinHigh, this);
        m_rrMinHigh = rrMinHigh;
        emit rrMinHighChanged();
    }
}

void VentilatorParametersInterface::setRrMinStep(int rrMinStep)
{
    if (GreenHouse::different(m_rrMinStep, rrMinStep)) {
        GreenHouse::cleanupProperty(m_rrMinStep, this);
        m_rrMinStep = rrMinStep;
        emit rrMinStepChanged();
    }
}
}
