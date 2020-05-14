#include "ipcsocket.h"

IpcSocket::IpcSocket(QObject *parent)
  : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::connected,
            [=]() { emit message(QStringLiteral("Socket connected")); });
    connect(this, &QTcpSocket::disconnected,
            [=]() { emit message(QStringLiteral("Socket disconnected")); });
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [=]() { emit message(QStringLiteral("Socket Error: ") + errorString()); });

    connect(this, &QTcpSocket::readyRead, [=]() {
        // It is better if there is a protocol in case of data loss. But now it is a localhost.
        // May be better to do socket into thread. (or moveToThread object of this class from code)
        // Plus better if parse will be through QThreads.

        QByteArray incomingData = readAll();
        parseAndEmit(QJsonDocument::fromBinaryData(incomingData));
    });
}

void IpcSocket::sendSimpleParameter(const QJsonValue &eventName, const QJsonValue &result)
{
    if (state() != QAbstractSocket::ConnectedState) {
        emit message(QStringLiteral("Socket is not connected"));
        return;
    }
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert(QStringLiteral("EventName"), eventName);
    obj.insert(QStringLiteral("Result"), result);
    doc.setObject(obj);
    write(doc.toBinaryData());
}

void IpcSocket::connectToSimulator()
{
    if (state() == QAbstractSocket::ConnectedState)
        return;

    connectToHost(QHostAddress::LocalHost, 12345);
}

void IpcSocket::parseAndEmit(const QJsonDocument &doc)
{
    if (doc.isNull()) {
        emit message(QStringLiteral("Document is Null"));
        return;
    }
    QJsonObject obj = doc.object();
    QString event = obj.value(QStringLiteral("EventName")).toString();
    if (event == QStringLiteral("PreUseTestFinished")) {
        emit receivedPreUseTestFinished(obj.value(QStringLiteral("Result")).toBool());
    } else if (event == QStringLiteral("PressureModeChanged")) {
        emit receivedPressureModeChanged(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("PowerSourceChanged")) {
        emit receivedPowerSourceChanged(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("BatteryLevel")) {
        emit receivedBatteryLevel(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("CurrentRate")) {
        emit receivedCurrentRate(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("CurrentPEEP")) {
        emit receivedCurrentPEEP(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("CurrentTidalVolume")) {
        emit receivedCurrentTidalVolume(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("CurrentAdditionalValue")) {
        emit receivedCurrentAdditionalValue(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("WaveFormFlow")) {
        emit receivedWaveFormFlow(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("WaveFormVolume")) {
        emit receivedWaveFormVolume(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("WaveFormPressure")) {
        emit receivedWaveFormPressure(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("TidalvMin")) {
        emit receivedTidalvMin(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("TidalvMax")) {
        emit receivedTidalvMax(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("Tidalv")) {
        emit receivedTidalv(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("PeepMin")) {
        emit receivedPeepMin(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("PeepMax")) {
        emit receivedPeepMax(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("Peep")) {
        emit receivedPeep(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("RrMin")) {
        emit receivedRrMin(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("RrMax")) {
        emit receivedRrMax(obj.value(QStringLiteral("Result")).toInt());
    } else if (event == QStringLiteral("Rr")) {
        emit receivedRr(obj.value(QStringLiteral("Result")).toInt());
    } else {
        emit message(QStringLiteral("Wrong EventName..."));
    }
}
