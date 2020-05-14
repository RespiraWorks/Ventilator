//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0
import Components 1.0 as ComponentsSet

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

AlarmLogs_MainViewPanelLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 19
    y: fillParent ? 0 : 80
    width: fillParent ? (parent ? parent.width : 0) : 986
    height: fillParent ? (parent ? parent.height : 0) : 501

    inputFallthroughBlocked: true

    Item {
        id: fi_PopUpALermLogs
        x: 0
        y: 0
        z: 0
        width: 984
        height: 495
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1

        VentilatorUISet.BoundingBox {
            id: fi_BoundingBox
            x: 0
            y: 0
            z: 0
            width: 984
            height: 500
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Title_horizontalAlignment: Text.AlignHCenter
            fi_Title_opacity: 0.85
            fi_Title_text: qsTr("Alarm Logs")
            fi_Title_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
        Item {
            id: fi_ListAlarmLogs
            x: 10
            y: 108
            z: 1
            width: 964
            height: 379
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            VentilatorUISet.Button_ArrowDown_Default_State {
                id: fi_ScrollUp
                x: 904
                y: 0
                z: 0
                width: 60
                height: 60
                opacity: 1
                enabled: true
                visible: true
                clip: false
                icon_opacity: 1
                icon_source: GreenHouseThemeManager.theme.asset("/Icons/ArrowDown__DefaultState.png")
                rotation: -180
                scale: 1
            }
            ComponentsSet.ModelView {
                id: fi_ListView_ListAlarmLogs_Fit_Delegate_MaxVisible_6
                x: 0
                y: 0
                z: 1
                width: 896
                height: 379
                viewModel: null
                viewDelegate: fi_ListView_ListAlarmLogs_Fit_Delegate_MaxVisible_6_delegate_cmp
                opacity: 1
                enabled: true
                visible: true
                clip: false
                currentDelegate: -1
                delegateSpacing: 0
                flickable: true
                prefferedHighlightBegin: 0
                prefferedHighlightEnd: 0
                rotation: 0
                scale: 1
                sectionProperty: ""
                sizeToContents: false
                stopAtBounds: false
                trackCurrentDelegate: true
                vertical: true
            }
            Item {
                id: fi_ScrollBarArrows
                x: 904
                y: 0
                z: 2
                width: 60
                height: 369
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                VentilatorUISet.ScrollBar {
                    id: fi_ScrollBar
                    x: 0
                    y: 64
                    z: 0
                    width: 60
                    height: 240
                    opacity: 1
                    enabled: true
                    visible: true
                    clip: false
                    rotation: 0
                    scale: 1
                }
            }
            VentilatorUISet.Button_ArrowUp_Default_State {
                id: fi_ScrollDown
                x: 904
                y: 309
                z: 3
                width: 60
                height: 60
                opacity: 1
                enabled: true
                visible: true
                clip: false
                icon_opacity: 1
                icon_source: GreenHouseThemeManager.theme.asset("/Icons/ArrowUp__DefaultState.png")
                rotation: -180
                scale: 1
            }
        }
        Item {
            id: fi_ListLabel
            x: 10
            y: 70
            z: 2
            width: 814
            height: 33
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            Text {
                id: fi_Alert
                x: 0
                y: 0
                z: 0
                width: 93
                height: 33
                text: qsTr("Alert")
                font: GreenHouseThemeManager.theme.font("Figma::Roboto__20_50_0_0")
                color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                opacity: 0.85
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                maximumLineCount: 2147483647
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Text {
                id: fi_Priority
                x: 716
                y: 0
                z: 1
                width: 98
                height: 33
                text: qsTr("Priority")
                font: GreenHouseThemeManager.theme.font("Figma::Roboto__20_50_0_0")
                color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                opacity: 0.85
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                maximumLineCount: 2147483647
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Text {
                id: fi_Time
                x: 561
                y: 0
                z: 2
                width: 73
                height: 33
                text: qsTr("Time")
                font: GreenHouseThemeManager.theme.font("Figma::Roboto__20_50_0_0")
                color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                opacity: 0.85
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                maximumLineCount: 2147483647
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Text {
                id: fi_Date
                x: 391
                y: 0
                z: 3
                width: 72
                height: 33
                text: qsTr("Date")
                font: GreenHouseThemeManager.theme.font("Figma::Roboto__20_50_0_0")
                color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                opacity: 0.85
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                maximumLineCount: 2147483647
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Image {
                id: fi_Divider
                x: 0
                y: 37
                z: 4
                width: 886
                height: 1
                source: GreenHouseThemeManager.theme.asset("/213_1032.png")
                fillMode: Image.Stretch
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
        }
    }
    Component {
        id: fi_ListView_ListAlarmLogs_Fit_Delegate_MaxVisible_6_delegate_cmp
        Item {
            id: fi_ListView_ListAlarmLogs_Fit_Delegate_MaxVisible_6_delegate
            x: 0
            y: 0
            z: 0
            width: 896
            height: 60
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            VentilatorUISet.Delegate_LogList {
                id: fi_ListDelegate
                x: 0
                y: 0
                z: 0
                width: 896
                height: 60
                opacity: 1
                enabled: true
                visible: true
                clip: false
                fi_DateLabel_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                fi_DateLabel_horizontalAlignment: Text.AlignRight
                fi_DateLabel_opacity: 1
                fi_DateLabel_text: qsTr("1/1/2020")
                fi_DateLabel_verticalAlignment: Text.AlignTop
                fi_NameLabel_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                fi_NameLabel_horizontalAlignment: Text.AlignLeft
                fi_NameLabel_opacity: 1
                fi_NameLabel_text: qsTr("Over Pressure detected")
                fi_NameLabel_verticalAlignment: Text.AlignVCenter
                fi_PriorityIndicator_fi_StateLabel___high_text: qsTr("High")
                fi_PriorityIndicator_fi_StateLabel___medium_init_text: qsTr("Medium")
                fi_PriorityIndicator_fi_StateLabel_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                fi_PriorityIndicator_fi_StateLabel_horizontalAlignment: Text.AlignLeft
                fi_PriorityIndicator_fi_StateLabel_opacity: 1
                fi_PriorityIndicator_fi_StateLabel_verticalAlignment: Text.AlignVCenter
                fi_PriorityIndicator_high_flag: false
                fi_PriorityIndicator_icon___high_source: GreenHouseThemeManager.theme.asset("/Icons/Indicator__HighState.png")
                fi_PriorityIndicator_icon___medium_init_source: GreenHouseThemeManager.theme.asset("/Icons/Indicator__MediumState.png")
                fi_PriorityIndicator_icon_opacity: 1
                fi_PriorityIndicator_medium_init_flag: true
                fi_Rectangle_479___default_color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                fi_Rectangle_479___default_opacity: 1
                fi_Rectangle_479___selected_color: GreenHouseThemeManager.theme.color("Figma::255_36_98_191")
                fi_Rectangle_479___selected_opacity: 0.5
                fi_TimeLabel_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                fi_TimeLabel_horizontalAlignment: Text.AlignLeft
                fi_TimeLabel_opacity: 1
                fi_TimeLabel_text: qsTr("12:00PM")
                fi_TimeLabel_verticalAlignment: Text.AlignTop
                itemIndex: 0
                rotation: 0
                scale: 1
                selectionIndex: -1
            }
        }
    }

    showAnimation: AlarmLogs_MainViewPanelLayoutShowAnimation { target: root }
    hideAnimation: AlarmLogs_MainViewPanelLayoutHideAnimation { target: root }
    initAction: AlarmLogs_MainViewPanelLayoutInitAnimation { target: root }

    itemInterface: AlarmLogs_MainViewPanelIFaceImpl {}
}
