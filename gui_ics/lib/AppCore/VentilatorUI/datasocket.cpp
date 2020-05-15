/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "datasocket.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
#include <QNetworkDatagram>
#endif

#include <QThread>
#include <QUdpSocket>

#include "global_constants.h"
#include "medicalplottingcomponentplugin_log.h"

DataSocket::DataSocket(const QString &dataPath)
  : QObject(nullptr)
  , m_socket(new QUdpSocket(this))
{
    qRegisterMetaType<QMap<int, QVector<WaveformSample>>>("QMap<int, QVector<WaveformSample>>");
    QThread *socketThread = new QThread;
    this->moveToThread(socketThread);
    connect(socketThread, &QThread::started, this, [this, dataPath]() -> void {
        const QStringList addressList = dataPath.split(QLatin1Char(':'));
        if (addressList.length() < 2) {
            mpcpWarning() << "Invalid data path:" << dataPath;
        } else {
            bool ok = false;
            quint16 port = quint16(addressList.value(addressList.length() - 1).toInt(&ok));
            if (!ok) {
                mpcpWarning() << "Invalid port value:"
                              << addressList.value(addressList.length() - 1);
            } else {
                m_socket->bind(QHostAddress(addressList.at(addressList.length() - 2)), port);
            }
        }
    });
    connect(m_socket, &QUdpSocket::readyRead, this, [this]() -> void {
        QMap<int, QVector<WaveformSample>> data;
        while (m_socket->hasPendingDatagrams()) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
            const QNetworkDatagram datagram = m_socket->receiveDatagram();
            QTextStream stream(datagram.data());
#else
                    QByteArray datagram;
                    datagram.resize(m_socket->pendingDatagramSize());
                    m_socket->readDatagram(datagram.data(), datagram.size());
                    QTextStream stream(datagram);
#endif
            while (!stream.atEnd()) {
                const QString line = stream.readLine();
                const QStringList tokens = line.split(QStringLiteral(","));
                // NOTE: We expect the first token to be a timestamp which we ignore for now
                for (int i = 1; i < tokens.length(); ++i) {
                    bool ok = false;
                    const double value = tokens.at(i).toDouble(&ok) * DATA_RANGE;
                    if (!ok)
                        continue;
                    auto it = data.find(i - 1);
                    if (it == data.end())
                        it = data.insert(i - 1, QVector<WaveformSample>());
                    it.value().append(WaveformSample { qRound(value), 0 });
                }
            }
        }
        emit dataReady(data);
    });
    socketThread->start();
}

DataSocket::~DataSocket()
{
    thread()->exit(0);
    thread()->wait();
    delete thread();
}
