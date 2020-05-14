//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0
import Components 1.0 as ComponentsSet

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

ActiveAlertsFrameLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 0
    y: fillParent ? 0 : 0
    width: fillParent ? (parent ? parent.width : 0) : 1024
    height: fillParent ? (parent ? parent.height : 0) : 600

    inputFallthroughBlocked: true

    VentilatorUISet.ScreenBackground {
        id: fi_ScreenBackground
        x: 0
        y: 60
        z: 0
        width: 1024
        height: 540
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    Item {
        id: fi_PopupActiveAlertsFrame
        x: 106
        y: 125
        z: 1
        width: 814
        height: 419
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
            width: 814
            height: 419
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Title_horizontalAlignment: Text.AlignHCenter
            fi_Title_opacity: 0.85
            fi_Title_text: qsTr("Active Alerts")
            fi_Title_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
        Item {
            id: fi_ListActiveAlerts
            x: 40
            y: 86
            z: 1
            width: 734
            height: 136
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            ComponentsSet.ModelView {
                id: fi_ListView_ListActiveAlerts_Fit_Delegate_MaxVisible_2
                x: 0
                y: 0
                z: 0
                width: 734
                height: 136
                viewModel: null
                viewDelegate: fi_ListView_ListActiveAlerts_Fit_Delegate_MaxVisible_2_delegate_cmp
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
        }
        Item {
            id: fi_ListLabel
            x: 40
            y: 56
            z: 3
            width: 733
            height: 40
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            Image {
                id: fi_Line_10
                x: 0
                y: 38
                z: 0
                width: 733
                height: 1
                source: GreenHouseThemeManager.theme.asset("/213_1125.png")
                fillMode: Image.Stretch
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Text {
                id: fi_Tidal_V_cm
                x: 614
                y: 0
                z: 2
                width: 98
                height: 33
                text: qsTr("Priority")
                font: GreenHouseThemeManager.theme.font("Figma::TeX Gyre Adventor_TeXGyreAdventor-Regular_20_50_0_0")
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
        }
        VentilatorUISet.Button_AlarmOff_Default_State {
            id: fi_AlarmOff
            x: 40
            y: 289
            z: 2
            width: 90
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            icon_opacity: 1
            icon_source: GreenHouseThemeManager.theme.asset("/Icons/AlarmOff__DefaultState.png")
            rotation: 0
            scale: 1
        }
        VentilatorUISet.Button_TextPrimary_Default_State {
            id: fi_ViewLogs
            x: 473
            y: 289
            z: 4
            width: 300
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignHCenter
            fi_Label_opacity: 1
            fi_Label_text: qsTr("View Logs")
            fi_Label_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.set_VentilatorParametersInterface_alertFlag(false)
                root.itemInterface.surface_Screens_MoveTo_AlarmLogs_ScreenState()
                root.itemInterface.moveToNoPopup()
            }
        }
    }
    Component {
        id: fi_ListView_ListActiveAlerts_Fit_Delegate_MaxVisible_2_delegate_cmp
        Item {
            id: fi_ListView_ListActiveAlerts_Fit_Delegate_MaxVisible_2_delegate
            x: 0
            y: 0
            z: 0
            width: 733
            height: 60
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            VentilatorUISet.Delegate_AlertPopup {
                id: fi_ListDelegate
                x: 0
                y: 0
                z: 0
                width: 733
                height: 60
                opacity: 1
                enabled: true
                visible: true
                clip: false
                fi_Background___default_color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                fi_Background___default_opacity: 1
                fi_Background___selected_color: GreenHouseThemeManager.theme.color("Figma::255_36_98_191")
                fi_Background___selected_opacity: 0.5
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
                itemIndex: 0
                rotation: 0
                scale: 1
                selectionIndex: -1
            }
        }
    }

    showAnimation: ActiveAlertsFrameLayoutShowAnimation { target: root }
    hideAnimation: ActiveAlertsFrameLayoutHideAnimation { target: root }
    initAction: ActiveAlertsFrameLayoutInitAnimation { target: root }

    itemInterface: ActiveAlertsFrameIFaceImpl {}
}
