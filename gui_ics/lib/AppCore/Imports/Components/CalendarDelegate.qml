import QtQuick 2.0

Rectangle {
    id: root
    property string fi_Label_text: "01"
    property bool triggered: false
    signal clicked

    width: 50
    height: 50

    border.color: "black"
    //HIDE property color
    color: triggered ? "lightblue" : "white"

    Text {
        id: delegateText
        anchors.centerIn: parent
        font.bold: triggered
        text: root.fi_Label_text
    }
}
