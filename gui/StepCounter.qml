import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import "."

/*!
    \qmltype StepCounter
    \brief A visual control for modifying values by steps.

    StepCounter allows the user to choose a number value by clicking the plus or minus indicator buttons.

    The incremental value is defined by the stepSize and the range can be defined
    in the properties maxValue and  minValue

    Example:
    \qml
        StepCounter {
            title: qsTr("My float counter")
            stepSize: 0.1
            value: 12.0
            onValueModified: console.log("new value: " + value)

            textFromValue: function (value, locale) {
                // display only one decimal
                return Number(value).toFixed(1)
            }
        }
    \endqml
*/
Control {
    id: root

    property alias title: title.text

    // define how much the stepper should inc/dec
    property real stepSize: 1

    property real value: 0

    property real maxValue: 65356
    property real minValue: 0

    property var textFromValue: function (value, locale) {
        return Math.round(value)
    }

    signal valueModified

    implicitWidth: 150
    implicitHeight: 100

    padding: Style.margin.normal

    background: Rectangle {
        color: Style.theme.color.primary
        radius: 10
    }

    contentItem: Item {
        Text {
            id: title

            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            font.pixelSize: 17
            font.bold: true
        }

        Row {
            id: row

            anchors {
                topMargin: Style.margin.normal
                top: title.bottom
                horizontalCenter: parent.horizontalCenter
            }

            RoundButton {
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("-")
                enabled: root.value > root.minValue
                checkable: false
                focusPolicy: Qt.NoFocus
                font.bold: true
                onClicked: {
                    root.value -= stepSize
                    valueModified()
                }
            }

            Text {
                width: 60
                height: 44
                anchors.verticalCenter: parent.verticalCenter
                text: textFromValue(value)
                horizontalAlignment: Text.AlignHCenter
                font: Style.stepButtonFont
            }

            RoundButton {
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("+")
                enabled: root.value < root.maxValue
                checkable: false
                focusPolicy: Qt.NoFocus
                font.bold: true
                onClicked: {
                    root.value += stepSize
                    valueModified()
                }
            }
        }
    }
}
