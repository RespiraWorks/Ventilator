//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

TopBarPanel_Shared0Layout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 0
    y: fillParent ? 0 : 0
    width: fillParent ? (parent ? parent.width : 0) : 1024
    height: fillParent ? (parent ? parent.height : 0) : 60

    inputFallthroughBlocked: true

    VentilatorUISet.TopBar {
        id: fi_TopBar
        x: 0
        y: 0
        z: 0
        width: 1024
        height: 60
        opacity: 1
        enabled: VentilatorParametersInterface.preUseTestPassed
        visible: true
        alert_flag: VentilatorParametersInterface.alertFlag
        clip: false
        fi_ACP_enabled: true
        fi_ACP_fi_AC_P_MODE___default_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_ACP_fi_AC_P_MODE___default_horizontalAlignment: Text.AlignHCenter
        fi_ACP_fi_AC_P_MODE___default_opacity: 0.85
        fi_ACP_fi_AC_P_MODE___default_text: VentilatorParametersInterface.acpModeName
        fi_ACP_fi_AC_P_MODE___default_verticalAlignment: Text.AlignVCenter
        fi_ACP_fi_Subtract___default_source: ""
        fi_ACP_fi_Subtract___pressed_source: GreenHouseThemeManager.theme.asset("/233_1735.png")
        fi_ACP_fi_Subtract___triggered_source: GreenHouseThemeManager.theme.asset("/138_3507.png")
        fi_ACP_fi_Subtract_opacity: 1
        fi_ACP_triggered: false
        fi_AlertText___alert_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_AlertText___alert_horizontalAlignment: Text.AlignHCenter
        fi_AlertText___alert_opacity: 1
        fi_AlertText___alert_text: VentilatorParametersInterface.alertText
        fi_AlertText___alert_verticalAlignment: Text.AlignVCenter
        fi_BatteryFillPercentage_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_BatteryFillPercentage_horizontalAlignment: Text.AlignHCenter
        fi_BatteryFillPercentage_opacity: 1
        fi_BatteryFillPercentage_text: VentilatorParametersInterface.batteryPercentageText
        fi_BatteryFillPercentage_verticalAlignment: Text.AlignVCenter
        fi_BatteryIndicator___alert_fi_ProgressBar_Battery_progressBar_labelVisible: false
        fi_BatteryIndicator___default_fi_ProgressBar_Battery_progressBar_labelVisible: false
        fi_BatteryIndicator_fi_ProgressBar_Battery_progressBar_max: 100
        fi_BatteryIndicator_fi_ProgressBar_Battery_progressBar_min: 0
        fi_BatteryIndicator_fi_ProgressBar_Battery_progressBar_value: VentilatorParametersInterface.batteryLevel
        fi_BatteryIndicator_fi_Rectangle_21_color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
        fi_BatteryIndicator_fi_Rectangle_21_opacity: 0
        fi_BatteryIndicator_fi_Rectangle_28_opacity: 1
        fi_BatteryIndicator_fi_Rectangle_28_source: GreenHouseThemeManager.theme.asset("/728_30.png")
        fi_BatteryIndicator_fi_Union_opacity: 1
        fi_BatteryIndicator_fi_Union_source: GreenHouseThemeManager.theme.asset(VentilatorParametersInterface.batteryPercentageAsset)
        fi_PowerSource_fi_Rectangle_26_color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
        fi_PowerSource_fi_Rectangle_26_opacity: 0
        fi_PowerSource_fi_Union_opacity: 1
        fi_PowerSource_fi_Union_source: GreenHouseThemeManager.theme.asset(VentilatorParametersInterface.powerSourceAsset)
        fi_Power_enabled: true
        fi_Power_fi_Shape___default_opacity: 1
        fi_Power_fi_Shape___default_source: GreenHouseThemeManager.theme.asset("/122_2249.png")
        fi_Power_fi_Subtract___pressed_opacity: 1
        fi_Power_fi_Subtract___pressed_source: GreenHouseThemeManager.theme.asset("/256_817.png")
        fi_Settings_enabled: true
        fi_Settings_fi_Rectangle_21_color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
        fi_Settings_fi_Rectangle_21_opacity: 0
        fi_Settings_fi_Union___default_source: GreenHouseThemeManager.theme.asset("/122_2230.png")
        fi_Settings_fi_Union___pressed_source: GreenHouseThemeManager.theme.asset("/233_1692.png")
        fi_Settings_fi_Union_opacity: 1
        icon___alert_opacity: 1
        icon___alert_source: GreenHouseThemeManager.theme.asset("/Icons/Alarm__DefaultState.png")
        rotation: 0
        scale: 1

        MouseArea {
            id: mouseArea0
            x: 172
            y: 6
            z: 0
            width: 527
            height: 43
            opacity: 1
            enabled: VentilatorParametersInterface.alertFlag
            visible: true
            clip: false
            hoverEnabled: false
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.surface_Popups_MoveTo_ActiveAlertsFrame()
            }
        }

        onFi_ACP___alert_clicked: {
            root.itemInterface.surface_Popups_MoveTo_ModeSelection()
        }
        onFi_ACP_clicked: {
            root.itemInterface.surface_Popups_MoveTo_ModeSelection()
        }
        onFi_Power___alert_clicked: {
            root.itemInterface.set_VentilatorParametersInterface_preUseTestPassed(false)
        }
        onFi_Power_clicked: {
            root.itemInterface.set_VentilatorParametersInterface_preUseTestPassed(false)
        }
        onFi_Settings___alert_clicked: {
            root.itemInterface.set_VentilatorParametersInterface_showSettings(!VentilatorParametersInterface.showSettings)
        }
        onFi_Settings_clicked: {
            root.itemInterface.set_VentilatorParametersInterface_showSettings(!VentilatorParametersInterface.showSettings)
        }
    }

    showAnimation: TopBarPanel_Shared0LayoutShowAnimation { target: root }
    hideAnimation: TopBarPanel_Shared0LayoutHideAnimation { target: root }
    initAction: TopBarPanel_Shared0LayoutInitAnimation { target: root }

    itemInterface: TopBarPanel_Shared0IFaceImpl {}
}
