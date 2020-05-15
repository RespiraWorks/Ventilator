//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0
import Decorative 1.0 as DecorativeSet

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

Alarms_Center_Content_PanelLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 10
    y: fillParent ? 0 : 140
    width: fillParent ? (parent ? parent.width : 0) : 1900
    height: fillParent ? (parent ? parent.height : 0) : 830

    inputFallthroughBlocked: true

    Item {
        id: fi_Heart_Rate
        x: 0
        y: 0
        z: 0
        width: 475
        height: 830
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    Item {
        id: fi_SpO2
        x: 475
        y: 0
        z: 1
        width: 475
        height: 830
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    Text {
        id: fi_Systolic_BP
        x: 1
        y: 4
        z: 10
        width: 473
        height: 38
        text: qsTr("Systolic BP")
        font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Bold_30_75_1_0")
        color: GreenHouseThemeManager.theme.color("Figma::255_255_179_198")
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
    Text {
        id: fi_Diastolic_BP
        x: 1
        y: 4
        z: 10
        width: 473
        height: 38
        text: qsTr("Diastolic BP")
        font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Bold_30_75_1_0")
        color: GreenHouseThemeManager.theme.color("Figma::255_255_179_198")
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

        Item {
            id: fi_Dark_AlarmLimits
            x: 0
            y: 217
            z: 0
            width: 475
            height: 613
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Rectangle {
                id: fi_BB
                x: 0
                y: 611
                z: 1
                width: 475
                height: 2
                color: GreenHouseThemeManager.theme.color("Figma::255_153_187_255")
                opacity: 0.2
                border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                border.width: 1
                enabled: true
                visible: true
                clip: false
                radius: 0
                rotation: 0
                scale: 1
            }
            Image {
                id: fi_TB
                x: 20
                y: 2
                z: 3
                width: 433
                height: 2
                source: GreenHouseThemeManager.theme.asset("/283_931.png")
                fillMode: Image.Stretch
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Text {
                id: fi_ALARM_LIMITS
                x: 19
                y: 32
                z: 5
                width: 173
                height: 32
                text: qsTr("ALARM LIMITS")
                font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Bold_25_75_0_0")
                color: GreenHouseThemeManager.theme.color("Figma::255_99_122_168")
                opacity: 1
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                maximumLineCount: 2147483647
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Item {
                id: fi_LowValueSelector
                x: 256
                y: 105
                z: 6
                width: 198
                height: 382
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Item {
                id: fi_HighValueSelector
                x: 20
                y: 105
                z: 7
                width: 198
                height: 382
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                Item {
                    id: fi_DecreaseButton
                    x: 0
                    y: 322
                    z: 0
                    width: 197
                    height: 60
                    opacity: 1
                    enabled: true
                    visible: true
                    clip: true
                    rotation: -180
                    scale: 1
                }
                Item {
                    id: fi_Dark_Alarm_LimitValue
                    x: 1
                    y: 146
                    z: 1
                    width: 197
                    height: 176
                    opacity: 1
                    enabled: true
                    visible: true
                    clip: false
                    rotation: 0
                    scale: 1

                    Text {
                        id: fi_currentValue
                        x: 51
                        y: 17
                        z: 1
                        width: 96
                        height: 98
                        text: qsTr("82")
                        font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Regular_80_50_1_0")
                        color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
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
                Item {
                    id: fi_IncreaseButton
                    x: 1
                    y: 86
                    z: 2
                    width: 197
                    height: 60
                    opacity: 1
                    enabled: true
                    visible: true
                    clip: true
                    rotation: 0
                    scale: 1

                    Image {
                        id: fi_Background
                        x: 0
                        y: 0
                        z: 0
                        width: 197
                        height: 60
                        source: GreenHouseThemeManager.theme.asset("/283_960.png")
                        fillMode: Image.Stretch
                        opacity: 1
                        enabled: true
                        visible: true
                        clip: false
                        rotation: 0
                        scale: 1
                    }
                    DecorativeSet.DecoratedItem {
                        id: fi_Polygon
                        x: 73
                        y: 17
                        z: 1
                        width: 50
                        height: 30
                        source: GreenHouseThemeManager.theme.asset("/283_961.png")
                        opacity: 1
                        enabled: true
                        visible: true
                        clip: false
                        horizontalCenterOffset: 0
                        rotation: 0
                        scale: 1
                        verticalCenterOffset: 0
                    }
                }
                Text {
                    id: fi_Label
                    x: 0
                    y: -1
                    z: 3
                    width: 198
                    height: 58
                    text: qsTr("High")
                    font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Light_46_50_0_0")
                    color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
                    opacity: 1
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
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
        }
        Item {
            id: fi_Dark_AlarmCurrentValue
            x: 0
            y: 0
            z: 1
            width: 475
            height: 217
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Text {
                id: fi_Current
                x: 30
                y: 120
                z: 2
                width: 88
                height: 32
                text: qsTr("Current")
                font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Regular_25_50_0_0")
                color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
                opacity: 1
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignTop
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
                id: fi_UnitLabel
                x: 351
                y: 110
                z: 3
                width: 110
                height: 46
                text: qsTr("bpm")
                font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Light_36_50_0_0")
                color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
                opacity: 0.6
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
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
                id: fi_Unit_Backpanel
                x: 137
                y: 85
                z: 4
                width: 197
                height: 104
                source: GreenHouseThemeManager.theme.asset("/283_968.png")
                fillMode: Image.Stretch
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Rectangle {
                id: fi_HeaderBG
                x: 0
                y: 0
                z: 5
                width: 475
                height: 50
                color: GreenHouseThemeManager.theme.color("Figma::255_153_187_255")
                opacity: 0.2
                border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                border.width: 1
                enabled: true
                visible: true
                clip: false
                radius: 0
                rotation: 0
                scale: 1
            }
            DecorativeSet.DecoratedItem {
                id: fi_HeaderTB
                x: 0
                y: 0
                z: 6
                width: 473
                height: 2
                source: GreenHouseThemeManager.theme.asset("/283_970.png")
                opacity: 1
                enabled: true
                visible: true
                clip: false
                horizontalCenterOffset: 0
                rotation: 0
                scale: 1
                verticalCenterOffset: 0
            }
            DecorativeSet.DecoratedItem {
                id: fi_HeaderBB
                x: 0
                y: 48
                z: 7
                width: 473
                height: 2
                source: GreenHouseThemeManager.theme.asset("/283_971.png")
                opacity: 1
                enabled: true
                visible: true
                clip: false
                horizontalCenterOffset: 0
                rotation: 0
                scale: 1
                verticalCenterOffset: 0
            }
            DecorativeSet.DecoratedItem {
                id: fi_HeaderLB
                x: 0
                y: 2
                z: 8
                width: 2
                height: 46
                source: GreenHouseThemeManager.theme.asset("/283_819.png")
                opacity: 1
                enabled: true
                visible: true
                clip: false
                horizontalCenterOffset: 0
                rotation: 0
                scale: 1
                verticalCenterOffset: 0
            }
            DecorativeSet.DecoratedItem {
                id: fi_HeaderRB
                x: 473
                y: 0
                z: 9
                width: 2
                height: 50
                source: GreenHouseThemeManager.theme.asset("/283_973.png")
                opacity: 1
                enabled: true
                visible: true
                clip: false
                horizontalCenterOffset: 0
                rotation: 0
                scale: 1
                verticalCenterOffset: 0
            }
            Text {
                id: fi_RegularTitle
                x: 1
                y: 4
                z: 10
                width: 473
                height: 38
                text: qsTr("Heart Rate")
                font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Bold_30_75_1_0")
                color: GreenHouseThemeManager.theme.color("Figma::255_166_255_196")
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
            Item {
                id: fi_AlarmValue
                x: 136
                y: 81
                z: 11
                width: 200
                height: 110
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                Text {
                    id: fi_value
                    x: -1
                    y: -1
                    z: 0
                    width: 202
                    height: 112
                    text: qsTr("108")
                    font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Regular_90_50_1_0")
                    color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
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
            Text {
                id: fi_
                x: 351
                y: 110
                z: 3
                width: 110
                height: 46
                text: qsTr("%")
                font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Light_36_50_0_0")
                color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
                opacity: 0.6
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
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
                id: fi_mmHg
                x: 351
                y: 110
                z: 3
                width: 110
                height: 46
                text: qsTr("mmHg")
                font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Light_36_50_0_0")
                color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
                opacity: 0.6
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                maximumLineCount: 2147483647
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Rectangle {
                id: fi_BG
                x: 0
                y: 0
                z: 0
                width: 475
                height: 217
                color: GreenHouseThemeManager.theme.color("Figma::255_0_4_12")
                opacity: 0.3
                border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                border.width: 1
                enabled: true
                visible: true
                clip: false
                radius: 0
                rotation: 0
                scale: 1

                Image {
                    id: fi_panel
                    x: 0
                    y: 0
                    z: 0
                    width: 197
                    height: 176
                    source: GreenHouseThemeManager.theme.asset("/283_956.png")
                    fillMode: Image.Stretch
                    opacity: 1
                    enabled: true
                    visible: true
                    clip: false
                    rotation: 0
                    scale: 1
                }
                Text {
                    id: fi_label
                    x: 73
                    y: 111
                    z: 1
                    width: 53
                    height: 32
                    text: qsTr("bpm")
                    font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Regular_25_50_0_0")
                    color: GreenHouseThemeManager.theme.color("Figma::255_99_122_168")
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
            Rectangle {
                id: fi_RB
                x: 473
                y: 50
                z: 1
                width: 2
                height: 167
                color: GreenHouseThemeManager.theme.color("Figma::255_153_187_255")
                opacity: 0.2
                border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                border.width: 1
                enabled: true
                visible: true
                clip: false
                radius: 0
                rotation: 0
                scale: 1
            }
        }
    }

    showAnimation: Alarms_Center_Content_PanelLayoutShowAnimation { target: root }
    hideAnimation: Alarms_Center_Content_PanelLayoutHideAnimation { target: root }
    initAction: Alarms_Center_Content_PanelLayoutInitAnimation { target: root }

    itemInterface: Alarms_Center_Content_PanelIFaceImpl {}
}
