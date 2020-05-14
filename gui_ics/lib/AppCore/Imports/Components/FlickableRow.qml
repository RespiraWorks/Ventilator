import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/row.png
//CONTAINER Row
//CONTENT MOVABLE
Flickable {
    id: root

    width: 120
    height: 40

    property bool stopAtBounds: false
    //INT 0
    property alias spacing: row.spacing
    //LAYOUTDIR 0
    property alias layoutDirection: row.layoutDirection

    contentWidth: row.width
    contentHeight: height
    boundsBehavior: stopAtBounds ? Flickable.StopAtBounds : Flickable.DragAndOvershootBounds
    clip: true

    default property alias rowContents: row.data
    Row {
        id: row
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
}
