/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "trendchartwidget.h"

#include <QAbstractListModel>
#include <QPainter>
#include <QPainterPath>
#include <QTime>

TrendChartWidget::TrendChartWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    // Calculate current hour, can be 9.0, 10.5 14.0, 16.5
    m_currentHour = getCurrentHour();

    // Calculate start point that now depends from current hour
    calculateStartPoint();

    connect(&m_timer, &QTimer::timeout, [this]() {
        double currentHour = getCurrentHour();
        // If hour is change update chart
        if (m_currentHour != currentHour) {
            m_currentHour = currentHour;
            update();
        }
    });
}

void TrendChartWidget::setDataModel(QAbstractItemModel *dataModel)
{
    if (m_dataModel != dataModel) {
        if (m_dataModel)
            disconnect(m_dataModel);
        m_dataModel = dataModel;
        emit dataModelChanged();
        if (m_dataModel) {
            connect(m_dataModel, &QAbstractListModel::rowsInserted, this, &TrendChartWidget::scheduleUpdate);
            connect(m_dataModel, &QAbstractListModel::rowsRemoved, this, &TrendChartWidget::scheduleUpdate);
            connect(m_dataModel, &QAbstractListModel::rowsMoved, this, &TrendChartWidget::scheduleUpdate);
        }
        syncValueRole();
        syncTimeStampRole();
    }
}

void TrendChartWidget::setValueRoleName(const QString &roleName)
{
    if (m_valueRoleName != roleName) {
        m_valueRoleName = roleName;
        emit valueRoleNameChanged();
        syncValueRole();
    }
}

void TrendChartWidget::setValueCeiling(int valueCeiling)
{
    if (m_valueCeiling != valueCeiling && valueCeiling >= 1) {
        m_valueCeiling = valueCeiling;
        emit valueCeilingChanged();
        update();
    }
}

void TrendChartWidget::setPlotStartIndex(int plotStartIndex)
{
    if (m_plotStartIndex != plotStartIndex) {
        m_plotStartIndex = qMax(0, plotStartIndex);
        emit plotStartIndexChanged();
        update();
    }
}

void TrendChartWidget::setPlotPointCount(int pointCount)
{
    if (m_plotPointCount != pointCount && pointCount >= 2) {
        m_plotPointCount = pointCount;
        emit plotPointCountChanged();
        update();
    }
}

void TrendChartWidget::setPlotValueOffset(int offset)
{
    if (m_plotValueOffset != offset) {
        m_plotValueOffset = offset;
        emit plotValueOffsetChanged();
        update();
    }
}

void TrendChartWidget::setMarkedNodeIndex(int markedNodeIndex)
{
    if (m_markedNodeIndex != markedNodeIndex) {
        m_markedNodeIndex = markedNodeIndex;
        emit markedNodeIndexChanged();
        update();
    }
}

void TrendChartWidget::setMarkerAreaSize(qreal areaSize)
{
    if (m_markerAreaSize < areaSize || m_markerAreaSize > areaSize) {
        m_markerAreaSize = areaSize;
        emit markerAreaSizeChanged();
    }
}

void TrendChartWidget::setPlotColor(const QColor &color)
{
    if (m_plotColor != color) {
        m_plotColor = color;
        emit plotColorChanged();
        update();
    }
}

void TrendChartWidget::setPlotLineWidth(qreal plotLineWidth)
{
    if ((m_plotLineWidth < plotLineWidth || m_plotLineWidth > plotLineWidth) && plotLineWidth >= 1.0) {
        m_plotLineWidth = plotLineWidth;
        emit plotLineWidthChanged();
        update();
    }
}

void TrendChartWidget::setNodeMarkerSource(const QString &nodeMarkerSource)
{
    if (m_nodeMarkerSource != nodeMarkerSource) {
        m_nodeMarkerSource = nodeMarkerSource;
        if (m_nodeMarkerSource.startsWith(QStringLiteral("file:")))
            m_nodeMarkerImage = QImage(QUrl(m_nodeMarkerSource).toLocalFile());
        else if (m_nodeMarkerSource.startsWith(QStringLiteral("qrc:")))
            m_nodeMarkerImage = QImage(m_nodeMarkerSource.right(m_nodeMarkerSource.length() - 3));
        else
            m_nodeMarkerImage = QImage(m_nodeMarkerSource);
        emit nodeMarkerSourceChanged();
        update();
    }
}

int TrendChartWidget::nodeIndexAt(qreal x, qreal y) const
{
    int res = -1;

    // Adjust for left margins
    x = x - static_cast<qreal>(m_plotLeftMargin);

    double realHeight = height() - m_headerHeight;

    double yPixelStep = realHeight / m_valueCeiling;
    double xPixelStep = width() / m_plotPointCount;
    int index = m_plotStartIndex + qRound(x / xPixelStep);

    // In case when reach max model items, restart index
    if (m_dataModel && index >= m_dataModel->rowCount())
        index -= m_dataModel->rowCount();

    if (index >= 0 && m_dataModel) {
        const QVariant val = m_dataModel->data(m_dataModel->index(index, 0), m_valueRole);
        bool ok = false;
        int value = qMin(m_valueCeiling, qMax(0, val.toInt(&ok) + m_plotValueOffset));
        if (ok) {
            double yNodeValue = height() - (value * yPixelStep);
            if (yNodeValue >= (y - m_markerAreaSize) && yNodeValue <= (y + m_markerAreaSize))
                res = index;
        }
    }
    return res;
}

void TrendChartWidget::paint(QPainter *painter)
{
    calculateStartPoint();

    double realHeight = height() - m_headerHeight;

    double yPixelStep = realHeight / m_valueCeiling;
    double xPixelStep = width() / m_plotPointCount;

    QPointF markedNodePosition(-1.0, -1.0);
    QPainterPath path;
    path.moveTo(0.0, height());
    if (m_dataModel) {
        double xPos = static_cast<double>(m_plotLeftMargin);

        // Set proper font for text in header
        painter->setPen(m_headerTextColor);
        painter->setFont(m_headerTextFont);
        QFontMetrics fontMetrics(m_headerTextFont, painter->device());

        for (int i = 0; i <= m_plotPointCount; ++i) {
            int index = i + m_plotStartIndex;
            if (index == m_dataModel->rowCount())
                break;

            // Get proper value and timestamp
            const QVariant val = m_dataModel->data(m_dataModel->index(index, 0), m_valueRole);
            const QVariant timeStamp = m_dataModel->data(m_dataModel->index(index, 0), m_timeStampRole);

            bool ok = false;
            int value = qMin(m_valueCeiling, qMax(0, val.toInt(&ok) + m_plotValueOffset));
            if (ok) {
                if (i == 0)
                    path.moveTo(xPos, height() - (value * yPixelStep));
                else
                    path.lineTo(xPos, height() - (value * yPixelStep));

                if (!timeStamp.toString().isEmpty()) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
                    painter->drawText(int(xPos - fontMetrics.horizontalAdvance(timeStamp.toString()) / 2),
#else
                    painter->drawText(int(xPos - fontMetrics.width(timeStamp.toString()) / 2),
#endif
                                      int(m_headerHeight - m_headerBottomOffset), timeStamp.toString());
                }

                if (index == m_markedNodeIndex) {
                    markedNodePosition.setX(xPos);
                    markedNodePosition.setY(height() - (value * yPixelStep));
                }
            } else {
                break;
            }
            xPos += xPixelStep;

            if (xPos > width() - m_plotRightMargin) {
                break;
            }
        }
    }
    if ((path.currentPosition().x() < width() - m_plotRightMargin) && m_drawLineToEnd) {
        path.lineTo(width() - m_plotRightMargin, path.currentPosition().y());
    }

    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setPen(QPen(m_plotColor, m_plotLineWidth));
    painter->drawPath(path);

    if (markedNodePosition.x() > -1.0 && markedNodePosition.y() > -1.0 && !m_nodeMarkerImage.isNull())
        painter->drawImage(markedNodePosition
                                   - QPointF(m_nodeMarkerImage.width() * 0.5, m_nodeMarkerImage.height() * 0.5),
                           m_nodeMarkerImage);
}

void TrendChartWidget::scheduleUpdate()
{
    update();
}

void TrendChartWidget::syncValueRole()
{
    if (m_dataModel) {
        const auto roleNames = m_dataModel->roleNames();
        m_valueRole = -1;
        for (auto it = roleNames.constBegin(); it != roleNames.constEnd(); ++it) {
            if (QLatin1String(it.value()) == m_valueRoleName) {
                m_valueRole = it.key();
                break;
            }
        }
        update();
    }
}

void TrendChartWidget::syncTimeStampRole()
{
    if (m_dataModel) {
        const auto roleNames = m_dataModel->roleNames();
        m_timeStampRole = -1;
        for (auto it = roleNames.constBegin(); it != roleNames.constEnd(); ++it) {
            if (QLatin1String(it.value()) == m_timeStampRoleName) {
                m_timeStampRole = it.key();
                break;
            }
        }
        update();
    }
}

double TrendChartWidget::getCurrentHour()
{
    QTime currentTime = QTime::currentTime();
    double currentHour = currentTime.hour() + ((currentTime.minute() > 30) ? 0.5 : 0);
    return currentHour;
}

void TrendChartWidget::calculateStartPoint()
{
    if (!m_useTimer) {
        return;
    }

    // Calculate start, depends from current hour
    if (m_currentHour >= 12)
        m_plotStartIndex = 2 * m_currentHour - m_plotPointCount;
    else
        m_plotStartIndex = 2 * m_currentHour + m_plotPointCount;
}

void TrendChartWidget::setTimerInterval(int timerInterval)
{
    if (m_timerInterval != timerInterval) {
        m_timerInterval = timerInterval;
        emit timerIntervalChanged();

        // Restart timer with new interval in milliseconds, if it used
        if (useTimer())
            m_timer.start(m_timerInterval);
    }
}

void TrendChartWidget::setDrawLineToEnd(bool drawLineToEnd)
{
    if (m_drawLineToEnd != drawLineToEnd) {
        m_drawLineToEnd = drawLineToEnd;
        emit drawLineToEndChanged();
        update();
    }
}

void TrendChartWidget::setPlotLeftMargin(int plotLeftMargin)
{
    if (m_plotLeftMargin != plotLeftMargin) {
        m_plotLeftMargin = plotLeftMargin;
        emit plotLeftMarginChanged();
        update();
    }
}

void TrendChartWidget::setPlotRightMargin(int plotRightMargin)
{
    if (m_plotRightMargin != plotRightMargin) {
        m_plotRightMargin = plotRightMargin;
        emit plotRightMarginChanged();
        update();
    }
}

void TrendChartWidget::setUseTimer(bool useTimer)
{
    if (m_useTimer != useTimer) {
        m_useTimer = useTimer;
        emit useTimerChanged();

        // If set to use timer, start it, otherwise stop it
        m_useTimer ? m_timer.start(m_timerInterval) : m_timer.stop();
    }
}

void TrendChartWidget::setHeaderBottomOffset(int headerBottomOffset)
{
    if (m_headerBottomOffset != headerBottomOffset) {
        m_headerBottomOffset = headerBottomOffset;
        emit headerBottomOffsetChanged();
        update();
    }
}

void TrendChartWidget::setHeaderTextFont(const QFont &headerTextFont)
{
    if (m_headerTextFont != headerTextFont) {
        m_headerTextFont = headerTextFont;
        emit headerTextFontChanged();
        update();
    }
}

void TrendChartWidget::setHeaderHeight(const qreal &headerHeight)
{
    if (m_headerHeight != headerHeight) {
        m_headerHeight = headerHeight;
        emit headerHeightChanged();
        update();
    }
}

void TrendChartWidget::setHeaderTextColor(const QColor &color)
{
    if (m_headerTextColor != color) {
        m_headerTextColor = color;
        emit headerTextColorChanged();
        update();
    }
}

void TrendChartWidget::setTimeStampRoleName(const QString &timeStampRoleName)
{
    if (m_timeStampRoleName != timeStampRoleName) {
        m_timeStampRoleName = timeStampRoleName;
        emit timeStampRoleNameChanged();
        syncTimeStampRole();
    }
}
