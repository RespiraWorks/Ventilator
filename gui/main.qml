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
            volumeDataSource.update(tidalVolumeView.series(0));
            pressureDataSource.update(pressureView.series(0));
            flowDataSource.update(flowView.series(0));

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
            name: "Flow [mL]"
            color: "green"
            Layout.fillHeight: true
            Layout.fillWidth: true
            height: parent.height/3
            width: parent.width

            onOpenGLSupportedChanged:
            {
                if (!openGLSupported)
                {
                    controlPanel.openGLButton.enabled = false
                    controlPanel.openGLButton.currentSelection = 0
                }
            }
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

            onOpenGLSupportedChanged:
            {
                if (!openGLSupported)
                {
                    controlPanel.openGLButton.enabled = false
                    controlPanel.openGLButton.currentSelection = 0
                }
            }
        }

        ScopeView {
            id: pressureView
            name: "Pressure [mmH2O]"
            color: "#4f67ff"
            Layout.fillHeight: true
            Layout.fillWidth: true
            height: parent.height/3
            width: parent.width

            onOpenGLSupportedChanged:
            {
                if (!openGLSupported)
                {
                    controlPanel.openGLButton.enabled = false
                    controlPanel.openGLButton.currentSelection = 0
                }
            }
        }



    }

}

/*##^##
Designer {
    D{i:2;anchors_height:93;anchors_width:185}D{i:3;anchors_height:600;anchors_width:839;anchors_x:157;anchors_y:0}
}
##^##*/
