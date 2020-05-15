//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

SettingPlaceholder_MainViewPanelLayout {
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
            fi_Title_text: qsTr("Settings")
            fi_Title_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
    }

    showAnimation: SettingPlaceholder_MainViewPanelLayoutShowAnimation { target: root }
    hideAnimation: SettingPlaceholder_MainViewPanelLayoutHideAnimation { target: root }
    initAction: SettingPlaceholder_MainViewPanelLayoutInitAnimation { target: root }

    itemInterface: SettingPlaceholder_MainViewPanelIFaceImpl {}
}
