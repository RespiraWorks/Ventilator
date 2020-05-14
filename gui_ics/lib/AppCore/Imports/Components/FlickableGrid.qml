import QtQuick 2.6

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/grid.png
//CONTAINER Grid
//CONTENT MOVABLE
Flickable {
    id: root

    width: 120
    height: 120

    property bool stopAtBounds: false

    //INT 4
    property alias columns: grid.columns
    //INT 0
    property alias columnSpacing: grid.columnSpacing
    //INT 0
    property alias rows: grid.rows
    //INT 0
    property alias rowSpacing: grid.rowSpacing

    //REAL 0.0
    property alias leftPadding: grid.leftPadding
    //REAL 0.0
    property alias rightPadding: grid.rightPadding
    //REAL 0.0
    property alias topPadding: grid.topPadding
    //REAL 0.0
    property alias bottomPadding: grid.bottomPadding

    //LAYOUTDIR 0
    property alias layoutDirection: grid.layoutDirection
    //GRIDHAL 1
    property alias horizontalItemAlignment: grid.horizontalItemAlignment
    //GRIDVAL 32
    property alias verticalItemAlignment: grid.verticalItemAlignment

    contentWidth: grid.width
    contentHeight: grid.height
    boundsBehavior: stopAtBounds ? Flickable.StopAtBounds : Flickable.DragAndOvershootBounds
    clip: true

    default property alias gridContents: grid.data
    Grid {
        id: grid
    }
}
