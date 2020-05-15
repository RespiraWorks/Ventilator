/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef FILTEREDWAVEFORMVALUEDATA_H
#define FILTEREDWAVEFORMVALUEDATA_H

#include <QObject>

#include <QElapsedTimer>
#include <QMutex>
#include <QSize>
#include <QVector>

#include "filteredsample.h"
#include "waveformsample.h"

class FilteredWaveformValueData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector<FilteredSample> filteredData READ filteredData)

    Q_PROPERTY(float sweepSpeed READ sweepSpeed WRITE setSweepSpeed NOTIFY sweepSpeedChanged)
    Q_PROPERTY(int scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QSize plotSize READ plotSize WRITE setPlotSize NOTIFY plotSizeChanged)

    Q_PROPERTY(float sweepError READ sweepError NOTIFY sweepErrorChanged)

public:
    explicit FilteredWaveformValueData(QObject *parent = nullptr);

    QVector<FilteredSample> filteredData() const { return m_filtered_data; }

    float sweepSpeed() const { return m_sweep_speed; }
    void setSweepSpeed(float sweepSpeed);

    int scale() const { return m_scale; }
    void setScale(int scale);

    QSize plotSize() const { return m_plot_size; }
    void setPlotSize(const QSize &size);

    int mapUnipolarValueToScreen(int value) const;

    float sweepError() const { return m_sweep_error; }

signals:
    void renderWaveform(int sweep_position, bool replot = false);

    void sweepSpeedChanged();
    void scaleChanged();
    void plotSizeChanged();

    void sweepErrorChanged();

public slots:
    void appendData(const QVector<WaveformSample> &data);
    void clearWaveform();
    void filterWaveform();

private:
    void printSweepError(qint64 elapsed);

private:
    float m_sweep_speed;
    QSize m_plot_size;
    int m_scale;

    int m_sweep_position;

    QVector<WaveformSample> m_raw_data;
    QVector<FilteredSample> m_filtered_data;

    bool m_full_redraw;

    QMutex m_mutex;

    float m_sweep_error;
};

#endif // FILTEREDWAVEFORMVALUEDATA_H
