/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef WAVEFORMCHARTWIDGET_H
#define WAVEFORMCHARTWIDGET_H

#include <QQuickPaintedItem>

#include <QImage>
#include <QMutex>

class DataReader;
class DataModelWaveform;
class FilteredWaveformValueData;

class WaveformChartWidget : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(FilteredWaveformValueData *filteredData READ filteredData WRITE setFilteredData
                       NOTIFY filteredDataChanged)

    Q_PROPERTY(int waveformType READ waveformType WRITE setWaveformType NOTIFY waveformTypeChanged)
    Q_PROPERTY(int sweepSpeed READ sweepSpeed WRITE setSweepSpeed NOTIFY sweepSpeedChanged)
    Q_PROPERTY(int scale READ scale WRITE setScale NOTIFY scaleChanged)

    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale NOTIFY autoScaleChanged)
    Q_PROPERTY(bool autoSize READ autoSize WRITE setAutoSize NOTIFY autoSizeChanged)

    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor NOTIFY titleColorChanged)
    Q_PROPERTY(QString titleText READ titleText WRITE setTitleText NOTIFY titleTextChanged)
    Q_PROPERTY(QString scaleText READ scaleText WRITE setScaleText NOTIFY scaleTextChanged)
    Q_PROPERTY(QString sourceText READ sourceText WRITE setSourceText NOTIFY sourceTextChanged)
    Q_PROPERTY(QString messageText READ messageText WRITE setMessageText NOTIFY messageTextChanged)

    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor NOTIFY gridColorChanged)
    Q_PROPERTY(QColor gridLabelsColor READ gridLabelsColor WRITE setGridLabelsColor NOTIFY
                       gridLabelsColorChanged)
    Q_PROPERTY(Qt::PenStyle gridStyle READ gridStyle WRITE setGridStyle NOTIFY gridStyleChanged)
    Q_PROPERTY(float gridWidth READ gridWidth WRITE setGridWidth NOTIFY gridWidthChanged)
    Q_PROPERTY(bool showVerticalGrid READ showVerticalGrid WRITE setShowVerticalGrid NOTIFY
                       showVerticalGridChanged)
    Q_PROPERTY(bool showVerticalGridLabels READ showVerticalGridLabels WRITE
                       setShowVerticalGridLabels NOTIFY showVerticalGridLabelsChanged)

    Q_PROPERTY(unsigned int waveformPlotType READ waveformPlotType WRITE setWaveformPlotType NOTIFY
                       waveformPlotTypeChanged)

    Q_PROPERTY(QColor waveformColor READ waveformColor WRITE setWaveformColor NOTIFY
                       waveformColorChanged)
    Q_PROPERTY(Qt::PenStyle waveformStyle READ waveformStyle WRITE setWaveformStyle NOTIFY
                       waveformStyleChanged)
    Q_PROPERTY(float waveformWidth READ waveformWidth WRITE setWaveformWidth NOTIFY
                       waveformWidthChanged)

    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

    Q_PROPERTY(QString dataPath READ dataPath WRITE setDataPath NOTIFY dataPathChanged)

public:
    WaveformChartWidget(QQuickPaintedItem *parent = nullptr);
    ~WaveformChartWidget();

    int waveformType() const { return m_waveformType; }
    void setWaveformType(int type);

    float sweepSpeed() const;
    void setSweepSpeed(float sweepSpeed);

    int scale() const;
    void setScale(int scale);

    void paint(QPainter *painter);

    FilteredWaveformValueData *filteredData() const { return m_filtered_data; }
    void setFilteredData(FilteredWaveformValueData *fd);

    bool autoScale() const { return m_auto_scale; }
    void setAutoScale(bool a);

    bool autoSize() const { return m_auto_size; }
    void setAutoSize(bool a);

    QColor titleColor() const { return m_title_color; }
    void setTitleColor(const QColor &c);

    QString titleText() const { return m_title_text; }
    void setTitleText(const QString &t);

    QString scaleText() const { return m_scale_text; }
    void setScaleText(const QString &t);

    QString sourceText() const { return m_source_text; }
    void setSourceText(const QString &t);

    QString messageText() const { return m_message_text; }
    void setMessageText(const QString &t);

    QColor gridColor() const { return m_grid_color; }
    void setGridColor(const QColor &c);

    QColor gridLabelsColor() const { return m_grid_labels_color; }
    void setGridLabelsColor(const QColor &c);

    Qt::PenStyle gridStyle() const { return m_grid_style; }
    void setGridStyle(Qt::PenStyle s);

    float gridWidth() const { return m_grid_width; }
    void setGridWidth(float w);

    bool showVerticalGrid() const { return m_show_vertical_grid; }
    void setShowVerticalGrid(bool s);

    bool showVerticalGridLabels() const { return m_show_vertical_grid_labels; }
    void setShowVerticalGridLabels(bool s);

    int waveformPlotType() const { return m_waveform_plot_type; }
    void setWaveformPlotType(int t);

    QColor waveformColor() const { return m_waveform_color; }
    void setWaveformColor(const QColor &c);

    Qt::PenStyle waveformStyle() const { return m_waveform_style; }
    void setWaveformStyle(Qt::PenStyle s);

    float waveformWidth() const { return m_waveform_width; }
    void setWaveformWidth(float w);

    bool selected() const { return m_selected; }
    void setSelected(bool s);

    QString dataPath() const { return m_dataPath; }
    void setDataPath(const QString &path);

signals:
    void updateWaveform();

    void filteredDataChanged();

    void waveformTypeChanged();
    void sweepSpeedChanged();
    void scaleChanged();

    void autoScaleChanged();
    void autoSizeChanged();

    void titleColorChanged();
    void titleTextChanged();
    void scaleTextChanged();
    void sourceTextChanged();
    void messageTextChanged();

    void gridColorChanged();
    void gridLabelsColorChanged();
    void gridStyleChanged();
    void gridWidthChanged();
    void showVerticalGridChanged();
    void showVerticalGridLabelsChanged();

    void waveformPlotTypeChanged();

    void waveformColorChanged();
    void waveformStyleChanged();
    void waveformWidthChanged();

    void selectedChanged();
    void dataPathChanged();

protected slots:
    void renderWaveform(int sweepPosition, bool replot);
    void updateSize();
    void init(int dataIndex);

private:
    void drawVerticalGrid(QPainter *painter);
    void drawName(QPainter *painter);
    void drawSource(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawMessage(QPainter *painter);

    void drawVerticalLineSegment(QPainter *painter, const QPoint &p1, const QPoint &p2);
    void drawLineSegment(QPainter *painter, const QPoint &p1, const QPoint &p2);

    void redrawWaveform(QPainter *imagePainter, int sweepPosition);

private:
    FilteredWaveformValueData *m_filtered_data;
    QMutex m_canvasMutex;
    QImage m_canvas;

    QString m_dataPath;
    int m_waveformType;

    bool m_auto_scale;
    bool m_auto_size;

    QColor m_title_color;
    QString m_title_text;
    QString m_scale_text;
    QString m_source_text;
    QString m_message_text;

    QColor m_grid_color;
    QColor m_grid_labels_color;
    Qt::PenStyle m_grid_style;
    float m_grid_width;
    bool m_show_vertical_grid;
    bool m_show_vertical_grid_labels;
    int m_waveform_plot_type;

    QColor m_waveform_color;
    Qt::PenStyle m_waveform_style;
    float m_waveform_width;

    bool m_selected;

    QString m_scale_string;

    DataModelWaveform *m_waveFormData = nullptr;
    DataReader *m_dataReader = nullptr;
};

#endif // WAVEFORMCHARTWIDGET_H
