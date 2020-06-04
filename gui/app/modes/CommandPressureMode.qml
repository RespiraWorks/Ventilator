import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "../controls"
import ".."

Mode {
    Timer // TODO: Make data sources be updated as we get data
    {
        id: refreshTimer
        interval: 1 / 15 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            // TODO: guiState as a contextObject is not great
            // as you lose autocomplete and you might infect
            // a lot of files. Context objects will also be
            // removed on Qt6. So better use singleton instead
            guiState.update(
                        pressureView.series(0),
                        flowView.series(0),
                        tidalVolumeView.series(0));
        }
    }

    Rectangle {
        id: parameterDisplayPanel
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
                parameterName: qsTr("PiP")
                parameterUnit: qsTr("cmH<sub>2</sub>O")
                parameterValue: guiState.pip.toString()
            }

            ParameterDisplay {
                parameterName: qsTr("PEEP")
                parameterUnit: qsTr("cmH<sub>2</sub>O")
                parameterValue: guiState.peep.toString()
            }

            ParameterDisplay {
                parameterName: qsTr("Flow")
                parameterUnit: qsTr("l/min")
                parameterValue: guiState.flowReadout.toFixed(0);
            }

            ParameterDisplay {
                parameterName: qsTr("RR")
                parameterUnit: qsTr("b/min")
                parameterValue: guiState.rr.toString()
            }

            ParameterDisplay {
                parameterName: qsTr("I:E")
                parameterUnit: qsTr("ratio")
                parameterValue: guiState.ier.toFixed(1).toString()
            }

            ParameterDisplay {
                // empty according to the design
            }

            ParameterDisplay {
                parameterName: qsTr("TV")
                parameterUnit: qsTr("ml")
                parameterValue: guiState.tvReadout.toFixed(0);
            }

            ParameterDisplay {
                parameterName: qsTr("FiO<sub>2</sub>")
                parameterUnit: qsTr("%")
                parameterValue: "15"
            }
        }
    }

    Rectangle {
        width: 784; height: 440
        radius: 8
        anchors {
            top: parent.top
            left: parent.left; leftMargin: 8
            right: parameterDisplayPanel.left; rightMargin: 8
        }

        color: "#05121C"

        ColumnLayout {

            id: scopeGridLayout
            anchors.fill: parent

            ScopeView {
                id: pressureView
                name: "Pressure [cmH2O]"
                // TODO: Are these reasonable lower and upper bounds?
                // Source for current value:
                // https://www.rtmagazine.com/public-health/pediatrics/neonatal/selecting-appropriate-ventilator-parameters/
                // mentions values in the range 5-30 cmH2O.
                yMin: -3
                yMax: 30

                color: "white"
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ScopeView
            {
                id: flowView
                name: "Flow [mL/min]"
                // TODO: Are these reasonable lower and upper bounds?
                // Source for current value:
                // https://www.sciencedirect.com/topics/medicine-and-dentistry/peak-inspiratory-flow
                // "Most modern ventilators can deliver flow rates between
                // 60 and 120 L/min. "
                yMin: -150
                yMax: 150

                color: "white"
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ScopeView
            {
                id: tidalVolumeView
                name: "Tidal Volume [mL]"
                // TODO: Are these reasonable lower and upper bounds?
                // Source for current value:
                // https://en.wikipedia.org/wiki/Tidal_volume
                // "In a healthy, young human adult, tidal volume is
                // approximately 500 mL per inspiration or 7 mL/kg of body mass."
                // Meaning, 2000 should be enough for a human of ~300kg body mass;
                // I don't know whether heavier humans have even larger
                // tidal volume.
                yMin: 0
                yMax: 2000

                color: "white"
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

        }

    }


    Row {
        id: parameterButtonsPanel
        spacing: 8

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom; bottomMargin: 8
        }

        Layout.alignment: Qt.AlignHCenter

        ParameterButton {
            parameterName: qsTr("Pip")
            parameterUnit: qsTr("cmH<sub>2</sub>0")
            parameterValue: guiState.pip.toString()
        }

        ParameterButton {
            parameterName: qsTr("Peep")
            parameterUnit: qsTr("cmH<sub>2</sub>0")
            parameterValue: guiState.peep.toString()
        }

        ParameterButton {
            parameterName: qsTr("FiO<sub>2</sub>")
            parameterUnit: "%"
            parameterValue: guiState.pip.toString()
        }

        ParameterButton {
            parameterName: qsTr("I-time")
            parameterUnit: "sec"
            parameterValue: Number(guiState.ier).toFixed(1)
        }

        ParameterButton {
            parameterName: qsTr("RR")
            parameterUnit: "b/min"
            parameterValue: guiState.rr.toString()
        }
    }
}
