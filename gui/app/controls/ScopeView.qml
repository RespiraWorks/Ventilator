import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0

import ".."

Item {

    id: root
    width: 606; height: 144

    property double yMin: -1.5
    property double yMax: 1.5
    property double rangeInSeconds: 30

    property alias name: nameLabel.text
    property alias unit: unitLabel.text

    property alias showBottomLine: bottomLine.visible
    property alias dataset: timeSeriesGraph.dataset
    property alias color: timeSeriesGraph.color

    Text {
        id: yMaxLabel
        anchors {
            top: parent.top; topMargin: 8
            right: timeSeriesGraph.left; rightMargin: 8
            left: parent.left; leftMargin: 22
        }

        font: Style.theme.font.graphLabel
        horizontalAlignment: Text.AlignRight
        text: yMax.toFixed(0).toString()
        color: Style.theme.color.textAlternative
    }

    Text {
        id: yMinLabel
        anchors {
            left: parent.left; leftMargin: 8
            right: timeSeriesGraph.left; rightMargin: 8
            top: yMaxLabel.bottom; topMargin: 90
        }
        font: Style.theme.font.graphLabel
        horizontalAlignment: Text.AlignRight
        text: yMin.toFixed(0).toString()
        color: Style.theme.color.textAlternative
    }

    Row {
        anchors {
            top: parent.top; topMargin: 8
            left: yMaxLabel.right; leftMargin: 8
        }
        spacing: 4

        Text {
            id: nameLabel
            color: Style.theme.color.textPrimary
            font: Style.theme.font.parameterDisplay
        }

        Text {
            id: unitLabel
            textFormat: Text.RichText
            color: Style.theme.color.textAlternative
            font: Style.theme.font.parameterDisplay
        }
    }

    TimeSeriesGraph {
        id: timeSeriesGraph
        color: "white"
        anchors {
            top: parent.top; topMargin: 12
            left: parent.left; leftMargin: 48
            right: parent.right; rightMargin:4
            bottom: parent.bottom; bottomMargin: 4
        }
        minValue: root.yMin
        maxValue: root.yMax
        rangeInSeconds: root.rangeInSeconds
    }

    Rectangle {
        id: bottomLine
        anchors {
            bottom: parent.bottom; bottomMargin: 0
            left: parent.left; leftMargin: 4
            right: parent.right; rightMargin: 4
        }
        width: parent.width; height: 1
        color: "#435360"
    }
}
