#ifndef IpcSocket_H
#define IpcSocket_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class IpcSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit IpcSocket(QObject *parent = nullptr);
    void connectToSimulator();

    void sendStartPreUseTest()          { sendSimpleParameter(QJsonValue(QStringLiteral("StartPreUseTest")), QJsonValue(QString())); };
    void sendSetPressureMode(int mode)  { sendSimpleParameter(QJsonValue(QStringLiteral("SetPressureMode")), QJsonValue(mode)); };
    void sendSetTidalv(int value)       { sendSimpleParameter(QJsonValue(QStringLiteral("SetTidalv")), QJsonValue(value)); };
    void sendSetPeep(int value)         { sendSimpleParameter(QJsonValue(QStringLiteral("SetPeep")), QJsonValue(value)); };
    void sendSetRr(int value)           { sendSimpleParameter(QJsonValue(QStringLiteral("SetRr")), QJsonValue(value)); };

private:
    void sendSimpleParameter(const QJsonValue& eventName, const QJsonValue& result);
    void parseAndEmit(const QJsonDocument&);

signals:
    void message(const QString&);

    void receivedPreUseTestFinished(bool result);
    void receivedPressureModeChanged(int mode);
    void receivedPowerSourceChanged(int type);
    void receivedBatteryLevel(int level);
    void receivedCurrentRate(int value);
    void receivedCurrentPEEP(int value);
    void receivedCurrentTidalVolume(int value);
    void receivedCurrentAdditionalValue(int value);
    void receivedWaveFormFlow(int value);
    void receivedWaveFormVolume(int value);
    void receivedWaveFormPressure(int value);
    void receivedAlert(const QString &alert, const QDateTime &timestamp, int priority);
    void receivedTidalvMin(int value);
    void receivedTidalvMax(int value);
    void receivedTidalv(int value);
    void receivedPeepMin(int value);
    void receivedPeepMax(int value);
    void receivedPeep(int value);
    void receivedRrMin(int value);
    void receivedRrMax(int value);
    void receivedRr(int value);
};

#endif // IpcSocket_H
