import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import com.myself 1.0

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

                    MyObject {
                       id: myobject
                    }

                    Text {
                        id: breathsPerMinuteVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: Number(myobject.rr_default());
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

                        onClicked: {
                            myobject.rr_inc();
                            breathsPerMinuteVal.text =  Number(myobject.rr_val());
                            }
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

                        onClicked: {
                            myobject.rr_dec();
                            breathsPerMinuteVal.text =  Number(myobject.rr_val());
                            }
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
                        text:  Number(myobject.peep_default());
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

                        onClicked: {
                            myobject.peep_inc();
                            peepVal.text =  Number(myobject.peep_val());
                        }
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

                        onClicked: {
                            myobject.peep_dec();
                            peepVal.text =  Number(myobject.peep_val());
                        }
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
                        text:  Number(myobject.pip_default());
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

                        onClicked: {
                            myobject.pip_inc();
                            pipVal.text =  Number(myobject.pip_val());
                        }
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

                        onClicked: {
                            myobject.pip_dec();
                            pipVal.text =  Number(myobject.pip_val());
                        }
                    }
                    Layout.minimumWidth: 150
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: ier
                    color: "#466eeb"
                    radius: 10
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10

                    Text {
                        id: ierVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text:  Number(myobject.ier_default());
                        horizontalAlignment: Text.AlignHCenter
                        font.weight: Font.DemiBold
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                        font.bold: true
                    }

                    Text {
                        id: element53
                        x: 61
                        y: 13
                        text: qsTr("I:E")
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                        font.bold: true
                    }

                    RoundButton {
                        id: ierInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.ier_inc();
                            ierVal.text =  Number(myobject.ier_val(), 'g', 1).toFixed(1);
                            //console.log("Increase :" + ierVal.text);
                        }
                    }

                    RoundButton {
                        id: ierDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        focusPolicy: Qt.NoFocus
                        font.bold: true

                        onClicked: {
                            myobject.ier_dec();
                            ierVal.text =  Number(myobject.ier_val(), 'g', 1).toFixed(1);
                        }
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

            ColumnLayout {
                id: columnLayout2
                width: 100
                height: 100

                Text {
                    id: element2
                    text: qsTr("Alarms")
                    Layout.fillWidth: true
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                }


                Rectangle {
                    id: rrAlarmHigh
                    color: "#466eeb"
                    radius: 10
                    MyObject {
                        id: myobject1
                    }

                    Text {
                        id: rrAlarmHighVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text:  Number(myobject.rrAlarmHigh_default());
                        font.weight: Font.DemiBold
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                    }

                    Text {
                        id: element1
                        x: 45
                        y: 13
                        text: qsTr("RR High")
                        font.bold: true
                        font.pixelSize: 17
                    }

                    RoundButton {
                        id: rrAlarmHighInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        font.bold: true
                        focusPolicy: Qt.ClickFocus
                        font.family: "Courier"

                        onClicked: {
                            myobject.rrAlarmHigh_inc();
                            rrAlarmHighVal.text =  Number(myobject.rrAlarmHigh_val());
                            }
                    }

                    RoundButton {
                        id: rrAlarmHighDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.rrAlarmHigh_dec();
                            rrAlarmHighVal.text =  Number(myobject.rrAlarmHigh_val());
                            }
                    }
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: rrAlarmLow
                    color: "#466eeb"
                    radius: 10
                    Text {
                        id: rrAlarmLowVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: Number(myobject.rrAlarmLow_default());
                        font.weight: Font.DemiBold
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                    }

                    Text {
                        id: element4
                        x: 45
                        y: 13
                        text: qsTr("RR Low")
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                    }

                    RoundButton {
                        id: rrAlarmLowInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.rrAlarmLow_inc();
                            rrAlarmLowVal.text =  Number(myobject.rrAlarmLow_val());
                            }
                    }

                    RoundButton {
                        id: rrAlarmLowDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.rrAlarmLow_dec();
                            rrAlarmLowVal.text =  Number(myobject.rrAlarmLow_val());
                            }
                    }
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: tvAlarmHigh
                    color: "#466eeb"
                    radius: 10
                    Text {
                        id: tvAlarmHighVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: Number(myobject.tvAlarmHigh_default());
                        font.weight: Font.DemiBold
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                    }

                    Text {
                        id: element6
                        x: 44
                        y: 13
                        text: qsTr("TV High")
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                    }

                    RoundButton {
                        id: tvAlarmHighInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.tvAlarmHigh_inc();
                            tvAlarmHighVal.text =  Number(myobject.tvAlarmHigh_val());
                            }
                    }

                    RoundButton {
                        id: tvAlarmHighDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.tvAlarmHigh_dec();
                            tvAlarmHighVal.text =  Number(myobject.tvAlarmHigh_val());
                            }
                    }
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                }

                Rectangle {
                    id: tvAlarmLow
                    color: "#466eeb"
                    radius: 10
                    Text {
                        id: tvAlarmLowVal
                        x: 45
                        y: 39
                        width: 60
                        height: 44
                        text: Number(myobject.tvAlarmLow_default());
                        font.weight: Font.DemiBold
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Times New Roman"
                        font.pixelSize: 38
                    }

                    Text {
                        id: element54
                        x: 45
                        y: 13
                        text: qsTr("TV Low")
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 17
                    }

                    RoundButton {
                        id: tvAlarmLowInc
                        x: 110
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("+")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.tvAlarmLow_inc();
                            tvAlarmLowVal.text =  Number(myobject.tvAlarmLow_val());
                            }
                    }

                    RoundButton {
                        id: tvAlarmLowDec
                        x: 9
                        y: 45
                        width: 30
                        height: 30
                        text: qsTr("-")
                        checkable: false
                        font.bold: true
                        focusPolicy: Qt.NoFocus

                        onClicked: {
                            myobject.tvAlarmLow_dec();
                            tvAlarmLowVal.text =  Number(myobject.tvAlarmLow_val());
                            }
                    }
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                }


                Layout.maximumHeight: 65356
                Layout.minimumWidth: 200
                Layout.fillWidth: true
                Layout.maximumWidth: 6553
                Layout.fillHeight: true
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
