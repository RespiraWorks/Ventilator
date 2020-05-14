import QtQuick 2.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/dropShadowRect.png
Item {
    id: root

    width: 64
    height: 64

    property int radius: 0

    //COLOR shadowColor
    property alias shadowColor: shadow.shadowColor
    //INT 4
    property alias horizontalOffset: shadow.horizontalOffset
    //INT 4
    property alias verticalOffset: shadow.verticalOffset
    //INT 32
    property alias samples: shadow.samples
    //REAL 4.0
    property alias shadowRadius: shadow.shadowRadius
    //REAL 0.0
    property alias spread: shadow.spread
    RectangularShadow {
        id: shadow

        anchors.fill: parent

        radius: root.radius
    }

    //COLOR shadowRectColor
    property alias color: rectangle.color
    //COLOR shadowRectColor
    property color borderColor: "#ff0000"
    property int borderWidth: 0
    Rectangle {
        id: rectangle

        anchors.fill: parent

        radius: root.radius
        color: "#ff0000"
        border.color: root.borderColor
        border.width: root.borderWidth
    }
}
