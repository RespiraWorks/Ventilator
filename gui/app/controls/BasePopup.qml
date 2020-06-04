import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.4

import ".."

Popup {
    id: root
    width: parent.width; height: parent.height
    parent: Overlay.overlay
    modal: true
    focus: true
    padding: 0
    margins: 0

    default property alias contentElement: loader.sourceComponent
    property alias title: titleText.text
    signal confirm()

    background: Rectangle {
        color: Style.theme.color.modalBackground
    }

    contentItem: Item {
        id: content
        anchors.fill: parent

        Image {
            id: logo
            width: 96; height: 96
            sourceSize: Qt.size(width, height)
            anchors {
                top: parent.top; topMargin: 24
                left: parent.left; leftMargin: 24
            }
            fillMode: Image.PreserveAspectFit
            source: 'qrc:/images/RW_adjust_24.svg'
        }

        Text {
            id: titleText
            anchors {
                left: logo.right; leftMargin: 32
                top: parent.top; topMargin: 37
            }

            color: Style.theme.color.textPrimary
            font: Style.theme.font.textHeaderTitle
        }

        Rectangle {
            id: headerLine
            anchors {
                top: parent.top; topMargin: 143
                left: parent.left;
            }

            width: parent.width; height: 1
            color: "#435360"
        }

        Loader {
            id: loader
            anchors {
                top: headerLine.bottom; bottomMargin: 8
                left: parent.left; leftMargin: 8
                right: parent.right; rightMargin: 8
                bottom: cancelButton.top; topMargin: 8
            }
        }

        PopupButton {
            id: cancelButton
            anchors {
                left: parent.left; leftMargin: 48
                bottom: parent.bottom; bottomMargin: 40
            }
            width: 144; height: 64
            indicator: Item {}

            text: qsTr("Cancel")
            onClicked: popup.close()
        }

        PopupButton {
            anchors {
                right: parent.right; rightMargin: 48
                bottom: parent.bottom; bottomMargin: 40
            }
            width: 144; height: 64
            indicator: Item{}

            text: qsTr("Confirm")
            onClicked: {
                popup.confirm()
                popup.close()
            }
        }
    }
}
