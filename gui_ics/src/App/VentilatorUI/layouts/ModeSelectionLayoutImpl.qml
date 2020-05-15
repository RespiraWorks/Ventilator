//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

ModeSelectionLayout {
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
        id: fi_PopUpModeSelection
        x: 112
        y: 95
        z: 1
        width: 800
        height: 470
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
            width: 800
            height: 470
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Title_horizontalAlignment: Text.AlignHCenter
            fi_Title_opacity: 0.85
            fi_Title_text: qsTr("Mode Selection")
            fi_Title_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
        VentilatorUISet.Button_TextPrimary_Default_State {
            id: fi_Mode1
            x: 56
            y: 86
            z: 1
            width: 688
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignHCenter
            fi_Label_opacity: 1
            fi_Label_text: qsTr("Assist Control Pressure (AC/P)")
            fi_Label_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.call_VentilatorParametersInterface_setPressureMode(0)
                root.itemInterface.moveToNoPopup()
            }
        }
        VentilatorUISet.Button_TextPrimary_Default_State {
            id: fi_Mode2
            x: 56
            y: 206
            z: 2
            width: 688
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignHCenter
            fi_Label_opacity: 1
            fi_Label_text: qsTr("Pressure Command Ventillation")
            fi_Label_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.call_VentilatorParametersInterface_setPressureMode(1)
                root.itemInterface.moveToNoPopup()
            }
        }
        VentilatorUISet.Button_TextPrimary_Default_State {
            id: fi_Mode3
            x: 56
            y: 326
            z: 3
            width: 688
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignHCenter
            fi_Label_opacity: 1
            fi_Label_text: qsTr("Continuous Positive Pressure Mode")
            fi_Label_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.call_VentilatorParametersInterface_setPressureMode(2)
                root.itemInterface.moveToNoPopup()
            }
        }
    }

    showAnimation: ModeSelectionLayoutShowAnimation { target: root }
    hideAnimation: ModeSelectionLayoutHideAnimation { target: root }
    initAction: ModeSelectionLayoutInitAnimation { target: root }

    itemInterface: ModeSelectionIFaceImpl {}
}
