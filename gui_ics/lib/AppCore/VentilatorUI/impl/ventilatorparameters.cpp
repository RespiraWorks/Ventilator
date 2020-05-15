#include "ventilatorparameters.hpp"

#include <QDateTime>

namespace VentilatorUI {
VentilatorParameters::VentilatorParameters(QObject *parent)
  : VentilatorParametersInterface(parent)
  , m_ipcSocket(new IpcSocket(this))
{
    // TODO Init CTOR with connections to real backend instead of IpcSocket
    connect(m_ipcSocket, &IpcSocket::receivedPreUseTestFinished, this,
            &VentilatorParameters::setPreUseTestPassed);
    connect(m_ipcSocket, &IpcSocket::receivedPressureModeChanged, this,
            &VentilatorParameters::setAcpMode);
    connect(m_ipcSocket, &IpcSocket::receivedPowerSourceChanged, this,
            &VentilatorParameters::setPowerSourceType);
    connect(m_ipcSocket, &IpcSocket::receivedBatteryLevel, this,
            &VentilatorParameters::setBatteryLevel);
    connect(m_ipcSocket, &IpcSocket::receivedAlert, this, &VentilatorParameters::handleAlert);

    connect(m_ipcSocket, &IpcSocket::receivedTidalv, this, &VentilatorParameters::setTidalV);
    connect(m_ipcSocket, &IpcSocket::receivedTidalvMin, this, &VentilatorParameters::setTidalVLow);
    connect(m_ipcSocket, &IpcSocket::receivedTidalvMax, this, &VentilatorParameters::setTidalVHigh);

    connect(m_ipcSocket, &IpcSocket::receivedPeep, this, &VentilatorParameters::setPeep);
    connect(m_ipcSocket, &IpcSocket::receivedPeepMin, this, &VentilatorParameters::setPeepLow);
    connect(m_ipcSocket, &IpcSocket::receivedPeepMax, this, &VentilatorParameters::setPeepHigh);

    connect(m_ipcSocket, &IpcSocket::receivedRr, this, &VentilatorParameters::setRrMin);
    connect(m_ipcSocket, &IpcSocket::receivedRrMin, this, &VentilatorParameters::setRrMinLow);
    connect(m_ipcSocket, &IpcSocket::receivedRrMax, this, &VentilatorParameters::setRrMinHigh);

    connect(m_ipcSocket, &IpcSocket::receivedCurrentRate, this,
            &VentilatorParameters::setCurrentRate);
    connect(m_ipcSocket, &IpcSocket::receivedCurrentPEEP, this,
            &VentilatorParameters::setCurrentPeep);
    connect(m_ipcSocket, &IpcSocket::receivedCurrentTidalVolume, this,
            &VentilatorParameters::setCurrentTidalV);
    connect(m_ipcSocket, &IpcSocket::receivedCurrentAdditionalValue, this,
            &VentilatorParameters::setCurrentAdditionalValue);

    setAcpMode(acpMode());
    setPowerSourceType(powerSourceType());
    setBatteryLevel(batteryLevel());
    setTidalVLow(tidalVLow());
    setTidalVHigh(tidalVHigh());
    setPeepLow(peepLow());
    setPeepHigh(peepHigh());
    setRrMinLow(rrMinLow());
    setRrMinHigh(rrMinHigh());

    m_ipcSocket->connectToSimulator();
}

void VentilatorParameters::startPreUseTest()
{
    VentilatorParametersInterface::startPreUseTest();

    if (!checkConnection()) {
        return;
    }

    m_ipcSocket->sendStartPreUseTest();

    // TODO remove stub
    setPreUseTestPassed(true);
}

void VentilatorParameters::setPressureMode(int mode)
{
    VentilatorParametersInterface::setPressureMode(mode);

    if (!checkConnection()) {
        return;
    }

    m_ipcSocket->sendSetPressureMode(mode);

    // TODO remove stub
    setAcpMode(mode);
}

void VentilatorParameters::setAcpMode(int acpMode)
{
    VentilatorParametersInterface::setAcpMode(acpMode);

    static const QVector<QString> modeNames = { QStringLiteral("AC/P"), QStringLiteral("PCV"),
                                                QStringLiteral("CPP") };

    setAcpModeName(modeNames.value(acpMode));
}

void VentilatorParameters::setPowerSourceType(int powerSourceType)
{
    VentilatorParametersInterface::setPowerSourceType(powerSourceType);

    static const QVector<QString> powerSourceAssets = {
        QStringLiteral("/Icons/Power__ACState.png"), QStringLiteral("/Icons/Power__DCState.png")
    };

    setPowerSourceAsset(powerSourceAssets.value(powerSourceType));
}

void VentilatorParameters::setBatteryLevel(int batteryLevel)
{
    VentilatorParametersInterface::setBatteryLevel(batteryLevel);

    setBatteryPercentageText(QStringLiteral("%0%").arg(batteryLevel));
}

void VentilatorParameters::adjustTidalV(int value)
{
    VentilatorParametersInterface::adjustTidalV(value);

    if (!checkConnection()) {
        return;
    }

    m_ipcSocket->sendSetTidalv(value);

    // TODO remove stub
    setTidalV(value);
}

void VentilatorParameters::setTidalV(int tidalV)
{
    const int value = qBound(tidalVLow(), tidalV, tidalVHigh());

    VentilatorParametersInterface::setTidalV(value);
}

void VentilatorParameters::setTidalVpopup(int tidalVpopup)
{
    const int value = qBound(tidalVLow(), tidalVpopup, tidalVHigh());

    VentilatorParametersInterface::setTidalVpopup(value);
}

void VentilatorParameters::setTidalVLow(int tidalVLow)
{
    VentilatorParametersInterface::setTidalVLow(tidalVLow);
    setTidalV(tidalV());
}

void VentilatorParameters::setTidalVHigh(int tidalVHigh)
{
    VentilatorParametersInterface::setTidalVHigh(tidalVHigh);
    setTidalV(tidalV());
}

void VentilatorParameters::adjustPeep(int value)
{
    VentilatorParametersInterface::adjustPeep(value);

    if (!checkConnection()) {
        return;
    }

    m_ipcSocket->sendSetPeep(value);

    // TODO remove stub
    setPeep(value);
}

void VentilatorParameters::setPeep(int peep)
{
    const int value = qBound(peepLow(), peep, peepHigh());

    VentilatorParametersInterface::setPeep(value);
}

void VentilatorParameters::setPeepPopup(int peepPopup)
{
    const int value = qBound(peepLow(), peepPopup, peepHigh());

    VentilatorParametersInterface::setPeepPopup(value);
}

void VentilatorParameters::setPeepLow(int peepLow)
{
    VentilatorParametersInterface::setPeepLow(peepLow);
    setPeep(peep());
}

void VentilatorParameters::setPeepHigh(int peepHigh)
{
    VentilatorParametersInterface::setPeepHigh(peepHigh);
    setPeep(peep());
}

void VentilatorParameters::adjustRrMin(int value)
{
    VentilatorParametersInterface::adjustRrMin(value);

    if (!checkConnection()) {
        return;
    }

    m_ipcSocket->sendSetRr(value);

    // TODO remove stub
    setRrMin(value);
}

void VentilatorParameters::setRrMin(int rrMin)
{
    const int value = qBound(rrMinLow(), rrMin, rrMinHigh());

    VentilatorParametersInterface::setRrMin(value);
}

void VentilatorParameters::setRrMinPopup(int rrMinPopup)
{
    const int value = qBound(rrMinLow(), rrMinPopup, rrMinHigh());

    VentilatorParametersInterface::setRrMinPopup(value);
}

void VentilatorParameters::setRrMinLow(int rrMinLow)
{
    VentilatorParametersInterface::setRrMinLow(rrMinLow);
    setRrMin(rrMin());
}

void VentilatorParameters::setRrMinHigh(int rrMinHigh)
{
    VentilatorParametersInterface::setRrMinHigh(rrMinHigh);
    setRrMin(rrMin());
}

void VentilatorParameters::handleAlert(const QString &alert, const QDateTime &timestamp,
                                       int priority)
{
    setAlertText(alert);
    setAlertFlag(true);
}

bool VentilatorParameters::checkConnection() const
{
    if (m_ipcSocket->state()
        != QAbstractSocket::ConnectedState) { // TODO replace with == for Simulator
        return true;
    }

    if (m_ipcSocket->state() == QAbstractSocket::UnconnectedState) {
        m_ipcSocket->connectToSimulator();
    } else {
        m_ipcSocket->disconnectFromHost();
    }

    return false;
}

}
