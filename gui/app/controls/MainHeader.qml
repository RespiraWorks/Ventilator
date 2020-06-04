import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import ".."

Control {

    width: 784; height: 64

    signal menuClicked()
    signal modeSelectionClicked()
    signal alarmSettingsClicked()

    HeaderButton {
        id: menuButton
        width: 56; height: 40
        anchors {
            verticalCenter: parent.verticalCenter
            leftMargin: 8
        }
        contentItem: Image {
            anchors.centerIn: parent
            source: 'qrc:/images/icon_menu.png'
        }

        onClicked: menuClicked()
    }

    HeaderButton {
        id: modeSelectionButton
        anchors {
            verticalCenter: parent.verticalCenter
            left: menuButton.right
            leftMargin: 8
        }
        width: 168; height: 40
        text: pageStack.currentMode.acronym
        onClicked: modeSelectionClicked()

        contentItem: Item {
            anchors.centerIn: parent

            Row {
                spacing: 8
                anchors.centerIn: parent

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 20; height: 20
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/Logo.png'
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: modeSelectionButton.text
                    font: Style.theme.font.headerButton
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }
    }

    HeaderButton {
        id: alertButton

        width: 104; height: 40

        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
        text: pageStack.currentMode.acronym

        onClicked: alarmSettingsClicked()

        contentItem: Item {
            anchors.centerIn: parent

            Row {
                spacing: 10
                anchors.centerIn: parent

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 16; height: 27
                    text: "0"
                    font: Style.theme.font.headerButton
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 20; height: 20
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/Logo.png'
                }
            }
        }
    }
}
