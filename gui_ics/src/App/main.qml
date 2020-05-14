import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: window

    visible: true

    property double desiredWidth: 1024
    property double desiredHeight: 600

    width: desiredWidth
    height: desiredHeight

    title: qsTr("GreenHouse Application")

    Item {
        id: root
        anchors.centerIn: parent

        width: window.desiredWidth
        height: window.desiredHeight

        scale: Math.min(window.width / width, window.height / height)
    }
    Component.onCompleted: Integration.mainWindowCreated(root, window)
}
