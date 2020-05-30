import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Item
{
    id: mainWindow
    width: 1024
    height: 600

    Timer // TODO: Make data sources be updated as we get data
    {
        id: refreshTimer
        interval: 1 / 15 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            guiState.update(
                pressureView.series(0),
                flowView.series(0),
                tidalVolumeView.series(0));
        }
    }

    Rectangle {
        id: rectangle
        color: "#e3e8ee"
        z: -1
        anchors.fill: parent
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent



        Item {
            id: rowLayout1
            width: parent.width
            height: 100
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            FpsItem {}

            Image {
                id: image
                anchors.horizontalCenter: parent.horizontalCenter
                width: 150
                height: 50
                clip: false
                source: "images/respiraWorksLogoHorizontalTransparent.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        RowLayout {
            id: rowLayout
            width: 100
            height: 100

            ColumnLayout {
                id: columnLayout1
                width: 100
                height: 100
                Layout.maximumHeight: 65356
                Layout.maximumWidth: 6553
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 200

                StepCounter {
                    id: rrStepCounter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.topMargin: 10
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10

                    title: qsTr("RR")
                    value: guiState.rr
                    onValueModified: guiState.rr = value
                }

                StepCounter {
                    id: peepStepCounter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.topMargin: 10
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10

                    title: qsTr("PEEP")
                    value: guiState.peep
                    onValueModified: guiState.peep = value
                }

                StepCounter {
                    id: pipStepCounter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.topMargin: 10
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10

                    title: qsTr("PIP")
                    value: guiState.pip
                    onValueModified: guiState.pip = value
                }

                StepCounter {
                    id: ierStepCounter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.topMargin: 10
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10

                    title: qsTr("I:E")
                    stepSize: 0.1
                    value: guiState.ier
                    onValueModified: guiState.ier = value

                    textFromValue: function (value, locale) {
                        return Number(value).toFixed(1)
                    }
                }
            }

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

        RowLayout {
            id: rowLayout2
            width: 100
            height: 50
            Layout.minimumHeight: 50
            Layout.maximumHeight: 65535
        }
    }
}

/*##^##
Designer {
    D{i:2;anchors_height:93;anchors_width:185}D{i:3;anchors_height:600;anchors_width:839;anchors_x:157;anchors_y:0}
}
##^##*/
/*##^## Designer {
    D{i:2;anchors_height:200;anchors_width:200}
}
 ##^##*/
