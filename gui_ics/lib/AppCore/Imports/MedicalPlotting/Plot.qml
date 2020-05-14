import QtQuick 2.0
import com.ics.waveformchart 1.0

//PREVIEW IMAGE qrc:/MedicalPlottingComponentSet/PreviewAssets/icon.png
Item {
    id: root

    width: 256
    height: 128

    //COLOR plotColor
    property color plotColor: "#89c435"
    //REAL 2.0
    property alias waveformWidth: wave.waveformWidth
    property bool dottedLine: false
    property int waveformType: 0
    property int waveformPlotType: 0
    property int plotScale: 2800

    property string dataPath: ":/MedicalPlottingDemoData/b0_3s.txt"

    WaveformChartWidget {
        id: wave

        anchors.fill: parent
        anchors.leftMargin: -20

        autoScale: false
        autoSize:  false

        titleColor: "transparent"

        gridColor: "transparent"
        gridLabelsColor: "transparent"
        showVerticalGrid: false
        showVerticalGridLabels: false

        waveformType: root.waveformType
        waveformPlotType: root.waveformPlotType
        waveformColor: root.plotColor
        waveformStyle: root.dottedLine ? Qt.DotLine : Qt.SolidLine
        waveformWidth: 2.0

        selected: false

        Component.onCompleted: {
            dataPath = root.dataPath
            init(0)
            sweepSpeed = 50
            scale = root.plotScale
        }

        onWidthChanged: updateSize()
        onHeightChanged: updateSize()
    }

    onDataPathChanged: {
        wave.dataPath = dataPath
    }
    onPlotScaleChanged: {
        wave.scale = plotScale
    }
}
