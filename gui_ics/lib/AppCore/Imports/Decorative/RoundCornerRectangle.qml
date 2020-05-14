import QtQuick 2.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/rounded-rectangle.png
Item {
    id: root

    width: 64
    height: 64

    property int topLeftRadius: 10
    property int topRightRadius: 0
    property int bottomLeftRadius: 0
    property int bottomRightRadius: 10

    //COLOR shadowRectColor
    property color color: "#ff0000"
    smooth: true

    Rectangle {
        id: topLeft

        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.verticalCenter

        radius: root.topLeftRadius

        color: root.color
    }

    Rectangle {
        id: topRight

        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.verticalCenter

        radius: root.topRightRadius

        color: root.color
    }

    Rectangle {
        id: bottomLeft

        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.top: parent.verticalCenter
        anchors.bottom: parent.bottom

        radius: root.bottomLeftRadius

        color: root.color
    }

    Rectangle {
        id: bottomRight

        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: parent.verticalCenter
        anchors.bottom: parent.bottom

        radius: root.bottomRightRadius

        color: root.color
    }

    Rectangle {
        id: verticalBar

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: Math.max(root.topLeftRadius,
                        Math.max(root.topRightRadius,
                                 Math.max(root.bottomLeftRadius, root.bottomRightRadius))) * 2

        color: root.color
    }

    Rectangle {
        id: horizontalBar

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        height: verticalBar.width
        color: root.color
    }
}
