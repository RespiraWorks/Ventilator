//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

AdjustTidalVLayout {
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
        id: fi_PopupFrame
        x: 199
        y: 141
        z: 1
        width: 626
        height: 377
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
            width: 626
            height: 377
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Title_horizontalAlignment: Text.AlignHCenter
            fi_Title_opacity: 0.85
            fi_Title_text: qsTr("Tidal V (cm)")
            fi_Title_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_Value
            anchors.left: fi_DecrementButton.right
            anchors.leftMargin: 0
            anchors.right: fi_IncrementButton.left
            anchors.rightMargin: 0
            y: 49
            z: 3
            height: 196
            text: VentilatorParametersInterface.tidalVpopup
            font: GreenHouseThemeManager.theme.font("Figma::Noto Serif_NotoSerif-Bold_144_75_0_0")
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
        VentilatorUISet.Button_Minus_Default_State {
            id: fi_DecrementButton
            x: 40
            y: 102
            z: 4
            width: 90
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            icon_opacity: 1
            icon_source: GreenHouseThemeManager.theme.asset("/Icons/Minus__DefaultState.png")
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.set_VentilatorParametersInterface_tidalVpopup(VentilatorParametersInterface.tidalVpopup - VentilatorParametersInterface.tidalVStep)
            }
        }
        VentilatorUISet.Button_Plus_Default_State {
            id: fi_IncrementButton
            x: 496
            y: 102
            z: 5
            width: 90
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            icon_opacity: 1
            icon_source: GreenHouseThemeManager.theme.asset("/Icons/Plus__DefaultState.png")
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.set_VentilatorParametersInterface_tidalVpopup(VentilatorParametersInterface.tidalVpopup + VentilatorParametersInterface.tidalVStep)
            }
        }
        VentilatorUISet.Button_TextPrimary_Default_State {
            id: fi_Confirm
            x: 336
            y: 247
            z: 1
            width: 250
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignHCenter
            fi_Label_opacity: 1
            fi_Label_text: qsTr("Confirm")
            fi_Label_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.call_VentilatorParametersInterface_adjustTidalV(VentilatorParametersInterface.tidalVpopup)
                root.itemInterface.moveToNoPopup()
            }
        }
        VentilatorUISet.Button_TextSecondary_Default_State {
            id: fi_Cancel
            x: 40
            y: 247
            z: 2
            width: 250
            height: 90
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignHCenter
            fi_Label_opacity: 1
            fi_Label_text: qsTr("Cancel")
            fi_Label_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.moveToNoPopup()
            }
        }
    }

    showAnimation: AdjustTidalVLayoutShowAnimation { target: root }
    hideAnimation: AdjustTidalVLayoutHideAnimation { target: root }
    initAction: AdjustTidalVLayoutInitAnimation { target: root }

    itemInterface: AdjustTidalVIFaceImpl {}
}
