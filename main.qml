import QtQuick 2.0
import QtQuick.Layouts 1.3

Item
{
    id: mainWindow
    width: 1024
    height: 600

    ControlPanel {

        id: controlPanel
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10

//        onRefreshRateChanged:
//        {
//            pressureView.changeRefreshRate(rate);
//            tidalVolumeView.changeRefreshRate(rate);
//        }
//        onOpenGlChanged:
//        {
//            pressureView.openGL = enabled;
//            tidalVolumeView.openGL = enabled;
//        }
    }
    ScopeView {
        id: pressureView
        name: "Pressure [mmH2O]"
        color: "#4f67ff"
        anchors.bottomMargin: 301
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: controlPanel.right

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
        height: 200
        anchors.top: pressureView.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: controlPanel.right
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 1

        onOpenGLSupportedChanged:
        {
            if (!openGLSupported)
            {
                controlPanel.openGLButton.enabled = false
                controlPanel.openGLButton.currentSelection = 0
            }
        }
    }
    Timer
    {
        id: refreshTimer
        interval: 1 / 15 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            volumeDataSource.update(tidalVolumeView.series(0));
            pressureDataSource.update(pressureView.series(0));
        }
    }
}
