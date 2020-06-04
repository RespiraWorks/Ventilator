import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "modes"
import "controls"

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 600
    title: qsTr("Ventilator")

    // uncomment this like if you want to see it full-screen
    // But that shouldn't matter on rPI running on EGLFS
    //visibility: Qt.WindowFullScreen

    background: Rectangle {
        color: Style.theme.color.windowBackground
    }

    header: ToolBar {
        contentHeight: 60
        padding: Style.margin.normal
        background:  Item {  }

        //TODO: Check with UX people how this button will look
        // like
        HeaderButton {
            id: modeSelectionButton

            width: 90

            anchors.verticalCenter: parent.verticalCenter
            text: pageStack.currentMode.acronym
            onClicked: modeSelectionPopup.open()
            contentItem: Text {
                text: modeSelectionButton.text
                font.pixelSize: 19
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        FpsItem {
            anchors {
                right: parent.right
            }
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            // width will naturally be selected preserving ratio
            height: parent.height
            source: "images/respiraWorksLogoHorizontalTransparent.png"
            fillMode: Image.PreserveAspectFit
        }

    }

    ModeSelectionPopup {
        id: modeSelectionPopup
        currentMode: pageStack.currentMode
        modesModel: pageStack.modesModel
    }

    StackView {
        id: pageStack
        anchors.fill: parent

        property var currentMode: modesModel.get(0)

        property ListModel modesModel: ListModel {
            ListElement {
                mode: "command_pressure_mode"
                title: qsTr("Command Pressure")
                acronym: "PC"
            }
            ListElement {
                mode: "pressure_assist_mode"
                title: qsTr("Pressure Assist")
                acronym: "A/C PC"
            }
            ListElement {
                mode: "high_flow_nasal_cannula_mode"
                title: qsTr("High-flow nasal cannula")
                acronym: "HFNC"
            }
        }

        property var components: {
            "command_pressure_mode": commandPressureMode,
            "pressure_assist_mode": pressureAssistMode,
            "high_flow_nasal_cannula_mode": highFlowNasalCannulaMode
        }

        onCurrentModeChanged: {
            pageStack.replace(components[currentMode.mode])
        }

        Connections {
            target: modeSelectionPopup
            onSelectedMode: pageStack.currentMode = mode
        }
    }

    Component {
        id: pressureAssistMode
        PressureAssistMode {}
    }

    Component {
        id: commandPressureMode
        CommandPressureMode {}
    }

    Component {
        id: highFlowNasalCannulaMode
        HighFlowNasalCannulaMode {}
    }
}
