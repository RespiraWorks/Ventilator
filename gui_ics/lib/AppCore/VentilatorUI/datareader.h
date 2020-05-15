/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#pragma once

#include <QObject>

#include <QMap>
#include <QMutex>
#include <QTime>
#include <QVector>

#include "global_constants.h"
#include "waveformsample.h"

class QTimer;

class DataSocket;
class SweepTimer;
class WaveformAgentConnector;

class DataReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dataPath READ dataPath WRITE setDataPath NOTIFY dataPathChanged)
public:
    DataReader(QObject *parent = nullptr);
    ~DataReader();

    static DataReader *instance(const QString &path);

    void registerAgentConnector(WaveformAgentConnector *connector, int waveformType);
    void unregisterAgentConnector(WaveformAgentConnector *connector);

    QString dataPath() const { return m_dataPath; }

public slots:
    void setDataPath(const QString &path);

protected slots:
    void generateData();
    void injectData(const QMap<int, QVector<WaveformSample>> &data);

signals:
    void dataPathChanged();
    void newDataAvailable();

private:
    void readData();
    WaveformSample getSample(int waveForm, int sample);

    int m_curSampleIndex = 0;
    const int nSamples = 3 * DATA_RATE; // 60 secs data

    QTimer *m_dataTimer = nullptr;

    // storage for 60 secs of data
    QMutex m_dataMutex;
    QVector<int> m_ecg1, m_ecg2, m_resp, m_scg;
    QVector<QTime> m_timestamps;

    QString m_dataPath;

    QMutex m_connectorsMutex;
    QMap<int, QList<WaveformAgentConnector *>> m_agentConnectors;

    DataSocket *m_socket = nullptr;
    SweepTimer *m_sweepTimer = nullptr;
};
