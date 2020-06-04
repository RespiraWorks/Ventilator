import QtQuick 2.0

import QtQuick 2.11
import QtQuick.Controls 2.4
import ".."

/*!
    \qmltype ParameterButton
    \brief A Parameter Button Element

    It displays parameter name, notation and current value.
    Once clicked, can be used to select a new parameter value.
*/
Button {
    id: control

    implicitWidth:192; implicitHeight: 80

    // define parameter name, e.g. PiP
    property alias parameterName: parameterNameText.text

    // define parameter notation, e.g cmH<sub>2</sub>O
    property alias parameterNotation: parameterNotationText.text

    // define current value as a string
    property alias parameterValue: parameterValueText.text

    background: Rectangle {
        antialiasing: true
        color: control.down ? Style.theme.color.headerButtonHighlighted :
                              Style.theme.color.headerButton
        radius: 8
    }

    contentItem: Item {
        anchors.fill: parent

        Text {
            id: parameterNameText
            anchors {
                top: parent.top; topMargin: 11
                left: parent.left; leftMargin: 8
            }
            textFormat: Text.AutoText
            font: Style.theme.font.parameterDisplay
            color: "white"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Text {
            id: parameterNotationText
            anchors {
                top: parameterNameText.bottom; topMargin: 1
                left: parent.left; leftMargin: 8
            }
            textFormat: Text.AutoText
            color: "#AFAFAF"
            font: Style.theme.font.parameterDisplayNotation
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Text {
            id: parameterValueText
            width: 104; height: 70
            anchors {
                top: parent.top; topMargin: 1
                right: parent.right; rightMargin: 8
            }
            textFormat: Text.AutoText
            color: "white"
            font: Style.theme.font.parameterDisplayValue
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
