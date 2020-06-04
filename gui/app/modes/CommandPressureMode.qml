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

    ColumnLayout {
        id: columnLayout
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            bottom: parent.bottom
            margins: 8
        }

        RowLayout {
            id: rowLayout
            width: 784
            height: 440

            GridLayout {
                id: scopeGridLayout
                columnSpacing: 0
                rowSpacing: -20
                Layout.minimumHeight: 400
                Layout.minimumWidth: 500
                Layout.fillHeight: true
                Layout.fillWidth: true
                flow: GridLayout.TopToBottom
                rows: 3

                ScopeView {
                    id: pressureView
                    name: "Pressure [cmH2O]"
                    // TODO: Are these reasonable lower and upper bounds?
                    // Source for current value:
                    // https://www.rtmagazine.com/public-health/pediatrics/neonatal/selecting-appropriate-ventilator-parameters/
                    // mentions values in the range 5-30 cmH2O.
                    yMin: -3
                    yMax: 30

                    color: "#4f67ff"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 10
                    height: parent.height/3
                    width: parent.width
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

                    color: "green"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 10
                    height: parent.height/3
                    width: parent.width
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

                    color: "#ffff00"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 10
                    height: parent.height/3
                    width: parent.width
                }

            }

            ColumnLayout {
                id: sensorReadouts
                width: 100
                height: 100
                Layout.maximumHeight: 65356
                Layout.maximumWidth: 6553
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 200

                Rectangle {
                    id: pressureReadout
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10

                    Text {
                        id: pressureVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: Number(guiState.pressureReadout, 'g', 1).toFixed(1);
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Times New Roman"
                        font.weight: Font.DemiBold
                        font.bold: true
                        font.pixelSize: 38
                    }

                    Text {
                        x: 3
                        y: 13
                        text: qsTr("Pressure (cm H2O)")
                        font.bold: true
                        font.pixelSize: 17
                    }
                    Layout.minimumWidth: 150
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: flowReadout
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10

                    Text {
                        id: flowVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: Number(guiState.flowReadout, 'g', 1).toFixed(1);
                        horizontalAlignment: Text.AlignHCenter
                        font.weight: Font.DemiBold
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                        font.bold: true
                    }

                    Text {
                        x: 23
                        y: 13
                        text: qsTr("Flow (mL/min)")
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                        font.bold: true
                    }

                    Layout.minimumWidth: 150
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: tvReadout
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10

                    Text {
                        id: tvVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text:  Number(guiState.tvReadout, 'g', 1).toFixed(1);
                        horizontalAlignment: Text.AlignHCenter
                        font.weight: Font.DemiBold
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                        font.bold: true
                    }

                    Text {
                        x: 41
                        y: 13
                        text: qsTr("TV (mL)")
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                        font.bold: true
                    }

                    Layout.minimumWidth: 150
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }
            }
        }

        Row {
            spacing: 8

            Layout.alignment: Qt.AlignHCenter

            ParameterButton {
                parameterName: qsTr("Pip")
                parameterNotation: qsTr("cmH<sub>2</sub>0")
                parameterValue: guiState.pip.toString()
            }

            ParameterButton {
                parameterName: qsTr("Peep")
                parameterNotation: qsTr("cmH<sub>2</sub>0")
                parameterValue: guiState.peep.toString()
            }

            ParameterButton {
                parameterName: qsTr("FiO<sub>2</sub>")
                parameterNotation: "%"
                parameterValue: guiState.pip.toString()
            }

            ParameterButton {
                parameterName: qsTr("I-time")
                parameterNotation: "sec"
                parameterValue: Number(guiState.ier).toFixed(1)
            }

            ParameterButton {
                parameterName: qsTr("RR")
                parameterNotation: "b/min"
                parameterValue: guiState.rr.toString()
            }
        }
    }
}
