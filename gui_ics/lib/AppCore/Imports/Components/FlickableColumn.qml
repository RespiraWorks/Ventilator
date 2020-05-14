import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/column.png
//CONTAINER Column
//CONTENT MOVABLE
Flickable {
    id: root

    width: 40
    height: 120

    property bool stopAtBounds: false
    //INT 0
    property alias spacing: column.spacing

    contentWidth: width
    contentHeight: column.height
    boundsBehavior: stopAtBounds ? Flickable.StopAtBounds : Flickable.DragAndOvershootBounds
    clip: true

    default property alias columnContents: column.data
    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
