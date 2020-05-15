/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "datareader.h"

#include <cmath>

#include <QCoreApplication>
#include <QFile>
#include <QElapsedTimer>
#include <QMutexLocker>
#include <QThread>

#include "datamodelwaveform.h"
#include "datasocket.h"
#include "medicalplottingcomponentplugin_log.h"
#include "sweeptimer.h"

DataReader::DataReader(QObject *parent)
  : QObject(parent)
  , m_dataTimer(new QTimer(this))
  , m_ecg1(nSamples)
  , m_ecg2(nSamples)
  , m_resp(nSamples)
  , m_scg(nSamples)
  , m_timestamps(nSamples)
  , m_dataPath()
{
    m_dataTimer->setTimerType(Qt::PreciseTimer);
    connect(m_dataTimer, &QTimer::timeout, this, &DataReader::generateData);

    QThread *timer_thread = new QThread();
    m_sweepTimer = new SweepTimer();
    connect(timer_thread, &QThread::started, m_sweepTimer, &SweepTimer::start);
    m_sweepTimer->moveToThread(timer_thread);

    timer_thread->start();
}

DataReader::~DataReader()
{
    if (m_socket)
        delete m_socket;

    m_sweepTimer->thread()->exit(0);
    m_sweepTimer->thread()->wait();
    delete m_sweepTimer->thread();
    delete m_sweepTimer;
}

static QMap<QString, DataReader *> s_dataReaderStorage;
DataReader *DataReader::instance(const QString &path)
{
    auto it = s_dataReaderStorage.find(path);
    if (it == s_dataReaderStorage.end()) {
        it = s_dataReaderStorage.insert(path, new DataReader(qApp));
        it.value()->setDataPath(path);
    }
    return it.value();
}

void DataReader::registerAgentConnector(WaveformAgentConnector *connector, int waveformType)
{
    if (connector) {
        QMutexLocker lock(&m_connectorsMutex);
        Q_UNUSED(lock)
        for (auto it = m_agentConnectors.begin(); it != m_agentConnectors.end(); ++it)
            it.value().removeAll(connector);
        auto it = m_agentConnectors.find(waveformType);
        if (it == m_agentConnectors.end())
            it = m_agentConnectors.insert(waveformType, QList<WaveformAgentConnector *>());
        it.value().append(connector);

        connect(m_sweepTimer, &SweepTimer::renderSweep, connector,
                &WaveformAgentConnector::advanceSweep);
    }
}

void DataReader::unregisterAgentConnector(WaveformAgentConnector *connector)
{
    bool empty = true;
    disconnect(m_sweepTimer, &SweepTimer::renderSweep, connector,
               &WaveformAgentConnector::advanceSweep);
    QMutexLocker lock(&m_connectorsMutex);
    Q_UNUSED(lock)
    for (auto it = m_agentConnectors.begin(); it != m_agentConnectors.end(); ++it) {
        it.value().removeAll(connector);
        if (empty && it.value().length() > 0)
            empty = false;
    }
    if (empty) {
        s_dataReaderStorage.remove(m_dataPath);
        deleteLater();
    }
}

void DataReader::setDataPath(const QString &path)
{
    if (m_dataPath != path) {
        m_dataPath = path;
        emit dataPathChanged();
        if (m_dataPath.startsWith(QStringLiteral("udp:"))) {
            if (m_dataTimer->isActive())
                m_dataTimer->stop();
            if (m_socket)
                m_socket->deleteLater();
            m_socket = new DataSocket(m_dataPath);
            connect(m_socket, &DataSocket::dataReady, this, &DataReader::injectData);
        } else {
            if (!m_dataTimer->isActive())
                m_dataTimer->start(16);
            readData();
        }
    }
}

WaveformSample DataReader::getSample(int waveForm, int sample)
{
    QMutexLocker lock(&m_dataMutex);
    Q_UNUSED(lock)

    switch (waveForm) {
    case 1:
        return WaveformSample(m_ecg2.at(sample), static_cast<unsigned int>(0));
    case 2:
        return WaveformSample(m_resp.at(sample), static_cast<unsigned int>(0));
    case 3:
        return WaveformSample(m_scg.at(sample), static_cast<unsigned int>(0));
    }
    return WaveformSample(m_ecg1.at(sample), static_cast<unsigned int>(0));
}

void DataReader::generateData()
{
    int num_samples = UPDATE_RATE * DATA_RATE / 1000;
    int sample = 0;
    QMutexLocker lock(&m_connectorsMutex);
    Q_UNUSED(lock)
    for (auto it = m_agentConnectors.begin(); it != m_agentConnectors.end(); ++it) {
        QVector<WaveformSample> vals;
        sample = m_curSampleIndex;
        for (int i = 0; i < num_samples; ++i) {
            vals.append(getSample(it.key(), sample));
            ++sample;
            if (sample > nSamples - 1)
                sample = 0;
        }
        for (WaveformAgentConnector *connector : it.value())
            connector->processWaveFormData(vals);
    }
    m_curSampleIndex = sample;
}

void DataReader::injectData(const QMap<int, QVector<WaveformSample>> &data)
{
    for (auto it = m_agentConnectors.begin(); it != m_agentConnectors.end(); ++it) {
        auto dataIt = data.find(it.key());
        if (dataIt != data.constEnd()) {
            for (WaveformAgentConnector *connector : it.value())
                connector->processWaveFormData(dataIt.value());
        }
    }
}

void DataReader::readData()
{
    QElapsedTimer stopWatch;
    stopWatch.start();

    QMutexLocker lock(&m_dataMutex);
    Q_UNUSED(lock)
    QFile file(m_dataPath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QTextStream ts(&file);
        int lc = 0;
        while (!ts.atEnd() && lc < nSamples) {
            const QString line = ts.readLine();
            if (line.startsWith(QStringLiteral("#")))
                continue; // TODO this is the header, grab info from here

            const QStringList tokens = line.split(QStringLiteral(","));
            int tc = 0;
            for (const QString &t : tokens) {
                // there are five tokens - the first one is the time stamp
                if (tc == 0) {
                    const QString v = t.mid(1, t.length() - 2);
                    QTime tm = QTime::fromString(v, QStringLiteral("m:ss.zzz"));
                    Q_ASSERT(tm.isValid());
                    m_timestamps[lc] = tm;
                } else {
                    bool ok = false;
                    const double v = t.toDouble(&ok) * DATA_RANGE;
                    Q_ASSERT(ok);
                    if (tc == 1) {
                        m_ecg1[lc] = static_cast<int>(std::round(v)); // ecg 1
                    } else if (tc == 2) {
                        m_ecg2[lc] = static_cast<int>(std::round(v)); // ecg 2
                    } else if (tc == 3) {
                        m_resp[lc] = static_cast<int>(std::round(v)); // resp
                    } else if (tc == 4) {
                        m_scg[lc] = static_cast<int>(std::round(v)); // scg
                    }
                }
                ++tc;
            }
            ++lc;
        }
        emit newDataAvailable();
    } else {
        mpcpWarning() << "Could not read data from:" << m_dataPath;
    }
}
