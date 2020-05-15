import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/carousel.png
//PARENTING FALSE
PathView {
    id: root

    //MODEL
    property alias model: repeater.model
    //DELEGATE
    property alias delegate: repeater.delegate
    //INT 0
    property alias selectedIndex: pathView.currentIndex

    property real offCenterScale: 0.3
    property real offCenterOpacity: 0.5
    property real delegateEdgeSize: 0.5
    property real centerX: 0.5
    property real centerY: 0.5
    property real rightX: 1.0
    property real rightY: 0.2
    property real leftX: 0.0
    property real leftY: 0.2

    signal indexSelected(int index)

    width: 240
    height: 200

    Item {
        id: delegateStash
        visible: false

        Repeater {
            id: repeater
            delegate: Rectangle { anchors.fill: parent }
        }
    }

    PathView {
        id: pathView

        onCurrentIndexChanged: root.indexSelected(currentIndex)
        anchors.fill: parent

        snapMode: PathView.SnapToItem
        highlightRangeMode: PathView.StrictlyEnforceRange

        property int __delegateEdgeSize: Math.min(root.width * root.delegateEdgeSize,
                                                  root.height * root.delegateEdgeSize)
        preferredHighlightBegin: (width - __delegateEdgeSize) * 0.5
        preferredHighlightEnd: preferredHighlightBegin + __delegateEdgeSize
        dragMargin: width * 0.2
        highlightMoveDuration: 200
        model: repeater.count

        delegate: Item {
            id: delegateRoot

            width: pathView.__delegateEdgeSize
            height: width

            scale: PathView.delegateScale
            opacity: PathView.delegateOpacity

            z: PathView.delegateZ

            function __sync() {
                var item = repeater.itemAt(index)
                if (item)
                    item.parent = delegateRoot
            }

            Connections {
                target: repeater
                onItemAdded: { delegateRoot.__sync() }
            }
            Component.onCompleted: __sync()
        }

        path: Path {
            startX: root.width * root.centerX
            startY: root.height * root.centerY
            PathAttribute { name: "delegateScale"; value: 1.0 }
            PathAttribute { name: "delegateOpacity"; value: 1.0 }
            PathAttribute { name: "delegateZ"; value: 2.0 }
            PathCurve { x: root.width * root.rightX; y: root.height * root.rightY;}
            PathAttribute { name: "delegateScale"; value: root.offCenterScale }
            PathAttribute { name: "delegateOpacity"; value: root.offCenterOpacity }
            PathAttribute { name: "delegateZ"; value: 0.0 }
            PathCurve { x: root.width * root.leftX; y: root.height * root.leftY;}
            PathAttribute { name: "delegateScale"; value: root.offCenterScale }
            PathAttribute { name: "delegateOpacity"; value: root.offCenterOpacity }
            PathAttribute { name: "delegateZ"; value: 0.0 }
            PathCurve { x: root.width * root.centerX; y: root.height * root.centerY;}
            PathAttribute { name: "delegateScale"; value: 1.0 }
            PathAttribute { name: "delegateOpacity"; value: 1.0 }
            PathAttribute { name: "delegateZ"; value: 2.0 }
        }
    }
}
