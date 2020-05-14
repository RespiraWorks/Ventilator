import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/listItem.png
Item {
    id: root

    width: 256
    height: 64

    //COLOR
    property color backgroundColor: "transparent"
    property color pressedColor: "transparent"
    Rectangle {
        id: background

        anchors.fill: parent

        visible: index % 2 === 0
        color: ma.pressed ? root.pressedColor : root.backgroundColor
    }

    //COLOR
    property color alternateBackgroundColor: "transparent"
    property color alternatePressedColor: "transparent"
    Rectangle {
        id: alternateBackground

        anchors.fill: parent

        visible: index % 2 !== 0
        color: ma.pressed ? root.alternatePressedColor : root.alternateBackgroundColor
    }

    //BOOL true
    property alias showIndexLabel: indexLabel.visible
    property int indexLabelMargin: 5
    property string indexLabelPostfix: ""
    //FONT textFont
    property alias indexLabelFont: indexLabel.font
    //COLOR textColor
    property color indexLabelColor: "white"
    Text {
        id: indexLabel

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: root.indexLabelMargin

        text: index + 1 + root.indexLabelPostfix
        color: root.indexLabelColor
    }

    property int textLeftMargin: 5
    property int textRightMargin: 5
    //TRANSLATABLE
    property string text: ""
    //FONT textFont
    property alias font: textLabel.font
    //COLOR textColor
    property color textColor: "white"
    //TEXTELIDEM 3
    property alias elide: textLabel.elide
    //TEXTHAL 4
    property alias horizontalAlignment: textLabel.horizontalAlignment
    //TEXTWRAPM 0
    property alias wrapMode: textLabel.wrapMode
    //TEXTVAL 32
    property alias verticalAlignment: textLabel.verticalAlignment
    property bool richText: false
    Text {
        id: textLabel

        anchors.left: indexLabel.visible ? indexLabel.right : parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: root.textLeftMargin
        anchors.rightMargin: root.textRightMargin

        elide: Text.ElideNone
        wrapMode: Text.NoWrap

        text: root.text
        color: root.textColor

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        textFormat: root.richText ? Text.RichText : Text.AutoText
    }

    signal clicked()
    MouseArea {
        id: ma
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
