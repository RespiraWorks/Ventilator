import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import Respira 1.0
import "../controls"
import ".."

Mode {
    id: mode
    Timer // TODO: Make data sources be updated as we get data
    {
        id: refreshTimer
        interval: 1 / 60 * 1000 // 60 Hz
        running: mode.visible
        repeat: true
        onTriggered: {
            GuiStateContainer.update();
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
            columnSpacing: 8
            rowSpacing: 8

            ParameterDisplay {
                parameterName: qsTr("PIP")
                parameterUnit: qsTr("cmH<sub>2</sub>O")
                parameterValue: GuiStateContainer.pip.toString()
            }

            ParameterDisplay {
                parameterName: qsTr("PEEP")
                parameterUnit: qsTr("cmH<sub>2</sub>O")
                parameterValue: GuiStateContainer.peep.toString()
            }

            ParameterDisplay {
                parameterName: qsTr("Flow")
                parameterUnit: qsTr("L/min")
                parameterValue: GuiStateContainer.flowReadout.toFixed(0);
            }

            ParameterDisplay {
                parameterName: qsTr("RR")
                parameterUnit: qsTr("b/min")
                parameterValue: GuiStateContainer.rr.toString()
            }

            ParameterDisplay {
                parameterName: qsTr("I:E")
                parameterUnit: qsTr("ratio")
                parameterValue: GuiStateContainer.ier.toFixed(1).toString()
            }

            ParameterDisplay {
                // empty according to the design
            }

            ParameterDisplay {
                parameterName: qsTr("TV")
                parameterUnit: qsTr("mL")
                parameterValue: GuiStateContainer.tvReadout.toFixed(0);
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
            bottom: parameterButtonsPanel.top; bottomMargin: 8
        }

        color: "#05121C"

        ColumnLayout {

            id: scopeGridLayout
            anchors.fill: parent
            spacing: 0

            ScopeView {
                id: pressureView
                name: "Pressure"
                unit: "cmH<sub>2</sub>O"
                // TODO: Are these reasonable lower and upper bounds?
                // Source for current value:
                // https://www.rtmagazine.com/public-health/pediatrics/neonatal/selecting-appropriate-ventilator-parameters/
                // mentions values in the range 5-30 cmH2O.
                dataset: GuiStateContainer.pressureSeries
                yMin: -3
                yMax: 30
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ScopeView
            {
                id: flowView
                name: "Flow"
                unit: "mL/min"
                // TODO: Are these reasonable lower and upper bounds?
                // Source for current value:
                // https://www.sciencedirect.com/topics/medicine-and-dentistry/peak-inspiratory-flow
                // "Most modern ventilators can deliver flow rates between
                // 60 and 120 L/min. "
                yMin: -150
                yMax: 150
                dataset: GuiStateContainer.flowSeries
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ScopeView
            {
                id: tidalVolumeView
                name: "Tidal Volume"
                unit: "mL"
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
                showBottomLine: false
                Layout.fillHeight: true
                Layout.fillWidth: true
                dataset: GuiStateContainer.tidalSeries
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
            parameterName: qsTr("PIP")
            parameterUnit: qsTr("cmH<sub>2</sub>0")
            parameterValue: GuiStateContainer.pip
            onValueConfirmed: GuiStateContainer.pip = value
        }

        ParameterButton {
            parameterName: qsTr("Peep")
            parameterUnit: qsTr("cmH<sub>2</sub>0")
            parameterMaxValue: 20
            parameterMinValue: 0
            parameterStepSize: 2
            parameterValue: GuiStateContainer.peep
            onValueConfirmed: GuiStateContainer.peep = value
        }

        ParameterButton {
            parameterName: qsTr("FiO<sub>2</sub>")
            parameterUnit: "%"
            parameterMaxValue: 100
            parameterMinValue: 21
            parameterStepSize: 0.5
            parameterValue: 10
            onValueConfirmed: parameterValue = value
        }

        ParameterButton {
            parameterName: qsTr("I-time")
            parameterUnit: "sec"
            parameterValue: Number(GuiStateContainer.ier).toFixed(1)
            parameterMaxValue: 1
            parameterMinValue: 0
            parameterStepSize: 0.05
            parameterDisplayFormatter: function (value) {
                return Number(value).toFixed(1)
            }
        }

        ParameterButton {
            parameterName: qsTr("RR")
            parameterUnit: "b/min"
            parameterMaxValue: 30
            parameterMinValue: 10
            parameterStepSize: 1
            parameterValue: GuiStateContainer.rr
            onValueConfirmed: GuiStateContainer.rr = value
        }
    }
}
