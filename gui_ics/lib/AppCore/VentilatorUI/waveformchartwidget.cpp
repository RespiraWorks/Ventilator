/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "waveformchartwidget.h"

#include <QMutexLocker>
#include <QPainter>
#include <QPainterPath>

#include "datamodelwaveform.h"
#include "datareader.h"
#include "filteredwaveformvaluedata.h"

static int PLOT_VERTICAL_OFFSET = 22;
static int PLOT_HORIZONTAL_OFFSET = 20;
static int SWEEP_GAP = 10;

WaveformChartWidget::WaveformChartWidget(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
    , m_filtered_data(nullptr)
    , m_canvas(QImage())
    , m_waveformType(-1)
    , m_auto_scale(false)
    , m_auto_size(false)
    , m_title_color(Qt::black)
    , m_title_text()
    , m_scale_text()
    , m_source_text()
    , m_message_text()
    , m_grid_color(Qt::black)
    , m_grid_labels_color(Qt::black)
    , m_grid_style(Qt::DotLine)
    , m_grid_width(1)
    , m_show_vertical_grid(true)
    , m_show_vertical_grid_labels(true)
    , m_waveform_plot_type(0)
    , m_waveform_color(Qt::black)
    , m_waveform_style(Qt::SolidLine)
    , m_waveform_width(1)
    , m_selected(false)
    , m_scale_string()
{
    connect(this, SIGNAL(updateWaveform()), this, SLOT(update()));

    m_waveFormData = new DataModelWaveform(this);
}

WaveformChartWidget::~WaveformChartWidget()
{
    if (!m_dataPath.isEmpty())
        DataReader::instance(m_dataPath)->unregisterAgentConnector(m_waveFormData->connector());
}

void WaveformChartWidget::setWaveformType(int type)
{
    if (m_waveformType != type) {
        m_waveformType = type;
        emit waveformTypeChanged();
        if (!m_dataPath.isEmpty())
            DataReader::instance(m_dataPath)->registerAgentConnector(m_waveFormData->connector(), m_waveformType);
    }
}

void WaveformChartWidget::setFilteredData(FilteredWaveformValueData *fd)
{
    if (m_filtered_data)
        disconnect(m_filtered_data);
    m_filtered_data = fd;
    connect(m_filtered_data, &FilteredWaveformValueData::renderWaveform, this, &WaveformChartWidget::renderWaveform,
            Qt::DirectConnection);
    connect(m_filtered_data, &FilteredWaveformValueData::sweepSpeedChanged, this,
            &WaveformChartWidget::sweepSpeedChanged);
    connect(m_filtered_data, &FilteredWaveformValueData::scaleChanged, this, [this]() -> void {
        // XXX formatting will be based on waveform id and units of measurement
        m_scale_string = QStringLiteral("%1 mV").arg(m_filtered_data->scale());
        emit scaleChanged();
    });

    updateSize();
    emit filteredDataChanged();
}

void WaveformChartWidget::setAutoScale(bool a)
{
    if (a != m_auto_scale) {
        m_auto_scale = a;
        emit autoScaleChanged();
    }
}

void WaveformChartWidget::setAutoSize(bool a)
{
    if (a != m_auto_size) {
        m_auto_size = a;
        emit autoSizeChanged();
    }
}

void WaveformChartWidget::setTitleColor(const QColor &c)
{
    if (c != m_title_color) {
        m_title_color = c;
        emit titleColorChanged();
    }
}

void WaveformChartWidget::setTitleText(const QString &t)
{
    if (t != m_title_text) {
        m_title_text = t;
        emit titleTextChanged();
    }
}

void WaveformChartWidget::setScaleText(const QString &t)
{
    if (t != m_scale_text) {
        m_scale_text = t;
        emit scaleTextChanged();
    }
}

void WaveformChartWidget::setSourceText(const QString &t)
{
    if (t != m_source_text) {
        m_source_text = t;
        emit sourceTextChanged();
    }
}

void WaveformChartWidget::setMessageText(const QString &t)
{
    if (t != m_message_text) {
        m_message_text = t;
        emit messageTextChanged();
    }
}

void WaveformChartWidget::setGridColor(const QColor &c)
{
    if (c != m_grid_color) {
        m_grid_color = c;
        emit gridColorChanged();
    }
}

void WaveformChartWidget::setGridLabelsColor(const QColor &c)
{
    if (c != m_grid_labels_color) {
        m_grid_labels_color = c;
        emit gridLabelsColorChanged();
    }
}

void WaveformChartWidget::setGridStyle(Qt::PenStyle s)
{
    if (s != m_grid_style) {
        m_grid_style = s;
        emit gridStyleChanged();
    }
}

void WaveformChartWidget::setGridWidth(float w)
{
    if (w > m_grid_width || w < m_grid_width) {
        m_grid_width = w;
        emit gridWidthChanged();
    }
}

void WaveformChartWidget::setShowVerticalGrid(bool s)
{
    if (s != m_show_vertical_grid) {
        m_show_vertical_grid = s;
        emit showVerticalGridChanged();
    }
}

void WaveformChartWidget::setShowVerticalGridLabels(bool s)
{
    if (s != m_show_vertical_grid_labels) {
        m_show_vertical_grid_labels = s;
        emit showVerticalGridLabelsChanged();
    }
}

void WaveformChartWidget::setWaveformPlotType(int t)
{
    if (t != m_waveform_plot_type) {
        m_waveform_plot_type = t;
        emit waveformPlotTypeChanged();
    }
}

void WaveformChartWidget::setWaveformColor(const QColor &c)
{
    if (c != m_waveform_color) {
        m_waveform_color = c;
        emit waveformColorChanged();
    }
}

void WaveformChartWidget::setWaveformStyle(Qt::PenStyle s)
{
    if (s != m_waveform_style) {
        m_waveform_style = s;
        emit waveformStyleChanged();
    }
}

void WaveformChartWidget::setWaveformWidth(float w)
{
    if (w > m_waveform_width || w < m_waveform_width) {
        m_waveform_width = w;
        emit waveformWidthChanged();
    }
}

void WaveformChartWidget::setSelected(bool s)
{
    if (s != m_selected) {
        m_selected = s;
        emit selectedChanged();
    }
}

void WaveformChartWidget::setDataPath(const QString &path)
{
    if (m_dataPath != path) {
        if (!m_dataPath.isEmpty())
            DataReader::instance(m_dataPath)->unregisterAgentConnector(m_waveFormData->connector());
        m_dataPath = path;
        emit dataPathChanged();
        if (!m_dataPath.isEmpty())
            DataReader::instance(m_dataPath)->registerAgentConnector(m_waveFormData->connector(), m_waveformType);
    }
}

float WaveformChartWidget::sweepSpeed() const
{
    if (m_filtered_data)
        return m_filtered_data->sweepSpeed();
    return 0.0f;
}

void WaveformChartWidget::setSweepSpeed(float sweepSpeed)
{
    if (m_filtered_data)
        m_filtered_data->setSweepSpeed(sweepSpeed);
}

int WaveformChartWidget::scale() const
{
    if (m_filtered_data)
        return m_filtered_data->scale();
    return 0;
}

void WaveformChartWidget::setScale(int scale)
{
    if (m_filtered_data)
        m_filtered_data->setScale(scale);
}

void WaveformChartWidget::updateSize()
{
    m_canvasMutex.lock();
    m_canvas =
            QImage(int(width() - PLOT_HORIZONTAL_OFFSET), int(height() - PLOT_VERTICAL_OFFSET), QImage::Format_ARGB32);
    m_canvas.fill(Qt::transparent);
    m_canvasMutex.unlock();

    if (m_filtered_data)
        m_filtered_data->setPlotSize(m_canvas.size());
}

void WaveformChartWidget::init(int dataIndex)
{
    setFilteredData(m_waveFormData->filteredWaveform(dataIndex));
}

void WaveformChartWidget::paint(QPainter *painter)
{
    drawName(painter);
    drawSource(painter);
    drawScale(painter);

    if (m_message_text.isEmpty()) {
        drawVerticalGrid(painter);
        QMutexLocker lock(&m_canvasMutex);
        Q_UNUSED(lock)
        painter->drawImage(PLOT_HORIZONTAL_OFFSET, PLOT_VERTICAL_OFFSET, m_canvas);
    } else {
        drawMessage(painter);
    }
}

void WaveformChartWidget::drawVerticalGrid(QPainter *painter)
{
    if (!m_filtered_data)
        return;

    QPen line_pen(m_grid_color, qreal(m_grid_width), m_grid_style);

    QPen text_pen(m_grid_labels_color);

    painter->setRenderHint(QPainter::Antialiasing, true);

    QFontMetrics fm(painter->font());
    QRect labelRect = fm.boundingRect(QLatin1Char('0'));

    painter->setPen(line_pen);
    painter->drawLine(PLOT_HORIZONTAL_OFFSET, PLOT_VERTICAL_OFFSET, int(width()), PLOT_VERTICAL_OFFSET);
    painter->drawLine(PLOT_HORIZONTAL_OFFSET, int(height() - PLOT_VERTICAL_OFFSET / 2 + PLOT_VERTICAL_OFFSET),
                      int(width()), int(height() - PLOT_VERTICAL_OFFSET / 2 + PLOT_VERTICAL_OFFSET));
    painter->drawLine(PLOT_HORIZONTAL_OFFSET, int(height()) - 1, int(width()), int(height()) - 1);

    painter->setPen(text_pen);
    painter->drawText(0, PLOT_VERTICAL_OFFSET + labelRect.height(),
                      QStringLiteral("%1").arg(m_filtered_data->scale() / 2));
    painter->drawText(0, int(height() - PLOT_VERTICAL_OFFSET / 2 + PLOT_VERTICAL_OFFSET + labelRect.height() / 2),
                      QStringLiteral("%1").arg(0));
    painter->drawText(0, int(height() - 1), QStringLiteral("%1").arg(-m_filtered_data->scale() / 2));
}

void WaveformChartWidget::drawName(QPainter *painter)
{
    if (!m_filtered_data)
        return;

    QPen text_pen(m_title_color);
    painter->setPen(text_pen);

    painter->drawText(20, 20, m_title_text);
}

void WaveformChartWidget::drawSource(QPainter *painter)
{
    if (!m_filtered_data)
        return;

    QPen text_pen(m_title_color);
    painter->setPen(text_pen);

    painter->drawText(80, 20, m_source_text);
}

void WaveformChartWidget::drawScale(QPainter *painter)
{
    if (!m_filtered_data)
        return;

    QPen text_pen(m_title_color);
    painter->setPen(text_pen);

    int left = 80;
    if (!m_source_text.isEmpty())
        left = 150;

    painter->drawText(left, 20, m_scale_string);
}

void WaveformChartWidget::drawMessage(QPainter *painter)
{

    QPen text_pen(m_title_color);
    painter->setPen(text_pen);

    QFontMetrics fm(painter->font());
    QRect labelRect = fm.boundingRect(m_message_text);

    painter->drawText(int(width() / 2 - labelRect.width() / 2), int(height() / 2), m_message_text);
}

void WaveformChartWidget::drawVerticalLineSegment(QPainter *painter, const QPoint &p1, const QPoint &p2)
{
    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    path.lineTo(QPoint(p2.x(), m_canvas.height() - 1));
    path.lineTo(QPoint(p1.x(), m_canvas.height() - 1));

    painter->fillPath(path, QBrush(m_waveform_color));
}

void WaveformChartWidget::drawLineSegment(QPainter *painter, const QPoint &p1, const QPoint &p2)
{
    painter->drawLine(p1, p2);
}

void WaveformChartWidget::redrawWaveform(QPainter *imagePainter, int sweepPosition)
{
    if (!m_filtered_data)
        return;

    int m_end = 0;
    int m_beg = 0;

    imagePainter->setCompositionMode(QPainter::CompositionMode_Source);
    imagePainter->eraseRect(0, 0, m_canvas.width(), m_canvas.height());

    for (int i = 1; i < m_filtered_data->filteredData().size(); i++) {
        m_beg = m_filtered_data->mapUnipolarValueToScreen(m_filtered_data->filteredData().at(i - 1).maxValue());
        m_end = m_filtered_data->mapUnipolarValueToScreen(m_filtered_data->filteredData().at(i).maxValue());

        if (i < sweepPosition - 1 || i > sweepPosition + 1) {
            switch (m_waveform_plot_type) {
            case 0:
                drawLineSegment(imagePainter, QPoint(i - 1, m_beg), QPoint(i, m_end));
                break;

            case 1:
                drawVerticalLineSegment(imagePainter, QPoint(i - 1, m_beg), QPoint(i, m_end));
                break;

            default:
                drawLineSegment(imagePainter, QPoint(i - 1, m_beg), QPoint(i, m_end));
                break;
            }
        }
    }
}

void WaveformChartWidget::renderWaveform(int sweepPosition, bool replot)
{
    if (!m_filtered_data)
        return;

    QMutexLocker lock(&m_canvasMutex);
    Q_UNUSED(lock)
    if (m_canvas.isNull())
        return;

    QPainter imagePainter;
    imagePainter.begin(&m_canvas);

    imagePainter.setBackground(QBrush(Qt::transparent));

    QPen pen(m_waveform_color, qreal(m_waveform_width), m_waveform_style);
    imagePainter.setPen(pen);
    imagePainter.setRenderHint(QPainter::Antialiasing, true);

    if (replot)
        redrawWaveform(&imagePainter, sweepPosition);

    imagePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    if (sweepPosition > 1 && sweepPosition - 1 < m_filtered_data->filteredData().size()) {
        int m_beg = m_filtered_data->mapUnipolarValueToScreen(
                m_filtered_data->filteredData().at(sweepPosition - 2).maxValue());
        int m_end = m_filtered_data->mapUnipolarValueToScreen(
                m_filtered_data->filteredData().at(sweepPosition - 1).maxValue());

        switch (m_waveform_plot_type) {
        case 0:
            drawLineSegment(&imagePainter, QPoint(sweepPosition - 2, m_beg), QPoint(sweepPosition - 1, m_end));
            break;

        case 1:
            drawVerticalLineSegment(&imagePainter, QPoint(sweepPosition - 2, m_beg), QPoint(sweepPosition - 1, m_end));
            break;

        default:
            drawLineSegment(&imagePainter, QPoint(sweepPosition - 2, m_beg), QPoint(sweepPosition - 1, m_end));
            break;
        }
    }

    imagePainter.setCompositionMode(QPainter::CompositionMode_Source);
    imagePainter.eraseRect(sweepPosition, 0, SWEEP_GAP, m_canvas.height());

    imagePainter.end();

    emit updateWaveform();
}
