import QtQuick 2.11
import QtQuick.Window 2.2
import ".."

/*!
    \qmltype FPSItem
    \brief A FPS counter for QML scene.

    A FPSItem is small debug item that will display current
    refresh rate of the QML scene.

    It keeps an image in a rotation animation and
    calculates how many updates happens on a given interval (default 2s).

*/
Item {
    id: root

    property bool enabled: true

    property int frameCounter: 0
    property int frameCounterAvg: 0
    property int counter: 0
    property int fps: 0
    property int fpsAvg: 0

    visible: enabled

    implicitWidth: fpsText.width + 10 + spinnerImage.width
    implicitHeight: 30

    Row {

        spacing: 10

        // Image that will be rotate for FPS calculation
        Image {
            id: spinnerImage
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/images/Logo.png"
            width: 24; height: 24
            opacity: 0
            NumberAnimation on rotation {
                from:0
                to: 360
                running: root.visible
                duration: 800
                loops: Animation.Infinite
            }
            onRotationChanged: frameCounter++;
        }

        // Display FPS count as Text
        Text {
            id: fpsText
            font.pointSize: 18
            color: "white"
            text: "Ø " + root.fpsAvg + " | " + root.fps + " fps"
        }
    }

    Timer {
        interval: 2000
        repeat: true
        running: root.visible
        onTriggered: {
            frameCounterAvg += frameCounter;
            root.fps = frameCounter/2;
            counter++;
            frameCounter = 0;
            if (counter >= 3) {
                root.fpsAvg = frameCounterAvg/(2*counter)
                frameCounterAvg = 0;
                counter = 0;
            }
        }
    }
}
