import QtQuick 2.0
import QtGraphicalEffects 1.0
import GreenHouse.Theming 1.0

Item {
    id: root

    property Item source

    anchors.fill: parent

    GaussianBlur {
        id: blure

        anchors.centerIn: parent
        width: root.source ? root.source.width : 0
        height: root.source ? root.source.height : 0

        samples: root.source ? root.source.samples : 0

        radius: root.source ? root.source.blurRadius : 0.0
        deviation: root.source ? root.source.blurDeviation : 0.0

        source: root.source ? root.source.__sourceItem : null
        transparentBorder: true
    }

    property string cacheName: root.source ? root.source.__blurAsset : ""
    onCacheNameChanged: {
        if (cacheName !== "" && GreenHouseThemeManager.theme.cacheEffects)
            cacheTimer.start()
    }
    Timer {
        id: cacheTimer
        repeat: false
        interval: 500
        onTriggered: {
            if (root.cacheName !== "")
                GreenHouseThemeManager.theme.itemCacher.cacheItem(root, root.cacheName)
        }
    }
}
