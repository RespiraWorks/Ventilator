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
            volumeDataSource.update(tidalVolumeView.series(0));
            pressureDataSource.update(pressureView.series(0));
            flowDataSource.update(flowView.series(0));

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



        RowLayout {
            id: rowLayout1
            width: 100
            height: 100
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Image {
                id: image
                width: 150
                height: 50
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.maximumWidth: 200
                clip: false
                Layout.maximumHeight: 50
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumHeight: 50
                Layout.topMargin: 5
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

                Rectangle {
                    id: breathsPerMinute
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.topMargin: 10
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10

                    Text {
                        id: breathsPerMinuteVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: qsTr("0")
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Times New Roman"
                        font.weight: Font.DemiBold
                        font.bold: true
                        font.pixelSize: 38
                    }

                    Text {
                        id: element
                        x: 63
                        y: 13
                        text: qsTr("RR")
                        font.bold: true
                        font.pixelSize: 17
                    }

                    RoundButton {
                        id: breathsPerMinuteInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        font.family: "Courier"
                        font.bold: true
                        focusPolicy: Qt.ClickFocus
                    }

                    RoundButton {
                        id: breathsPerMinuteDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        focusPolicy: Qt.NoFocus
                        font.bold: true
                    }

                }

                Rectangle {
                    id: peep
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10

                    Text {
                        id: peepVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: qsTr("0")
                        horizontalAlignment: Text.AlignHCenter
                        font.weight: Font.DemiBold
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                        font.bold: true
                    }

                    Text {
                        id: element3
                        x: 53
                        y: 13
                        text: qsTr("PEEP")
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                        font.bold: true
                    }

                    RoundButton {
                        id: peepInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        checkable: false
                        focusPolicy: Qt.NoFocus
                        font.bold: true
                    }

                    RoundButton {
                        id: peepDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus
                    }
                    Layout.minimumWidth: 150
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: pip
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10

                    Text {
                        id: pipVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: qsTr("0")
                        horizontalAlignment: Text.AlignHCenter
                        font.weight: Font.DemiBold
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                        font.bold: true
                    }

                    Text {
                        id: element5
                        x: 61
                        y: 13
                        text: qsTr("PIP")
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                        font.bold: true
                    }

                    RoundButton {
                        id: pipInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus
                    }

                    RoundButton {
                        id: pipDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        focusPolicy: Qt.NoFocus
                        font.bold: true
                    }
                    Layout.minimumWidth: 150
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }

            }

            GridLayout {
                id: scopeGridLayout
                columnSpacing: 0
                rowSpacing: -20
           /*     anchors.left: parent.left
                anchors.leftMargin: 195
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0 */
                Layout.minimumHeight: 400
                Layout.minimumWidth: 500
                Layout.fillHeight: true
                Layout.fillWidth: true
                flow: GridLayout.TopToBottom
                rows: 3

                ScopeView
                {
                    id: flowView
                    name: "Flow [mL]"
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
                    color: "#ffff00"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 10
                    height: parent.height/3
                    width: parent.width
                }

                ScopeView {
                    id: pressureView
                    name: "Pressure [mmH2O]"
                    color: "#4f67ff"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 10
                    height: parent.height/3
                    width: parent.width
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
