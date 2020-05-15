#pragma once

#include <VentilatorUI/ventilatorparametersinterface.hpp>

#include "ipcsocket.h"

namespace VentilatorUI {
class APPCORE_EXPORT VentilatorParameters : public VentilatorParametersInterface
{
    Q_OBJECT
    Q_DISABLE_COPY(VentilatorParameters)
    Q_DISABLE_MOVE(VentilatorParameters)
public:
    enum class PressureModes { ACPMode = 0, PCVMode, CPPMode };
    Q_ENUM(PressureModes)

    explicit VentilatorParameters(QObject *parent = nullptr);
    ~VentilatorParameters() override = default;

public slots:
    void startPreUseTest() override;

    void setPressureMode(int mode) override;
    void setAcpMode(int acpMode) override;
    void setPowerSourceType(int powerSourceType) override;
    void setBatteryLevel(int batteryLevel) override;

    void adjustTidalV(int value) override;
    void setTidalV(int tidalV) override;
    void setTidalVpopup(int tidalVpopup) override;
    void setTidalVLow(int tidalVLow) override;
    void setTidalVHigh(int tidalVHigh) override;

    void adjustPeep(int value) override;
    void setPeep(int peep) override;
    void setPeepPopup(int peepPopup) override;
    void setPeepLow(int peepLow) override;
    void setPeepHigh(int peepHigh) override;

    void adjustRrMin(int value) override;
    void setRrMin(int rrMin) override;
    void setRrMinPopup(int rrMinPopup) override;
    void setRrMinLow(int rrMinLow) override;
    void setRrMinHigh(int rrMinHigh) override;

private slots:
    void handleAlert(const QString &alert, const QDateTime &timestamp, int priority);

private:
    bool checkConnection() const; // TODO remove for real backend

private:
    IpcSocket *m_ipcSocket; // TODO remove for real backend
};
}
