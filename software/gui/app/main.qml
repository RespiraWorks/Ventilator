import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import Respira 1.0
import "modes"
import "controls"

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    // Setting max and  min size
    // to avoid resize screens on windowed mode
    minimumWidth: 1024
    maximumWidth: 1024
    minimumHeight: 600
    maximumHeight: 600
    height: 600
    title: qsTr("Ventilator")
    visibility: GuiStateContainer.isDebugBuild ?
                    ApplicationWindow.Windowed :
                    ApplicationWindow.FullScreen
    // uncomment this like if you want to see it full-screen
    // But that shouldn't matter on rPI running on EGLFS
    //visibility: Qt.WindowFullScreen

    background: Rectangle {
        color: Style.theme.color.windowBackground
    }

    FpsItem {
        visible: GuiStateContainer.isDebugBuild
        anchors {
            right: parent.right; rightMargin: 8
            bottom: parent.bottom; bottomMargin: 8
        }
        z: 10
    }

    header: ToolBar {

        contentHeight: 60
        background:  Item {  }

        MainHeader {
            anchors {
                left: parent.left; leftMargin: 8
                top: parent.top;
            }

            onMenuClicked: console.log("Menu Click")
            onAlarmSettingsClicked: console.log("Alarm settings clicked")
            onModeSelectionClicked: modeSelectionPopup.open()
        }

        DeviceStatusDisplay {
            anchors {
                right: parent.right; rightMargin: 8
                verticalCenter: parent.verticalCenter
            }
        }

        AlarmSound {
            priority: GuiStateContainer.alarmManager.highestPriorityActiveAlarm.effectiveAudioPriority
        }

        AlarmNotificationBanner {
            id: alarmNotificationBanner
            priority: GuiStateContainer.alarmManager.highestPriorityActiveAlarm.effectiveAudioPriority
            title: GuiStateContainer.alarmManager.highestPriorityActiveAlarm.bannerText
            numActiveAlarms: GuiStateContainer.alarmManager.numActiveAlarms
            onPauseAlarmClicked: {
              GuiStateContainer.alarmManager.acknowledgeHighestPriorityActiveAlarm()
            }
            z: 999 // just to make sure its always on top
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
        replaceEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 300
            }
        }
        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
        property var currentMode: modesModel.get(0)

        property ListModel modesModel: ListModel {
            ListElement {
                mode: GuiStateContainer.PRESSURE_CONTROL
                title: qsTr("Pressure control")
                description: "Continuous Mandatory Ventilation"
                acronym: "PC-CMV"
            }
            ListElement {
                mode: GuiStateContainer.PRESSURE_ASSIST
                title: qsTr("Pressure Assist")
                description: "Pressure Control - Assist Control"
                acronym: "PC-AC"
            }
            ListElement {
                mode: GuiStateContainer.HIGH_FLOW_NASAL_CANNULA
                title: qsTr("High-flow Nasal Cannula")
                description: "High flow nasal cannula"
                acronym: "HFNC"
            }
            ListElement {
                mode: GuiStateContainer.VC
                title: qsTr("VC")
                description: "Volume Control - Continuous Mandatory Ventilation"
                acronym: "VC-CMV"
            }
            ListElement {
                mode: GuiStateContainer.CPAP
                title: qsTr("CPAP")
                description: "CPAP"
                acronym: "CPAP"
            }
            ListElement {
                mode: GuiStateContainer.VC_AC
                title: qsTr("VC_AC")
                description: "Volume Control - Assist Control"
                acronym: "VC-AC"
            }

            ListElement {
                mode: GuiStateContainer.PSV
                title: qsTr("PSV")
                description: "Pressure Support Ventilation"
                acronym: "PC-PSV"
            }
            ListElement {
                mode: GuiStateContainer.SIMVPC
                title: qsTr("SIMVPC")
                description: "Synchronized Intermittent Mandatory Ventilation"
                acronym: "PC-SIMV"
            }
            ListElement {
                mode: GuiStateContainer.SIMVVC
                title: qsTr("SIMVVC")
                description: "Synchronized Intermittent Mandatory Ventilation"
                acronym: "VC-SIMV"
            }

            ListElement {
                mode: GuiStateContainer.BIPAP
                title: qsTr("BIPAP")
                description: "Pressure Control- Bi-level Positive Airway Pressure"
                acronym: "PC-BIPAP"
            }

            ListElement {
                mode: GuiStateContainer.PRVC
                title: qsTr("PRVC")
                description: "Pressure Regulated Volume Control"
                acronym: "PRVC"
            }
            ListElement {
                mode: GuiStateContainer.SPV
                title: qsTr("SPV")
                description: "SPV"
                acronym: "SPV"
            }
      /*      ListElement {
                mode: GuiStateContainer.OFF
                title: qsTr("OFF")
                description: "OFF"
                acronym: "OFF"
            }*/
        }
        onCurrentModeChanged: {
           if (currentMode.mode === GuiStateContainer.PRESSURE_CONTROL) {
              pageStack.replace(pressureControlMode)
            } else if (currentMode.mode === GuiStateContainer.PRESSURE_ASSIST) {
              pageStack.replace(pressureAssistMode);
            } else if (currentMode.mode === GuiStateContainer.HIGH_FLOW_NASAL_CANNULA) {
              pageStack.replace(highFlowNasalCannulaMode);
            }
            else if (currentMode.mode === GuiStateContainer.VC) {
              pageStack.replace(volumeControlVentilationMode);
            }
            else if (currentMode.mode === GuiStateContainer.CPAP) {
              pageStack.replace(cpapMode);
            }
            else if (currentMode.mode === GuiStateContainer.VC_AC) {
              pageStack.replace(volumeAssistVentilationMode);
            }
            else if (currentMode.mode === GuiStateContainer.PSV) {
              pageStack.replace(presureSupportVentilationMode);
            }
            else if (currentMode.mode === GuiStateContainer.SIMVPC) {
              pageStack.replace(syncIMVPCMode);
            }
            else if (currentMode.mode === GuiStateContainer.SIMVVC) {
              pageStack.replace(syncIMVVCMode);
            }
            else if (currentMode.mode === GuiStateContainer.BIPAP) {
              pageStack.replace(bIPAPMode);
            }
            else if (currentMode.mode === GuiStateContainer.PRVC) {
              pageStack.replace(pressureRegulatedVolumeControlMode);
            }
            else if (currentMode.mode === GuiStateContainer.SPV) {
              pageStack.replace(spontaneousVentilationModel);
            }
            GuiStateContainer.mode = currentMode.mode
            GuiStateContainer.commanded_mode = currentMode.mode
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
        id: pressureControlMode
        PressureControlMode {}
    }

    Component {
        id: highFlowNasalCannulaMode
        HighFlowNasalCannulaMode {}
    }
    Component {
        id: volumeControlVentilationMode
        VolumeControlVentilationMode {}
    }
    Component {
        id: volumeAssistVentilationMode
        VolumeAssistVentilationMode {}
    }
    Component {
        id: cpapMode
        CPAPMode {}
    }
    Component {
        id: presureSupportVentilationMode
        PresureSupportVentilation {}
    }
    Component {
        id: syncIMVPCMode
        SyncIMVPC {}
    }
    Component {
        id: syncIMVVCMode
        SyncIMVVC {}
    }
    Component {
        id: bIPAPMode
        BIPAP {}
    }
    Component {
        id: pressureRegulatedVolumeControlMode
        PressureRegulatedVolumeControlMode {}
    }
    Component {
        id: spontaneousVentilationModel
        SpontaneousVentilationModel {}
    }
}
