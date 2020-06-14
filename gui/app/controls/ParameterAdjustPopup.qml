import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.4

import ".."

/*!
    \qmltype ParameterAdjustPopup
    \brief A popup control that allows user to change parameter values
*/
BasePopup {
    id: root

    // informational text about value displayed
    property string parameterInfoText

    // unit text about value displayed
    property string parameterUnit

    // current value for the paramater
    property real value: 0

    // minimum value for the parameter
    property real minValue: 0

    // maximum value for the parameter
    property real maxValue: 512

    // step value used for incrementing or decrementing value
    property real stepSize: 1

    // formatter function used to display parameter value
    property var displayFormatter: function(value) {
        return value.toString()
    }

    signal valueConfirmed

    onConfirm: root.valueConfirmed()

    Item {

        Text {
            id: parameterInfo
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top; topMargin: 46
            }
            text: root.parameterInfoText
            font: Style.theme.font.textHeaderSubtitle
            color: Style.theme.color.textPrimary
        }

        BaseButton {
            id: decreaseButton
            anchors {
                left: parent.left; leftMargin: 176
                top: parent.top; topMargin: 168
            }

            contentItem: Image {
                sourceSize: Qt.size(36, 36)
                fillMode: Image.PreserveAspectFit
                source: 'qrc:/images/RW_substract_24.svg'
            }
            visible: root.value > root.minValue
            onClicked: {
                root.value -= root.stepSize
            }
        }

        BaseButton {
            id: increaseButton
            anchors {
                right: parent.right; rightMargin: 176
                top: parent.top; topMargin: 168
            }
            contentItem: Image {
                antialiasing: true
                sourceSize: Qt.size(36, 36)
                fillMode: Image.PreserveAspectFit
                source: 'qrc:/images/RW_add_24.svg'
            }
            visible: root.value < root.maxValue
            onClicked: {
                root.value += root.stepSize
            }
        }

        Text {
            id: valueText
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parameterInfo.bottom; topMargin: 17
            }

            text: root.displayFormatter(root.value)
            font: Style.theme.font.parameterPopupDisplayValue
            color: Style.theme.color.textPrimary
        }

        Text {
            id: unitText
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: valueText.bottom; topMargin: 1
            }
            text: root.parameterUnit
            font: Style.theme.font.textHeaderTitle
            textFormat: Text.RichText
            color: Style.theme.color.textAlternative
        }
    }
}
