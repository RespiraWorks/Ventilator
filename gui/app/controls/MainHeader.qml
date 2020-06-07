import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import ".."

/*!
    \qmltype MainHeader
    \brief Top screen header containing menu, mode selection and alarm buttons.
*/
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
            sourceSize: Qt.size(32, 32)
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            source: 'qrc:/images/RW_menu_24.svg'
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
                    sourceSize: Qt.size(width, height)
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/RW_inspsenssettings_24.svg'
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
        id: alarmSettingsButton

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
                    sourceSize: Qt.size(width, height)
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/RW_alarm_24.svg'
                }
            }
        }
    }
}
