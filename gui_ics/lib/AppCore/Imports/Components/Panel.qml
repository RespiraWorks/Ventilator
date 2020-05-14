import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/panel.png
Item {
    id: root

    width: 128
    height: 128

    //COLOR panelColor
    property color color: "gray"
    //COLOR alternativePanelColor
    property color alternativeColor: "darkGray"
    property bool alternative: false

    default property alias __data: contents.data
    property int __horizontalPadding: 5
    property int __verticalPadding: 5
    property bool __clipping: contents.clip

    Rectangle {
        anchors.fill: parent
        color: root.alternative ? root.alternativeColor : root.color
    }

    Item {
        id: contents
        anchors.fill: parent

        anchors.leftMargin: root.__horizontalPadding
        anchors.rightMargin: root.__horizontalPadding
        anchors.topMargin: root.__verticalPadding
        anchors.bottomMargin: root.__verticalPadding

        clip: true
    }
}
