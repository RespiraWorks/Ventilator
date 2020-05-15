import QtQuick 2.0
import com.ics.waveformchart 1.0

//PREVIEW IMAGE qrc:/MedicalPlottingComponentSet/PreviewAssets/icon.png
Item {
    id: root

    width: 256
    height: 128

    signal nodeSelected(int index)

    //MODEL
    property alias dataModel: plot.dataModel
    //STRING value
    property alias valueRoleName: plot.valueRoleName

    //STRING timeStamp
    property alias timeStampRoleName: plot.timeStampRoleName

    //INT 256
    property alias valueCeiling: plot.valueCeiling
    //INT 0
    property alias plotStartIndex: plot.plotStartIndex
    //INT 24
    property alias plotPointCount: plot.plotPointCount
    //INT 0
    property alias plotValueOffset: plot.plotValueOffset
    property int markedNodeIndex: -1
    //REAL 50.0
    property alias markerAreaSize: plot.markerAreaSize

    //COLOR plotColor
    property alias plotColor: plot.plotColor
    //REAL 2.0
    property alias plotLineWidth: plot.plotLineWidth
    //ASSET PLACEHOLDER
    property alias nodeMarkerSource: plot.nodeMarkerSource

    //REAL 50.0
    property alias headerHeight: plot.headerHeight

    //COLOR plotColor
    property alias headerTextColor: plot.headerTextColor

    //FONT textFont
    property alias headerTextFont: plot.headerTextFont

    //INT 15
    property alias headerBottomOffset: plot.headerBottomOffset

    //BOOL FALSE
    property alias useTimer: plot.useTimer

    //INT 5000
    property alias timerInterval: plot.timerInterval

    property bool autoSelect: true

    TrendChartWidget {
        id: plot

        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            var index = plot.nodeIndexAt(mouse.x, mouse.y)
            if (root.autoSelect)
                plot.markedNodeIndex = index
            root.nodeSelected(index)
        }
    }

    onMarkedNodeIndexChanged: plot.markedNodeIndex = markedNodeIndex
}
