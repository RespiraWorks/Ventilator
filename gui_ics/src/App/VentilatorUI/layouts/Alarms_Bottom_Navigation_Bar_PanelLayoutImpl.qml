//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import Decorative 1.0 as DecorativeSet
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

Alarms_Bottom_Navigation_Bar_PanelLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 0
    y: fillParent ? 0 : 971
    width: fillParent ? (parent ? parent.width : 0) : 1920
    height: fillParent ? (parent ? parent.height : 0) : 109

    inputFallthroughBlocked: true

    Item {
        id: fi_Dark_Bar_Navigation
        x: 0
        y: 9
        z: 0
        width: 1920
        height: 100
        opacity: 1
        enabled: true
        visible: true
        clip: true
        rotation: 0
        scale: 1

        Item {
            id: fi_Dark_Buttons_Navigation_Home
            x: 10
            y: 0
            z: 1
            width: 263
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Item {
                id: fi_Dark_Icons_Navigation_Home
                x: 107
                y: 15
                z: 3
                width: 47
                height: 45
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                DecorativeSet.DecoratedItem {
                    id: fi_Home_Icon_Blur
                    x: 0
                    y: 5
                    z: 0
                    width: 47
                    height: 40
                    source: GreenHouseThemeManager.theme.asset("/283_1065.png")
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
                    id: fi_Home_Icon
                    x: 0
                    y: 0
                    z: 1
                    width: 47
                    height: 40
                    source: GreenHouseThemeManager.theme.asset("/283_1069.png")
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
        }
        Item {
            id: fi_Dark_Buttons_Navigation_ECG
            x: 283
            y: 0
            z: 2
            width: 262
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Item {
                id: fi_Dark_Icons_Navigation_ECG
                x: 105
                y: 15
                z: 3
                width: 53
                height: 45
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Item {
                id: fi_ButtonText
                x: 18
                y: 62
                z: 4
                width: 225
                height: 30
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
        }
        Item {
            id: fi_Dark_Buttons_Navigation_Pulse_Oximetry
            x: 555
            y: 0
            z: 3
            width: 262
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Item {
                id: fi_Dark_Icons_Navigation_SpO2
                x: 88
                y: 5
                z: 3
                width: 86
                height: 65
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
        }
        Item {
            id: fi_Dark_Buttons_Navigation_Blood_Pressure
            x: 827
            y: 0
            z: 4
            width: 262
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Item {
                id: fi_Dark_Icons_Navigation_Blood_Pressure
                x: 103
                y: 15
                z: 3
                width: 56
                height: 45
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
        }
        Item {
            id: fi_Dark_Buttons_Navigation_Alarms
            x: 1371
            y: 0
            z: 5
            width: 262
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Item {
                id: fi_Icons
                x: 0
                y: 15
                z: 3
                width: 262
                height: 85
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                Rectangle {
                    id: fi_Selected_Accent
                    x: 0
                    y: 80
                    z: 2
                    width: 262
                    height: 5
                    color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
                    opacity: 1
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
        Item {
            id: fi_Dark_Buttons_Navigation_Call
            x: 1675
            y: 0
            z: 6
            width: 235
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            BorderImage {
                id: fi_Background
                x: 0
                y: 0
                z: 0
                width: 235
                height: 100
                source: GreenHouseThemeManager.theme.asset("/283_1132.png")
                opacity: 1
                enabled: true
                visible: true
                border.bottom: 10
                border.left: 10
                border.right: 10
                border.top: 10
                clip: false
                mirror: false
                rotation: 0
                scale: 1
            }
            Item {
                id: fi_Dark_Icons_Navigation_Call
                x: 98
                y: 15
                z: 2
                width: 40
                height: 45
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
        }
        Item {
            id: fi_Dark_Buttons_Navigation_Patient_Records
            x: 1099
            y: 0
            z: 7
            width: 262
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: true
            rotation: 0
            scale: 1

            Image {
                id: fi_BG
                x: 0
                y: 0
                z: 0
                width: 262
                height: 100
                source: GreenHouseThemeManager.theme.asset("/283_1143.png")
                fillMode: Image.Stretch
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Image {
                id: fi_Border
                x: 0
                y: 0
                z: 1
                width: 262
                height: 100
                source: GreenHouseThemeManager.theme.asset("/283_1144.png")
                fillMode: Image.Stretch
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1
            }
            Rectangle {
                id: fi_Accent
                x: 0
                y: 95
                z: 2
                width: 262
                height: 5
                color: GreenHouseThemeManager.theme.color("Figma::255_153_187_255")
                opacity: 0.4
                border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
                border.width: 1
                enabled: true
                visible: true
                clip: false
                radius: 0
                rotation: 0
                scale: 1
            }
            Item {
                id: fi_Dark_Icons_Navigation_Patient_Records
                x: 113
                y: 15
                z: 3
                width: 35
                height: 46
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                DecorativeSet.DecoratedItem {
                    id: fi_Icon_Blur
                    x: 0
                    y: 5
                    z: 0
                    width: 35
                    height: 41
                    source: GreenHouseThemeManager.theme.asset("/283_1147.png")
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
                    id: fi_Icon
                    x: 0
                    y: 0
                    z: 1
                    width: 35
                    height: 41
                    source: GreenHouseThemeManager.theme.asset("/283_1152.png")
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
            Item {
                id: fi_NavigationButtonText
                x: 17
                y: 62
                z: 4
                width: 225
                height: 30
                opacity: 1
                enabled: true
                visible: true
                clip: false
                rotation: 0
                scale: 1

                Text {
                    id: fi_Text_Label
                    x: -1
                    y: 2
                    z: 1
                    width: 227
                    height: 21
                    text: qsTr("Patient Records")
                    font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Bold_25_75_0_0")
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
                DecorativeSet.BlurredText {
                    id: fi_BlurredText_radius_5
                    x: -1
                    y: 7
                    z: 0
                    width: 227
                    height: 21
                    text: qsTr("Blood Pressure")
                    font: GreenHouseThemeManager.theme.font("Figma::Exo 2_Exo2-Bold_25_75_0_0")
                    color: GreenHouseThemeManager.theme.color("Figma::255_217_236_255")
                    opacity: 0.2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideNone
                    wrapMode: Text.WordWrap
                    enabled: true
                    visible: true
                    blurDeviation: 5.0600000000000005
                    blurRadius: 15
                    clip: false
                    richText: false
                    rotation: 0
                    samples: 48
                    scale: 1
                }
            }
        }
    }

    showAnimation: Alarms_Bottom_Navigation_Bar_PanelLayoutShowAnimation { target: root }
    hideAnimation: Alarms_Bottom_Navigation_Bar_PanelLayoutHideAnimation { target: root }
    initAction: Alarms_Bottom_Navigation_Bar_PanelLayoutInitAnimation { target: root }

    itemInterface: Alarms_Bottom_Navigation_Bar_PanelIFaceImpl {}
}
