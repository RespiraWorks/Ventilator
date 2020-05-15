import QtQuick 2.0
import QtQuick.Window 2.2

import "GlobalPassThroguh.js" as GlobalPassThroguh

Item {
    id: root

    signal pressedAt(real posX, real posY)
    signal hidden()

    function _topLevelParent(itemParent) {
        if (Window.contentItem)
            return Window.contentItem;
        var topLevelParent = itemParent;
        while (topLevelParent && topLevelParent.parent)
            topLevelParent = topLevelParent.parent
        return topLevelParent;
    }

    function _startListening() {
        if (GlobalPassThroguh.mouseArea === null) {
            var topLevelParent = root._topLevelParent(root.parent);
            if (topLevelParent)
                GlobalPassThroguh.mouseArea = globalPassThroughMouseAreaCmp.createObject(topLevelParent);
        }
        __globalPassThroughMouseArea = GlobalPassThroguh.mouseArea;
        if (trackInput)
            __globalPassThroughMouseArea.trackingItem = root;
    }

    function _stopListening() {
        if (__globalPassThroughMouseArea && __globalPassThroughMouseArea.trackingItem === root)
            __globalPassThroughMouseArea.trackingItem = null;
        __globalPassThroughMouseArea = null;
    }

    property bool trackInput: false
    onTrackInputChanged: {
        if (trackInput) {
            _startListening()
        } else {
            _stopListening();
        }
    }

    Component {
        id: globalPassThroughMouseAreaCmp

        MouseArea {
            anchors.fill: parent
            z: 2147483647
            propagateComposedEvents: true
            onPressed: mouse.accepted = false
            Component.onDestruction: GlobalPassThroguh.mouseArea = null

            enabled: trackingItem
            property Item trackingItem: null
        }
    }

    property MouseArea __globalPassThroughMouseArea: null
    onVisibleChanged: {
        if (!visible) {
            root.hidden();
            _stopListening();
        } else if (trackInput) {
            _startListening();
        }
    }

    Connections {
        target: root.__globalPassThroughMouseArea
        onPressed: {
            // NOTE: Not using the mouse event parameter to maintain code compatibility between Qt 5.6 and Qt 5.15+
            var pressPos = root.__globalPassThroughMouseArea.mapToItem(root,
                                                                       root.__globalPassThroughMouseArea.mouseX,
                                                                       root.__globalPassThroughMouseArea.mouseY)
            root.pressedAt(pressPos.x, pressPos.y)
        }
    }

    Component.onCompleted: {
        if (trackInput)
            root._startListening()
    }
    Component.onDestruction: root._stopListening()
}
