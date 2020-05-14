/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "filteredwaveformvaluedata.h"

#include "global_constants.h"
#include "waveformfilter.h"

FilteredWaveformValueData::FilteredWaveformValueData(QObject *parent)
  : QObject(parent)
  , m_sweep_speed(0.0f)
  , m_plot_size(QSize(0, 0))
  , m_scale(0)
  , m_sweep_position(0)
  , m_raw_data(QVector<WaveformSample>())
  , m_filtered_data(QVector<FilteredSample>())
  , m_full_redraw(false)
  , m_mutex(QMutex::Recursive)
{
}

void FilteredWaveformValueData::setSweepSpeed(float sweepSpeed)
{
    if (m_sweep_speed > sweepSpeed || m_sweep_speed < sweepSpeed) {
        QMutexLocker locker(&m_mutex);
        Q_UNUSED(locker)

        m_sweep_speed = sweepSpeed;
        clearWaveform();

        emit sweepSpeedChanged();
    }
}

void FilteredWaveformValueData::setScale(int scale)
{
    if (m_scale != scale) {
        QMutexLocker locker(&m_mutex);
        Q_UNUSED(locker)

        m_scale = scale;
        m_full_redraw = true;

        emit scaleChanged();
    }
}

void FilteredWaveformValueData::setPlotSize(const QSize &size)
{
    if (m_plot_size != size) {
        QMutexLocker locker(&m_mutex);
        Q_UNUSED(locker)

        m_plot_size = size;

        m_filtered_data = QVector<FilteredSample>(m_plot_size.width());
        m_filtered_data.resize(m_plot_size.width());

        emit plotSizeChanged();
    }
}

void FilteredWaveformValueData::appendData(const QVector<WaveformSample> &data)
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker)
    m_raw_data.append(data);
    if (m_raw_data.size() >= MAX_RAW_PLOT_DATA_SIZE)
        m_raw_data.erase(m_raw_data.begin(), m_raw_data.begin() + MAX_RAW_PLOT_DATA_SIZE_HALF);
}

void FilteredWaveformValueData::clearWaveform()
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker)

    m_raw_data.clear();

    m_filtered_data = QVector<FilteredSample>(m_plot_size.width());
    m_filtered_data.resize(m_plot_size.width());

    m_sweep_position = 0;

    m_full_redraw = true;
}

void FilteredWaveformValueData::filterWaveform()
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker)

    if (m_sweep_speed <= 0)
        return;

    float samples_per_pixel = DATA_RATE / (m_sweep_speed / PIXEL_PITCH);

    int use_samples = int(samples_per_pixel);

    if (samples_per_pixel > 0.0f && samples_per_pixel < 1.0f)
        use_samples = 1;

    while (m_raw_data.size() >= use_samples) {
        FilteredSample filtered = WaveformFilter::filterWaveformMinMax(m_raw_data.mid(0, use_samples));

        m_raw_data.remove(0, use_samples);

        if (m_filtered_data.size() >= m_plot_size.width()) {
            m_filtered_data.removeLast();
        }

        m_filtered_data.prepend(filtered);

        emit renderWaveform(m_sweep_position, m_full_redraw);

        if (samples_per_pixel > 0.0f && samples_per_pixel < 1.0f) {
            if (m_filtered_data.size() >= m_plot_size.width()) {
                m_filtered_data.removeLast();
            }

            m_filtered_data.prepend(filtered);

            emit renderWaveform(m_sweep_position, m_full_redraw);
        }
    }
}

int FilteredWaveformValueData::mapUnipolarValueToScreen(int value) const
{
    float gradient = m_plot_size.height() / float(m_scale);

    float y_intercept = (m_plot_size.height() / 2.0f);

    value = int(y_intercept - (gradient * value));

    if (value < 0)
        value = 0;

    if (value > m_plot_size.height())
        value = m_plot_size.height();

    return value;
}

void FilteredWaveformValueData::printSweepError(qint64 elapsed)
{
    float error =
            float(((m_plot_size.width() - double(m_sweep_speed / PIXEL_PITCH) * (elapsed * 1e-9))
                   / m_plot_size.width())
                  * 100.0);

    m_sweep_error = error;
    emit sweepErrorChanged();
}
