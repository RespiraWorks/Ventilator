import QtQuick 2.0
import QtGraphicalEffects 1.0
import GreenHouse.Theming 1.0

Item {
    id: root

    property Item source

    LinearGradient {
        id: gradient

        anchors.horizontalCenter: parent.horizontalCenter
        width: root.source ? root.source.__sourceItem.contentWidth : 0
        height: root.source ? root.source.__sourceItem.height : 0

        source: root.source ? root.source.__sourceItem : null
        start: {
            if(!root.source)
                return Qt.point(0, 0)
            var x = root.source.startPointX
            var y = root.source.startPointY
            return Qt.point(x * gradient.width, y * root.height)
        }
        end: {
            if(!root.source)
                return Qt.point(0, 0)
            var x = root.source.endPointX
            var y = root.source.endPointY
            return Qt.point(x * gradient.width, y * root.height)
        }
        gradient: Gradient {
            GradientStop { position: 0; color: root.source ? root.source.startColor : "white" }
            GradientStop { position: 1; color: root.source ? root.source.endColor : "white" }
        }
    }

    property string cacheName: root.source ? root.source.__gradientAsset : ""
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
                GreenHouseThemeManager.theme.itemCacher.cacheItem(gradient, root.cacheName)
        }
    }
}
