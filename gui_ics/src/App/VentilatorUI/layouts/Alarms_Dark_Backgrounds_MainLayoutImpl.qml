//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

Alarms_Dark_Backgrounds_MainLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 0
    y: fillParent ? 0 : 0
    width: fillParent ? (parent ? parent.width : 0) : 1920
    height: fillParent ? (parent ? parent.height : 0) : 1080

    inputFallthroughBlocked: true

    Image {
        id: fi_BG
        x: 0
        y: 0
        z: 0
        width: 1920
        height: 1080
        source: GreenHouseThemeManager.theme.asset("/38_385.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }

    showAnimation: Alarms_Dark_Backgrounds_MainLayoutShowAnimation { target: root }
    hideAnimation: Alarms_Dark_Backgrounds_MainLayoutHideAnimation { target: root }
    initAction: Alarms_Dark_Backgrounds_MainLayoutInitAnimation { target: root }

    itemInterface: Alarms_Dark_Backgrounds_MainIFaceImpl {}
}
