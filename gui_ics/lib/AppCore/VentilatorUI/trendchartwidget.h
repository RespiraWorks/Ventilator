/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef TRENDCHARTWIDGET_H
#define TRENDCHARTWIDGET_H

#include <QQuickPaintedItem>

#include <QImage>
#include <QTimer>

class QAbstractItemModel;

class TrendChartWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *dataModel READ dataModel WRITE setDataModel NOTIFY dataModelChanged)
    Q_PROPERTY(QString valueRoleName READ valueRoleName WRITE setValueRoleName NOTIFY valueRoleNameChanged)
    Q_PROPERTY(
            QString timeStampRoleName READ timeStampRoleName WRITE setTimeStampRoleName NOTIFY timeStampRoleNameChanged)

    Q_PROPERTY(int valueCeiling READ valueCeiling WRITE setValueCeiling NOTIFY valueCeilingChanged)
    Q_PROPERTY(int plotStartIndex READ plotStartIndex WRITE setPlotStartIndex NOTIFY plotStartIndexChanged)
    Q_PROPERTY(int plotPointCount READ plotPointCount WRITE setPlotPointCount NOTIFY plotPointCountChanged)
    Q_PROPERTY(int plotValueOffset READ plotValueOffset WRITE setPlotValueOffset NOTIFY plotValueOffsetChanged)
    Q_PROPERTY(int markedNodeIndex READ markedNodeIndex WRITE setMarkedNodeIndex NOTIFY markedNodeIndexChanged)
    Q_PROPERTY(qreal markerAreaSize READ markerAreaSize WRITE setMarkerAreaSize NOTIFY markerAreaSizeChanged)

    Q_PROPERTY(QColor plotColor READ plotColor WRITE setPlotColor NOTIFY plotColorChanged)
    Q_PROPERTY(qreal plotLineWidth READ plotLineWidth WRITE setPlotLineWidth NOTIFY plotLineWidthChanged)
    Q_PROPERTY(QString nodeMarkerSource READ nodeMarkerSource WRITE setNodeMarkerSource NOTIFY nodeMarkerSourceChanged)

    Q_PROPERTY(qreal headerHeight READ headerHeight WRITE setHeaderHeight NOTIFY headerHeightChanged)
    Q_PROPERTY(QColor headerTextColor READ headerTextColor WRITE setHeaderTextColor NOTIFY headerTextColorChanged)

    Q_PROPERTY(QFont headerTextFont READ headerTextFont WRITE setHeaderTextFont NOTIFY headerTextFontChanged)

    Q_PROPERTY(
            int headerBottomOffset READ headerBottomOffset WRITE setHeaderBottomOffset NOTIFY headerBottomOffsetChanged)

    Q_PROPERTY(bool useTimer READ useTimer WRITE setUseTimer NOTIFY useTimerChanged)
    Q_PROPERTY(int timerInterval READ timerInterval WRITE setTimerInterval NOTIFY timerIntervalChanged)

    Q_PROPERTY(bool drawLineToEnd READ drawLineToEnd WRITE setDrawLineToEnd NOTIFY drawLineToEndChanged)
    Q_PROPERTY(int plotLeftMargin READ plotLeftMargin WRITE setPlotLeftMargin NOTIFY plotLeftMarginChanged)
    Q_PROPERTY(int plotRightMargin READ plotRightMargin WRITE setPlotRightMargin NOTIFY plotRightMarginChanged)

public:
    explicit TrendChartWidget(QQuickItem *parent = nullptr);

    QAbstractItemModel *dataModel() const { return m_dataModel; }
    void setDataModel(QAbstractItemModel *dataModel);

    QString valueRoleName() const { return m_valueRoleName; }
    void setValueRoleName(const QString &roleName);

    QString timeStampRoleName() const { return m_valueRoleName; }
    void setTimeStampRoleName(const QString &timeStampRoleName);

    int valueCeiling() const { return m_valueCeiling; }
    void setValueCeiling(int valueCeiling);

    int plotStartIndex() const { return m_plotStartIndex; }
    void setPlotStartIndex(int plotStartIndex);

    int plotPointCount() const { return m_plotPointCount; }
    void setPlotPointCount(int pointCount);

    int plotValueOffset() const { return m_plotValueOffset; }
    void setPlotValueOffset(int offset);

    int markedNodeIndex() const { return m_markedNodeIndex; }
    void setMarkedNodeIndex(int markedNodeIndex);

    qreal markerAreaSize() const { return m_markerAreaSize; }
    void setMarkerAreaSize(qreal areaSize);

    QColor plotColor() const { return m_plotColor; }
    void setPlotColor(const QColor &color);

    qreal plotLineWidth() const { return m_plotLineWidth; }
    void setPlotLineWidth(qreal plotLineWidth);

    QString nodeMarkerSource() const { return m_nodeMarkerSource; }
    void setNodeMarkerSource(const QString &nodeMarkerSource);

    qreal headerHeight() const { return m_headerHeight; }
    void setHeaderHeight(const qreal &headerHeight);

    QColor headerTextColor() const { return m_headerTextColor; }
    void setHeaderTextColor(const QColor &color);

    QFont headerTextFont() const { return m_headerTextFont; }
    void setHeaderTextFont(const QFont &headerTextFont);

    int headerBottomOffset() const { return m_headerBottomOffset; }
    void setHeaderBottomOffset(int headerBottomOffset);

    bool useTimer() const { return m_useTimer; }
    void setUseTimer(bool useTimer);

    int timerInterval() const { return m_timerInterval; }
    void setTimerInterval(int timerInterval);

    bool drawLineToEnd() const { return m_drawLineToEnd; }
    void setDrawLineToEnd(bool drawLineToEnd);

    int plotLeftMargin() const { return m_plotLeftMargin; }
    void setPlotLeftMargin(int plotLeftMargin);

    int plotRightMargin() const { return m_plotRightMargin; }
    void setPlotRightMargin(int plotRightMargin);

    Q_INVOKABLE int nodeIndexAt(qreal x, qreal y) const;

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);

signals:
    void dataModelChanged();
    void valueRoleNameChanged();
    void timeStampRoleNameChanged();

    void baseLinePositionChanged();
    void valueCeilingChanged();
    void plotStartIndexChanged();
    void plotPointCountChanged();
    void plotValueOffsetChanged();
    void markedNodeIndexChanged();
    void markerAreaSizeChanged();

    void plotColorChanged();
    void plotLineWidthChanged();
    void nodeMarkerSourceChanged();
    void headerHeightChanged();
    void headerTextColorChanged();
    void headerTextFontChanged();
    void headerBottomOffsetChanged();
    void useTimerChanged();
    void timerIntervalChanged();
    void drawLineToEndChanged();
    void plotLeftMarginChanged();
    void plotRightMarginChanged();

protected slots:
    void scheduleUpdate();

private:
    void syncValueRole();
    void syncTimeStampRole();
    double getCurrentHour();
    void calculateStartPoint();

    QAbstractItemModel *m_dataModel = nullptr;
    QString m_valueRoleName = QStringLiteral("value");
    QString m_timeStampRoleName = QStringLiteral("timeStamp");
    int m_valueRole = -1;
    int m_timeStampRole = -1;

    int m_valueCeiling = 256;
    int m_plotStartIndex = 0;
    int m_plotPointCount = 24;
    int m_plotValueOffset = 0;
    int m_markedNodeIndex = -1;
    qreal m_markerAreaSize = 50.0;
    qreal m_headerHeight = 50.0;

    QColor m_plotColor = QColor(137, 196, 53, 128);
    QColor m_headerTextColor = QColor(217, 236, 255, 128);
    qreal m_plotLineWidth = 2.0;
    QString m_nodeMarkerSource;
    QImage m_nodeMarkerImage;

    QFont m_headerTextFont;
    int m_headerBottomOffset = 15;
    double m_currentHour = 0.0;
    bool m_useTimer = false;
    QTimer m_timer;
    int m_timerInterval = 5000;
    bool m_drawLineToEnd = true;
    int m_plotLeftMargin = 0;
    int m_plotRightMargin = 0;
};

#endif // TRENDCHARTWIDGET_H
