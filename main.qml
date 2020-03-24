import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.VirtualKeyboard 2.14
import QtCharts 2.3
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.0


Window {
    id: window
    visible: true
    width: 1024
    height: 600
    title: qsTr("Ventilator")

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
    

    ButtonGroup {
        id: mode_button_group
        buttons: button_column.children

    }
    Column{
        id: button_column
        x: 15
        y: 28
        width: 253
        height: 361
        spacing: 25

        Label {
            id: modes_label
            width: 227
            height: 65
            text: qsTr("Ventilation Mode Selection")
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 20
        }

        DelayButton {
            id: assist_control_delay_button
            x: 26
            y: 255
            width: 190
            height: 86
            delay: 1000
            text: qsTr("Assist Control Ventilation \n(ACV / CMV)")
            autoExclusive: true
            ButtonGroup.group: mode_button_group
        }

        DelayButton
        {
            id: psv_delay_button
            x: 26
            y: 361
            width: 190
            height: 86
            delay: 1000
            text: qsTr("Partial Support Ventilation \n(PCV)")
            autoExclusive: true
            ButtonGroup.group: mode_button_group

        }

        DelayButton {
            id: off_delay_button
            x: 26
            y: 467
            width: 190
            height: 86
            delay: 1000
            text: qsTr("Stop")
            autoExclusive: true
            ButtonGroup.group: mode_button_group


        }


    }



}


