/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "datamodelwaveform.h"

#include <QThread>

#include "filteredwaveformvaluedata.h"
#include "global_constants.h"

WaveformAgentConnector::WaveformAgentConnector(DataModelWaveform *model, QObject *parent)
  : QObject(parent)
  , m_model(model)
{
    connect(this, &WaveformAgentConnector::processWaveFormData, this,
            &WaveformAgentConnector::updateWaveformValueData, Qt::QueuedConnection);
}

void WaveformAgentConnector::updateWaveformValueData(const QVector<WaveformSample> &data)
{
// append data to each filtered slot
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
    for (FilteredWaveformValueData *val : qAsConst(m_model->m_filtered_data)) {
#else
    for (auto it = m_model->m_filtered_data.constBegin(); it != m_model->m_filtered_data.constEnd();
         ++it) {
        FilteredWaveformValueData *val = *it;
#endif
        val->appendData(data);
    }
}

void WaveformAgentConnector::advanceSweep()
{
// filter each filtered data
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
    for (FilteredWaveformValueData *val : qAsConst(m_model->m_filtered_data)) {
#else
    for (auto it = m_model->m_filtered_data.constBegin(); it != m_model->m_filtered_data.constEnd();
         ++it) {
        FilteredWaveformValueData *val = *it;
#endif
        val->filterWaveform();
    }
}

DataModelWaveform::DataModelWaveform(QObject *parent)
  : QObject(parent)
  , m_connector(new WaveformAgentConnector(this))
  , m_connector_thread(new QThread())
{
    m_connector->moveToThread(m_connector_thread);
    m_connector_thread->start();

    for (unsigned int i = 0; i < NUMBER_WAVEFORMS; ++i) {
        m_waveform_ids.append(i);
        m_filtered_data.append(new FilteredWaveformValueData(this));
    }

    qRegisterMetaType<QVector<WaveformSample>>("QVector<WaveformSample>");
}

DataModelWaveform::~DataModelWaveform()
{
    m_connector_thread->exit(0);
    m_connector_thread->wait();
    delete m_connector;
    delete m_connector_thread;
}

FilteredWaveformValueData *DataModelWaveform::filteredWaveform(int index) const
{
    return index >= 0 && index < m_filtered_data.length() ? m_filtered_data.at(index) : nullptr;
}
