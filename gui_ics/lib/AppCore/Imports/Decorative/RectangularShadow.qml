import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/dropShadow.png
Item {
    id: root

    width: 64
    height: 64

    //COLOR shadowColor
    property color shadowColor: "#80202020"

    property int radius: 0
    property int horizontalOffset: 4
    property int verticalOffset: 4
    property int samples: 32

    property real shadowRadius: 4.0
    property real spread: 0.0

    property string __shadowAssetName: "Decorative/rect_shadow_"
                                       + GreenHouseThemeManager.theme.assetHash(
                                           root.radius
                                           + "_w_" + root.width
                                           + "_h_" + root.height
                                           + "_ho_" + root.horizontalOffset
                                           + "_vo_" + root.verticalOffset
                                           + "_s_" + root.samples
                                           + "_sr_" + root.shadowRadius
                                           + "_sp_" + root.spread
                                           + "_sca_" + Math.floor(root.shadowColor.a * 255)
                                           + "_scr_" + Math.floor(root.shadowColor.r * 255)
                                           + "_scg_" + Math.floor(root.shadowColor.g * 255)
                                           + "_scb_" + Math.floor(root.shadowColor.b * 255))
                                       + ".png"

    Image {
        anchors.centerIn: parent

        source: root.__inSync && GreenHouseThemeManager.theme.itemCacher === undefined
                && GreenHouseThemeManager.theme.cacheEffects ? GreenHouseThemeManager.theme.asset(root.__shadowAssetName)
                                                             : ""
    }

    Loader {
        id: loader

        anchors.fill: parent
    }

    property bool __inSync: false
    Component.onCompleted: {
        __inSync = true
        if (GreenHouseThemeManager.theme.itemCacher !== undefined
                || !GreenHouseThemeManager.theme.cacheEffects)
            loader.setSource("Internal/Cached_DropShadowRect.qml", { source: root })
    }
}
