import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import Respira 1.0
import ".."

/*!
    \qmltype AlarmNotificationBanner
    \brief A banner that will be presented when alarm occurs.
*/
Item {
    id: root
    width: parent.width; height: 72

    property alias title: titleText.text
    property int numActiveAlarms: 0
    property int priority: AlarmPriority.NONE

    enabled: priority != AlarmPriority.NONE

    signal pauseAlarmClicked()

    state: switch(priority) {
        case AlarmPriority.NONE: "";   break;
        case AlarmPriority.LOW: "low"; break;
        case AlarmPriority.MEDIUM: "medium"; break;
        case AlarmPriority.HIGH: "high"; break;
    }


    y: enabled ? 0 : - (height*2)

    Behavior on y {
        SmoothedAnimation { velocity: 300 }
    }

    states: [
        State {
            name: "high"
            PropertyChanges {
                target: root;
                bgColor: Style.theme.color.alarmHighBright
                alarm_button_bg: "black"
                alarm_button_fg: "white"
                restoreEntryValues: false
            }
        },
        State {
            name: "medium"
            PropertyChanges {
                target: root;
                bgColor: Style.theme.color.alarmMediumBright
                alarm_button_bg: "white"
                alarm_button_fg: "black"
                restoreEntryValues: false
            }
        },
        State {
            name: "low"
            PropertyChanges {
                target: root;
                bgColor: Style.theme.color.alarmLowBright
                alarm_button_bg: "black"
                alarm_button_fg: "white"
                restoreEntryValues: false
            }
        }
    ]

    property color bgColor: "white"

    property color alarm_button_bg: "black"
    property color alarm_button_fg: "white"

    Rectangle {
        id: bg_rect
        color: root.bgColor
        anchors.fill: parent
        layer.enabled: true
        layer.effect: DropShadow {
            cached: true
            width:parent.width; height: parent.height
            horizontalOffset: 0
            verticalOffset: 2
            radius: 12
            samples: 25
            color: "#80000000"
            source: bg_rect
        }
    }

    Image {
        id: icon
        anchors {
            left: parent.left; leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        width: 40; height: 40
        sourceSize: Qt.size(width, height)
        source: 'qrc:/images/RW_warning_24.svg'
        layer.enabled: true
        layer.effect: ColorOverlay {
            anchors.fill: icon
            source: icon
            color: root.alarm_button_fg
        }
    }

    Text {
        id: titleText
        anchors {
            left: icon.right; leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        text: "Respiratory rate too low"
        color: root.alarm_button_fg
        font: Style.theme.font.textHeaderSubtitle
    }

    Text {
        id: titleCounterText
        anchors {
            left: titleText.right; leftMargin: 8
            verticalCenter: parent.verticalCenter
        }
        text: root.numActiveAlarms > 1 ? "+" + (root.numActiveAlarms - 1) : ""
        color: root.alarm_button_fg
        font: Style.theme.font.modalTitle
    }

    Button {
        id: button

        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right; rightMargin: 8
        }

        width: 210; height: 40

        background: Rectangle {
            anchors.fill: parent
            color: button.checked || button.down ?
                       alarm_button_bg :
                       alarm_button_fg
            radius: 8
        }

        onClicked: root.pauseAlarmClicked()

        contentItem: Row {

            spacing: 10
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left; leftMargin: 34
            }

            Image {
                id: alarm_icon
                anchors.verticalCenter: parent.verticalCenter
                width: 20; height: 20
                sourceSize: Qt.size(width, height)
                source: 'qrc:/images/RW_alarm-off_24.svg'

                layer.enabled: true
                layer.effect: ColorOverlay {
                    anchors.fill: alarm_icon
                    source: alarm_icon
                    color: button.checked || button.down ?
                               alarm_button_fg :
                               alarm_button_bg
                }
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Pause Alarm")
                font: Style.theme.font.modalTitle
                color: button.checked || button.down ?
                           alarm_button_fg :
                           alarm_button_bg
            }
        }
    }
}
