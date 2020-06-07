import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import ".."
import "../controls"

// TODO: Implement mode
Mode {
    Text {
        anchors.centerIn: parent
        text: "High Flow Nasal Cannula Mode"
        color: "white"
        font.pixelSize: 38
    }

    Rectangle {

        width: 216; height: 440
        color: "#05121C"
        radius: 8
        anchors {
            top: parent.top;
            right: parent.right; rightMargin: 8
        }

        GridLayout {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            columns: 2
            columnSpacing: 0
            rowSpacing: 0

            ParameterDisplay {
                parameterName: qsTr("PIP")
                parameterUnit: qsTr("cmH<sub>2</sub>O")
                parameterValue: "23"
            }

            ParameterDisplay {
                parameterName: qsTr("PEEP")
                parameterUnit: qsTr("cmH<sub>2</sub>O")
                parameterValue: "4"
            }

            ParameterDisplay {
                parameterName: qsTr("Flow")
                parameterUnit: qsTr("L/min")
                parameterValue: "58"
            }

            ParameterDisplay {
                parameterName: qsTr("RR")
                parameterUnit: qsTr("b/min")
                parameterValue: "15"
            }

            ParameterDisplay {
                parameterName: qsTr("VT")
                parameterUnit: qsTr("mL")
                parameterValue: "385"
            }

            ParameterDisplay {
                parameterName: qsTr("FiO<sub>2</sub>")
                parameterUnit: qsTr("%")
                parameterValue: "15"
            }
        }
    }

    Row {

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom; bottomMargin: 8
        }

        spacing: 8

        ParameterButton {
            parameterName: qsTr("Flow")
            parameterUnit: qsTr("L/min")
            parameterValue: 60
        }

        ParameterButton {
            parameterName: qsTr("FiO<sub>2</sub>")
            parameterUnit: "%"
            parameterValue: 40
        }
    }
}
