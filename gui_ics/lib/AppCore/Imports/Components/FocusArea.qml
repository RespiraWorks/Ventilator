import QtQuick 2.0

import "Internal"

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/mouse.png
MouseArea {
    id: root

    property bool focused: false
    onClicked: focused = !focused

    //HIDE property z
    z: -2147483647

    width: 50
    height: 50

    ExternalInputHandler {
        id: focusHandler

        anchors.fill: parent

        trackInput: root.focused
        onHidden: root.focused = false
        onPressedAt: {
            if (root.parent) {
                var parentPos = focusHandler.mapToItem(root.parent, posX, posY);
                if (parentPos.x < root.parent.childrenRect.x
                        || parentPos.x > (root.parent.childrenRect.x + root.parent.childrenRect.width)
                        || parentPos.y < root.parent.childrenRect.y
                        || parentPos.y > (root.parent.childrenRect.y + root.parent.childrenRect.height))
                    root.focused = false
            } else  if (posX < 0 || posX > width || posY < 0 || posY > height) {
                root.focused = false
            }
        }
    }

    Connections {
        target: root.parent
        ignoreUnknownSignals: true
        onSelectedIndexChanged: { root.__removeFocus() }
        onSelectionMade: { root.__removeFocus() }
    }

    function __removeFocus() {
        root.focused = false
    }
    function __connectTo(item) {
        if (item && item.children !== undefined) {
            var children = item.children;
            for (var i = 0; i < children.length; ++i) {
                if (children[i] !== root) {
                    if (children[i].released !== undefined) {
                        children[i].released.connect(__removeFocus)
                    } else {
                        __connectTo(children[i])
                    }
                }
            }
        }
    }
    function __makeConnections() {
        if (root.parent && root.parent.selectedIndexChanged === undefined
                && root.parent.selectionMade === undefined) {
            __connectTo(root.parent)
        }
    }
    Component.onCompleted: __makeConnections()
    onParentChanged: __makeConnections()
}
