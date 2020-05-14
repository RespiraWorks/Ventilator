/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef DATAMODELWAVEFORM_H
#define DATAMODELWAVEFORM_H

#include <QObject>

#include <QQmlListProperty>

#include "waveformsample.h"

class DataModelWaveform;
class FilteredWaveformValueData;

class WaveformAgentConnector : public QObject
{
    Q_OBJECT

public:
    explicit WaveformAgentConnector(DataModelWaveform *model, QObject *parent = nullptr);

public slots:
    void updateWaveformValueData(const QVector<WaveformSample> &data);
    void advanceSweep();

signals:
    void processWaveFormData(const QVector<WaveformSample> &data);

private:
    DataModelWaveform *m_model;
};

class DataModelWaveform : public QObject
{
    Q_OBJECT
public:
    explicit DataModelWaveform(QObject *parent = nullptr);
    ~DataModelWaveform();

    FilteredWaveformValueData *filteredWaveform(int index) const;
    WaveformAgentConnector *connector() const { return m_connector; }

private:
    QList<unsigned int> m_waveform_ids;
    QList<FilteredWaveformValueData *> m_filtered_data;

    WaveformAgentConnector *m_connector;
    QThread *m_connector_thread;

    friend class WaveformAgentConnector;
};

#endif // DATAMODELWAVEFORM_H
