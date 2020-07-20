import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

Rectangle {
    id: control
    width: 104; height: 104

    color: Style.theme.color.parameterBackgroundByPriority(alarmPriority)
    antialiasing: true
    radius: 8

    property int alarmPriority: AlarmPriority.NONE

    // define parameter name, e.g. PIP
    property alias parameterName: parameterNameText.text

    // define parameter notation, e.g cmH<sub>2</sub>O
    property alias parameterUnit: parameterUnitText.text

    // define current value as a string
    property alias parameterValue: parameterValueText.text

    Row {
        id: parameterRow
        spacing: 2
        anchors {
            top: parent.top; topMargin: 7
            left: parent.left; leftMargin: 8
            right: parent.right; rightMargin: 4
        }

        Text {
            id: parameterNameText
            textFormat: Text.RichText
            font: Style.theme.font.parameterDisplay
            color: Style.theme.color.textPrimary
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Text {
            id: parameterUnitText
            textFormat: Text.RichText
            color: Style.theme.color.textAlternative
            font: Style.theme.font.parameterDisplay
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

    Text {
        id: parameterValueText

        anchors {
            top: parameterRow.bottom;
            horizontalCenter: parent.horizontalCenter
        }
        textFormat: Text.AutoText
        color: Style.theme.color.textPrimary
        font: Style.theme.font.parameterDisplayValue
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
