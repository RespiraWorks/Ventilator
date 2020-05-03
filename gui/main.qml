import QtQuick 2.11
import QtQuick.Layouts 1.3

Item
{
    id: mainWindow
    width: 1024
    height: 600


    Timer // TODO: Make data sources be updated as we get data
    {
        id: refreshTimer
        interval: 1 / 15 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            stateContainer.update(
                pressureView.series(0),
                flowView.series(0),
                tidalVolumeView.series(0));
            pressureReadout.text = pressureView.series(0).at(pressureView.series(0).count-1).y.toFixed(2).toString()
            flowReadout.text = flowView.series(0).at(flowView.series(0).count-1).y.toFixed(2).toString()
            volumeReadout.text = tidalVolumeView.series(0).at(flowView.series(0).count-1).y.toFixed(2).toString()
        }
    }


    ControlPanel {

        id: controlPanel
        anchors.right: scopeGridLayout.left
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        Layout.leftMargin: 10
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.minimumHeight: 85
        Layout.minimumWidth: 160
        Layout.maximumWidth: 180
        spacing: 7.7

    }

    GridLayout {
        id: scopeGridLayout
        columnSpacing: 0
        rowSpacing: -20
        anchors.left: parent.left
        anchors.leftMargin: 195
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        Layout.minimumHeight: 400
        Layout.minimumWidth: 500
        Layout.fillHeight: true
        Layout.fillWidth: true
        flow: GridLayout.TopToBottom
        rows: 3

        ScopeView
        {
            id: flowView
            name: "Flow [mL/min]"
            color: "green"
            Layout.fillHeight: true
            Layout.fillWidth: true
            height: parent.height/3
            width: parent.width
        }

        ScopeView
        {
            id: tidalVolumeView
            name: "Tidal Volume [mL]"
            color: "yellow"
            Layout.fillHeight: true
            Layout.fillWidth: true
            height: parent.height/3
            width: parent.width
        }

        ScopeView {
            id: pressureView
            name: "Pressure [cmH2O]"
            color: "#4f67ff"
            Layout.fillHeight: true
            Layout.fillWidth: true
            height: parent.height/3
            width: parent.width
        }



    }

    Text {
        id: flowReadout
        x: 901
        y: 15
        width: 62
        height: 41
        color: "green"
        text: qsTr("0")
        styleColor: "#ffffff"
        font.pixelSize: 32
    }

    Text {
        id: volumeReadout
        x: 901
        y: 202
        width: 62
        height: 63
        color: "yellow"
        text: qsTr("0")
        styleColor: "#ffffff"
        font.pixelSize: 32
    }

    Text {
        id: pressureReadout
        x: 901
        y: 395
        width: 62
        height: 52
        color: "#4f67ff"
        text: qsTr("0")
        styleColor: "#ffffff"
        font.pixelSize: 32
    }

}

/*##^##
Designer {
    D{i:2;anchors_height:93;anchors_width:185}D{i:3;anchors_height:600;anchors_width:839;anchors_x:157;anchors_y:0}
}
##^##*/
