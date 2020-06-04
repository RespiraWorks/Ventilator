import QtQuick 2.0
import "../controls"

// TODO: Implement mode
Mode {
    Text {
        anchors.centerIn: parent
        text: "High Flow Nasal Cannula Mode"
        color: "white"
        font.pointSize: 38
    }

    Row {

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom; bottomMargin: 8
        }

        spacing: 8

        ParameterButton {
            parameterName: qsTr("Flow")
            parameterNotation: qsTr("l/min")
            parameterValue: "60"
        }

        ParameterButton {
            parameterName: qsTr("FiO<sub>2</sub>")
            parameterNotation: "%"
            parameterValue: "40"
        }
    }
}
