import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/Label.png
Item {
    id: root

    //TRANSLATABLE
    property string text: "Lorem ipsum"
    //FONT textFont
    property alias font: textLabel.font
    //COLOR textColor
    property color textColor: "white"

    property alias contentHeight: textLabel.contentHeight
    property alias contentWidth: textLabel.contentWidth
    //TEXTELIDEM 3
    property alias elide: textLabel.elide
    //TEXTHAL 4
    property alias horizontalAlignment: textLabel.horizontalAlignment
    property alias lineCount: textLabel.lineCount
    property alias lineHeight: textLabel.lineHeight
    property alias lineHeightMode: textLabel.lineHeightMode
    property alias linkColor: textLabel.linkColor
    property alias maximumLineCount: textLabel.maximumLineCount
    property alias minimumPixelSize: textLabel.minimumPixelSize
    property alias minimumPointSize: textLabel.minimumPointSize
    property alias paintedWidth: textLabel.paintedWidth
    property alias paintedHeight: textLabel.paintedHeight
    property alias renderType: textLabel.renderType
    //TEXTWRAPM 0
    property alias wrapMode: textLabel.wrapMode
    //TEXTVAL 32
    property alias verticalAlignment: textLabel.verticalAlignment
    property bool richText: false

    //NOTE: We can't use implicitHeight for wrapped text as that breaks multi line eliding
    implicitHeight: textLabel.wrapMode === Text.NoWrap ? textLabel.implicitHeight : textLabel.contentHeight
    implicitWidth: textLabel.contentWidth

    Text {
        id: textLabel

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        elide: Text.ElideNone
        wrapMode: Text.NoWrap

        text: root.text
        color: root.textColor

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        textFormat: root.richText ? Text.RichText : Text.AutoText
    }
}
