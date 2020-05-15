/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef DATASOCKET_H
#define DATASOCKET_H

#include <QObject>

#include "waveformsample.h"

class QUdpSocket;

class DataSocket : public QObject
{
    Q_OBJECT
public:
    explicit DataSocket(const QString &dataPath);
    ~DataSocket();

signals:
    void dataReady(const QMap<int, QVector<WaveformSample>> &data);

private:
    QUdpSocket *m_socket;
};

#endif // DATASOCKET_H
