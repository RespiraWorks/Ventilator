import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0
import VentilatorUI 1.0 as VentilatorUISet

//USES VentilatorUISet.BatteryIndicator
//USES Text
//USES Item
//USES VentilatorUISet.Button_Settings
//USES VentilatorUISet.Button_ACP
//USES VentilatorUISet.Button_Power
//USES Image
//USES VentilatorUISet.Power_Source

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    property bool alert_flag: false
    //BOOL true
    property alias fi_ACP_enabled: fi_ACP___alert.enabled
    //COLOR buttonAltTextColor
    property alias fi_ACP_fi_AC_P_MODE___default_color: fi_ACP___alert.fi_AC_P_MODE___default_color
    //TEXTHAL 4
    property alias fi_ACP_fi_AC_P_MODE___default_horizontalAlignment: fi_ACP___alert.fi_AC_P_MODE___default_horizontalAlignment
    //REAL 0.85
    property alias fi_ACP_fi_AC_P_MODE___default_opacity: fi_ACP___alert.fi_AC_P_MODE___default_opacity
    //TRANSLATABLE AC/P
    property alias fi_ACP_fi_AC_P_MODE___default_text: fi_ACP___alert.fi_AC_P_MODE___default_text
    //TEXTVAL 128
    property alias fi_ACP_fi_AC_P_MODE___default_verticalAlignment: fi_ACP___alert.fi_AC_P_MODE___default_verticalAlignment
    //ASSET
    property alias fi_ACP_fi_Subtract___default_source: fi_ACP___alert.fi_Subtract___default_source
    //ASSET /233_1735.png
    property alias fi_ACP_fi_Subtract___pressed_source: fi_ACP___alert.fi_Subtract___pressed_source
    //ASSET /138_3507.png
    property alias fi_ACP_fi_Subtract___triggered_source: fi_ACP___alert.fi_Subtract___triggered_source
    //REAL 1
    property alias fi_ACP_fi_Subtract_opacity: fi_ACP___alert.fi_Subtract_opacity
    //BOOL false
    property alias fi_ACP_triggered: fi_ACP___alert.triggered
    //COLOR buttonAltTextColor
    property alias fi_AlertText___alert_color: fi_AlertText___alert.color
    //TEXTHAL 4
    property alias fi_AlertText___alert_horizontalAlignment: fi_AlertText___alert.horizontalAlignment
    //REAL 1
    property alias fi_AlertText___alert_opacity: fi_AlertText___alert.opacity
    //TRANSLATABLE Over Pressure Alert
    property alias fi_AlertText___alert_text: fi_AlertText___alert.text
    //TEXTVAL 128
    property alias fi_AlertText___alert_verticalAlignment: fi_AlertText___alert.verticalAlignment
    //COLOR buttonAltTextColor
    property alias fi_BatteryFillPercentage_color: fi_BatteryFillPercentage___alert.color
    //TEXTHAL 4
    property alias fi_BatteryFillPercentage_horizontalAlignment: fi_BatteryFillPercentage___alert.horizontalAlignment
    //REAL 1
    property alias fi_BatteryFillPercentage_opacity: fi_BatteryFillPercentage___alert.opacity
    //TRANSLATABLE 100%
    property alias fi_BatteryFillPercentage_text: fi_BatteryFillPercentage___alert.text
    //TEXTVAL 128
    property alias fi_BatteryFillPercentage_verticalAlignment: fi_BatteryFillPercentage___alert.verticalAlignment
    //BOOL false
    property alias fi_BatteryIndicator___alert_fi_ProgressBar_Battery_progressBar_labelVisible: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_labelVisible
    //INT 100
    property alias fi_BatteryIndicator_fi_ProgressBar_Battery_progressBar_max: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_max
    //INT 0
    property alias fi_BatteryIndicator_fi_ProgressBar_Battery_progressBar_min: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_min
    //INT 25
    property alias fi_BatteryIndicator_fi_ProgressBar_Battery_progressBar_value: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_value
    //COLOR Figma::255_196_196_196
    property alias fi_BatteryIndicator_fi_Rectangle_21_color: fi_BatteryIndicator___alert.fi_Rectangle_21_color
    //REAL 0
    property alias fi_BatteryIndicator_fi_Rectangle_21_opacity: fi_BatteryIndicator___alert.fi_Rectangle_21_opacity
    //REAL 1
    property alias fi_BatteryIndicator_fi_Rectangle_28_opacity: fi_BatteryIndicator___alert.fi_Rectangle_28_opacity
    //ASSET /728_30.png
    property alias fi_BatteryIndicator_fi_Rectangle_28_source: fi_BatteryIndicator___alert.fi_Rectangle_28_source
    //REAL 1
    property alias fi_BatteryIndicator_fi_Union_opacity: fi_BatteryIndicator___alert.fi_Union_opacity
    //ASSET /254_142.png
    property alias fi_BatteryIndicator_fi_Union_source: fi_BatteryIndicator___alert.fi_Union_source
    //BOOL false
    property alias fi_BatteryIndicator___default_fi_ProgressBar_Battery_progressBar_labelVisible: fi_BatteryIndicator___default.fi_ProgressBar_Battery_progressBar_labelVisible
    //COLOR Figma::255_196_196_196
    property alias fi_PowerSource_fi_Rectangle_26_color: fi_PowerSource___alert.fi_Rectangle_26_color
    //REAL 0
    property alias fi_PowerSource_fi_Rectangle_26_opacity: fi_PowerSource___alert.fi_Rectangle_26_opacity
    //REAL 1
    property alias fi_PowerSource_fi_Union_opacity: fi_PowerSource___alert.fi_Union_opacity
    //ASSET /122_2237.png
    property alias fi_PowerSource_fi_Union_source: fi_PowerSource___alert.fi_Union_source
    //BOOL true
    property alias fi_Power_enabled: fi_Power___alert.enabled
    //REAL 1
    property alias fi_Power_fi_Shape___default_opacity: fi_Power___alert.fi_Shape___default_opacity
    //ASSET /122_2249.png
    property alias fi_Power_fi_Shape___default_source: fi_Power___alert.fi_Shape___default_source
    //REAL 1
    property alias fi_Power_fi_Subtract___pressed_opacity: fi_Power___alert.fi_Subtract___pressed_opacity
    //ASSET /256_817.png
    property alias fi_Power_fi_Subtract___pressed_source: fi_Power___alert.fi_Subtract___pressed_source
    //BOOL true
    property alias fi_Settings_enabled: fi_Settings___alert.enabled
    //COLOR Figma::255_196_196_196
    property alias fi_Settings_fi_Rectangle_21_color: fi_Settings___alert.fi_Rectangle_21_color
    //REAL 0
    property alias fi_Settings_fi_Rectangle_21_opacity: fi_Settings___alert.fi_Rectangle_21_opacity
    //ASSET /122_2230.png
    property alias fi_Settings_fi_Union___default_source: fi_Settings___alert.fi_Union___default_source
    //ASSET /233_1692.png
    property alias fi_Settings_fi_Union___pressed_source: fi_Settings___alert.fi_Union___pressed_source
    //REAL 1
    property alias fi_Settings_fi_Union_opacity: fi_Settings___alert.fi_Union_opacity
    //REAL 1
    property alias icon___alert_opacity: icon___alert.opacity
    //ASSET /Icons/Alarm__DefaultState.png
    property alias icon___alert_source: icon___alert.source

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------
    signal fi_ACP___alert_clicked() /*fi_ACP___alert.clicked*/
    signal fi_ACP___alert_pressed() /*fi_ACP___alert.pressed*/
    signal fi_ACP___alert_released() /*fi_ACP___alert.released*/
    signal fi_ACP_clicked() /*fi_ACP___default.clicked*/
    signal fi_ACP_pressed() /*fi_ACP___default.pressed*/
    signal fi_ACP_released() /*fi_ACP___default.released*/
    signal fi_Power___alert_clicked() /*fi_Power___alert.clicked*/
    signal fi_Power___alert_pressed() /*fi_Power___alert.pressed*/
    signal fi_Power___alert_released() /*fi_Power___alert.released*/
    signal fi_Power_clicked() /*fi_Power___default.clicked*/
    signal fi_Power_pressed() /*fi_Power___default.pressed*/
    signal fi_Power_released() /*fi_Power___default.released*/
    signal fi_Settings___alert_clicked() /*fi_Settings___alert.clicked*/
    signal fi_Settings___alert_pressed() /*fi_Settings___alert.pressed*/
    signal fi_Settings___alert_released() /*fi_Settings___alert.released*/
    signal fi_Settings_clicked() /*fi_Settings___default.clicked*/
    signal fi_Settings_pressed() /*fi_Settings___default.pressed*/
    signal fi_Settings_released() /*fi_Settings___default.released*/

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 1024
    height: 60
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Item {
        id: alert_StateParent
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        opacity: 1
        enabled: true
        visible: false
        clip: false
        rotation: 0
        scale: 1

        Item {
            id: fi_Alert_background___alert
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1

            VentilatorUISet.BatteryIndicator {
                id: fi_BatteryIndicator___alert
                anchors.left: parent.left
                anchors.leftMargin: 873
                anchors.right: parent.right
                anchors.rightMargin: 67
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                z: 0
                opacity: 1
                enabled: true
                visible: true
                clip: false
                fi_ProgressBar_Battery_progressBar_labelVisible: false
                fi_ProgressBar_Battery_progressBar_max: 100
                fi_ProgressBar_Battery_progressBar_min: 0
                fi_ProgressBar_Battery_progressBar_value: 25
                fi_Rectangle_21_color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
                fi_Rectangle_21_opacity: 0
                fi_Rectangle_28_opacity: 1
                fi_Rectangle_28_source: GreenHouseThemeManager.theme.asset("/728_30.png")
                fi_Union_opacity: 1
                fi_Union_source: GreenHouseThemeManager.theme.asset("/254_142.png")
                rotation: 0
                scale: 1
            }
            Text {
                id: fi_BatteryFillPercentage___alert
                anchors.left: parent.left
                anchors.leftMargin: 827
                anchors.right: parent.right
                anchors.rightMargin: 151
                anchors.top: parent.top
                anchors.topMargin: 19
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 19
                z: 1
                text: qsTr("100%")
                font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_18_75_1_0")
                color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                opacity: 1
                horizontalAlignment: Text.AlignHCenter
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
        VentilatorUISet.Button_Settings {
            id: fi_Settings___alert
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 958
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 1
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Rectangle_21_color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
            fi_Rectangle_21_opacity: 0
            fi_Union___default_source: GreenHouseThemeManager.theme.asset("/122_2230.png")
            fi_Union___pressed_source: GreenHouseThemeManager.theme.asset("/233_1692.png")
            fi_Union_opacity: 1
            rotation: 0
            scale: 1

            onClicked: {
                root.fi_Settings___alert_clicked();
            }
            onPressed: {
                root.fi_Settings___alert_pressed();
            }
            onReleased: {
                root.fi_Settings___alert_released();
            }
        }
        VentilatorUISet.Button_ACP {
            id: fi_ACP___alert
            anchors.left: parent.left
            anchors.leftMargin: 66
            anchors.right: parent.right
            anchors.rightMargin: 872
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 2
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_AC_P_MODE___default_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_AC_P_MODE___default_horizontalAlignment: Text.AlignHCenter
            fi_AC_P_MODE___default_opacity: 0.85
            fi_AC_P_MODE___default_text: qsTr("AC/P")
            fi_AC_P_MODE___default_verticalAlignment: Text.AlignVCenter
            fi_Subtract___default_source: ""
            fi_Subtract___pressed_source: GreenHouseThemeManager.theme.asset("/233_1735.png")
            fi_Subtract___triggered_source: GreenHouseThemeManager.theme.asset("/138_3507.png")
            fi_Subtract_opacity: 1
            rotation: 0
            scale: 1
            triggered: false

            onClicked: {
                root.fi_ACP___alert_clicked();
            }
            onPressed: {
                root.fi_ACP___alert_pressed();
            }
            onReleased: {
                root.fi_ACP___alert_released();
            }
        }
        VentilatorUISet.Button_Power {
            id: fi_Power___alert
            anchors.left: parent.left
            anchors.leftMargin: 957
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 3
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Shape___default_opacity: 1
            fi_Shape___default_source: GreenHouseThemeManager.theme.asset("/122_2249.png")
            fi_Subtract___pressed_opacity: 1
            fi_Subtract___pressed_source: GreenHouseThemeManager.theme.asset("/256_817.png")
            rotation: 0
            scale: 1

            onClicked: {
                root.fi_Power___alert_clicked();
            }
            onPressed: {
                root.fi_Power___alert_pressed();
            }
            onReleased: {
                root.fi_Power___alert_released();
            }
        }
        Text {
            id: fi_AlertText___alert
            anchors.left: parent.left
            anchors.leftMargin: 306
            anchors.right: parent.right
            anchors.rightMargin: 366
            anchors.top: parent.top
            anchors.topMargin: 11
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 11
            z: 4
            text: qsTr("Over Pressure Alert ")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_32_75_1_0")
            color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            opacity: 1
            horizontalAlignment: Text.AlignHCenter
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
            id: icon___alert
            anchors.left: parent.left
            anchors.leftMargin: 246
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            z: 5
            width: 60
            height: 60
            source: GreenHouseThemeManager.theme.asset("/Icons/Alarm__DefaultState.png")
            fillMode: Image.Stretch
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        VentilatorUISet.Power_Source {
            id: fi_PowerSource___alert
            anchors.left: parent.left
            anchors.leftMargin: 702
            anchors.right: parent.right
            anchors.rightMargin: 199
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 6
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Rectangle_26_color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
            fi_Rectangle_26_opacity: 0
            fi_Union_opacity: 1
            fi_Union_source: GreenHouseThemeManager.theme.asset("/122_2237.png")
            rotation: 0
            scale: 1
        }
    }
    Item {
        id: default_StateParent
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1

        VentilatorUISet.Button_Settings {
            id: fi_Settings___default
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 958
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            opacity: 1
            enabled: fi_Settings___alert.enabled
            visible: true
            clip: false
            fi_Rectangle_21_color: fi_Settings___alert.fi_Rectangle_21_color
            fi_Rectangle_21_opacity: fi_Settings___alert.fi_Rectangle_21_opacity
            fi_Union___default_source: fi_Settings___alert.fi_Union___default_source
            fi_Union___pressed_source: fi_Settings___alert.fi_Union___pressed_source
            fi_Union_opacity: fi_Settings___alert.fi_Union_opacity
            rotation: 0
            scale: 1

            onClicked: {
                root.fi_Settings_clicked();
            }
            onPressed: {
                root.fi_Settings_pressed();
            }
            onReleased: {
                root.fi_Settings_released();
            }
        }
        VentilatorUISet.Button_ACP {
            id: fi_ACP___default
            anchors.left: parent.left
            anchors.leftMargin: 66
            anchors.right: parent.right
            anchors.rightMargin: 872
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 1
            opacity: 1
            enabled: fi_ACP___alert.enabled
            visible: true
            clip: false
            fi_AC_P_MODE___default_color: fi_ACP___alert.fi_AC_P_MODE___default_color
            fi_AC_P_MODE___default_horizontalAlignment: fi_ACP___alert.fi_AC_P_MODE___default_horizontalAlignment
            fi_AC_P_MODE___default_opacity: fi_ACP___alert.fi_AC_P_MODE___default_opacity
            fi_AC_P_MODE___default_text: fi_ACP___alert.fi_AC_P_MODE___default_text
            fi_AC_P_MODE___default_verticalAlignment: fi_ACP___alert.fi_AC_P_MODE___default_verticalAlignment
            fi_Subtract___default_source: fi_ACP___alert.fi_Subtract___default_source
            fi_Subtract___pressed_source: fi_ACP___alert.fi_Subtract___pressed_source
            fi_Subtract___triggered_source: fi_ACP___alert.fi_Subtract___triggered_source
            fi_Subtract_opacity: fi_ACP___alert.fi_Subtract_opacity
            rotation: 0
            scale: 1
            triggered: fi_ACP___alert.triggered

            onClicked: {
                root.fi_ACP_clicked();
            }
            onPressed: {
                root.fi_ACP_pressed();
            }
            onReleased: {
                root.fi_ACP_released();
            }
        }
        VentilatorUISet.Power_Source {
            id: fi_PowerSource___default
            anchors.left: parent.left
            anchors.leftMargin: 712
            anchors.right: parent.right
            anchors.rightMargin: 199
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 2
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Rectangle_26_color: fi_PowerSource___alert.fi_Rectangle_26_color
            fi_Rectangle_26_opacity: fi_PowerSource___alert.fi_Rectangle_26_opacity
            fi_Union_opacity: fi_PowerSource___alert.fi_Union_opacity
            fi_Union_source: fi_PowerSource___alert.fi_Union_source
            rotation: 0
            scale: 1
        }
        VentilatorUISet.Button_Power {
            id: fi_Power___default
            anchors.left: parent.left
            anchors.leftMargin: 957
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 3
            opacity: 1
            enabled: fi_Power___alert.enabled
            visible: true
            clip: false
            fi_Shape___default_opacity: fi_Power___alert.fi_Shape___default_opacity
            fi_Shape___default_source: fi_Power___alert.fi_Shape___default_source
            fi_Subtract___pressed_opacity: fi_Power___alert.fi_Subtract___pressed_opacity
            fi_Subtract___pressed_source: fi_Power___alert.fi_Subtract___pressed_source
            rotation: 0
            scale: 1

            onClicked: {
                root.fi_Power_clicked();
            }
            onPressed: {
                root.fi_Power_pressed();
            }
            onReleased: {
                root.fi_Power_released();
            }
        }
        VentilatorUISet.BatteryIndicator {
            id: fi_BatteryIndicator___default
            anchors.left: parent.left
            anchors.leftMargin: 873
            anchors.right: parent.right
            anchors.rightMargin: 67
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 4
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_ProgressBar_Battery_progressBar_labelVisible: false
            fi_ProgressBar_Battery_progressBar_max: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_max
            fi_ProgressBar_Battery_progressBar_min: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_min
            fi_ProgressBar_Battery_progressBar_value: fi_BatteryIndicator___alert.fi_ProgressBar_Battery_progressBar_value
            fi_Rectangle_21_color: fi_BatteryIndicator___alert.fi_Rectangle_21_color
            fi_Rectangle_21_opacity: fi_BatteryIndicator___alert.fi_Rectangle_21_opacity
            fi_Rectangle_28_opacity: fi_BatteryIndicator___alert.fi_Rectangle_28_opacity
            fi_Rectangle_28_source: fi_BatteryIndicator___alert.fi_Rectangle_28_source
            fi_Union_opacity: fi_BatteryIndicator___alert.fi_Union_opacity
            fi_Union_source: fi_BatteryIndicator___alert.fi_Union_source
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_BatteryFillPercentage___default
            anchors.left: parent.left
            anchors.leftMargin: 827
            anchors.right: parent.right
            anchors.rightMargin: 151
            anchors.top: parent.top
            anchors.topMargin: 19
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 19
            z: 5
            text: fi_BatteryFillPercentage___alert.text
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_18_75_1_0")
            color: fi_BatteryFillPercentage___alert.color
            opacity: fi_BatteryFillPercentage___alert.opacity
            horizontalAlignment: fi_BatteryFillPercentage___alert.horizontalAlignment
            verticalAlignment: fi_BatteryFillPercentage___alert.verticalAlignment
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

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------
    StateGroup { //  generic_alert
        states: [
        State {
            when: root.alert_flag
            name: "alert"
            PropertyChanges {
                target: alert_StateParent
                visible: true
                z: 1
            }
        }
        ]
    }

    //----------------------------------------------------------------------------------------------
}
