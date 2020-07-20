import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import Respira 1.0
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
            source: GuiStateContainer.is_using_fake_data ? 'qrc:/images/RW_debug.svg' : 'qrc:/images/RW_menu_24.svg'
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
                    color: Style.theme.color.textPrimary
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }
    }

    AlarmButton {
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
        priority: {
          var alarm = GuiStateContainer.alarmManager.highestPrioritySilencedAlarm
          return (alarm == null) ? AlarmPriority.NONE : alarm.nominalPriority
        }
        numAlarms: GuiStateContainer.alarmManager.numSilencedAlarms
        remainingSilenceMs: {
          var alarm = GuiStateContainer.alarmManager.highestPrioritySilencedAlarm
          return (alarm == null) ? 0 : alarm.remainingSilenceMs
        }
        onClicked: console.log("Alarm Button clicked")
    }
}
