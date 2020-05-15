//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

Background_Shared0Layout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 0
    y: fillParent ? 0 : 0
    width: fillParent ? (parent ? parent.width : 0) : 1024
    height: fillParent ? (parent ? parent.height : 0) : 600

    inputFallthroughBlocked: true

    Image {
        id: fi_Fill
        x: 0
        y: 0
        z: 0
        width: 1024
        height: 600
        source: GreenHouseThemeManager.theme.asset("/233_1125.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: true
        rotation: 0
        scale: 1
    }

    showAnimation: Background_Shared0LayoutShowAnimation { target: root }
    hideAnimation: Background_Shared0LayoutHideAnimation { target: root }
    initAction: Background_Shared0LayoutInitAnimation { target: root }

    itemInterface: Background_Shared0IFaceImpl {}
}
