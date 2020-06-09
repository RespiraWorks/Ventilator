import QtQuick 2.11
import QtQuick.Controls 2.4
import QtCharts 2.2
import ".."

Item {

    id: root
    width: 606; height: 144

    property double yMin: -1.5
    property double yMax: 1.5

    property alias name: nameLabel.text
    property alias unit: unitLabel.text

    property alias showBottomLine: bottomLine.visible

    property alias serie: lineSeries

    property alias chart: chartView

    Text {
        id: yMaxLabel
        anchors {
            top: parent.top; topMargin: 8
            left: parent.left; leftMargin: 22
        }

        font: Style.theme.font.graphLabel
        horizontalAlignment: Text.AlignLeft
        text: yMax.toFixed(0).toString()
        color: Style.theme.color.textAlternative
    }

    Text {
        id: yMinLabel
        anchors {
            left: parent.left; leftMargin: 22
            top: yMaxLabel.bottom; topMargin: 90
        }
        font: Style.theme.font.graphLabel
        horizontalAlignment: Text.AlignLeft
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

    ChartView
    {
        id: chartView

        //due to a bug on Qt charts 5.11, which is used at the moment
        // in Raspiberry OS, plotArea property is marked as read-only
        // making it impossible to control the drawing area.
        // This was fixed in Qt 5.13 (maybe even on 5.12). Once we have
        // the fix we can uncomment the following code as the proper
        // solution:

//         anchors {
//            top: parent.top; topMargin: 0
//            left: parent.left; leftMargin: 48
//            right: parent.right; rightMargin:8
//            bottom: parent.bottom; bottomMargin: 8
//        }
        //plotArea: Qt.rect(0, 0, chartView.width, chartView.height)

        //meanwhile we have to do guess work and some magic numbers to
        //increase the size of chartview beyond its parent in order to
        //get a proper rectangle for the plotArea

        anchors {
            top: parent.top; topMargin: -30
            left: parent.left; leftMargin: -20
            right: parent.right; rightMargin:-40
        }
        width: parent.width + 100
        height: parent.height + 70

        backgroundColor: "transparent"
        legend.visible: false
        antialiasing: true


        property string name

        ValueAxis
        {
            id: timeAxis
            min: -30
            max: 0
            labelsVisible: false
            gridVisible:false
            lineVisible: false
        }

        ValueAxis
        {
            id: valueAxis
            min: root.yMin
            max: root.yMax
            labelsVisible: false
            gridVisible:false
            lineVisible: false
        }

        AreaSeries {
            id: areaSeries
            useOpenGL: true

            borderColor: "white"
            borderWidth: 0

            color: "#202A32"
            axisX: timeAxis
            axisY: valueAxis
            upperSeries: lineSeries
        }

        LineSeries
        {
            id: baselineSeries
            color: "#1F579C"
            useOpenGL: true
            style: Qt.DashLine
            axisX: timeAxis
            axisY: valueAxis

            pointsVisible: false
            width: 1
            XYPoint { x: -0xFFFFFF; y: 0 }
            XYPoint { x: 0xFFFFFF; y: 0 }
        }

        LineSeries
        {
            id: lineSeries
            color: "white"
            useOpenGL: true
            axisX: timeAxis
            axisY: valueAxis

            pointsVisible: false
            width: 2
        }
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
