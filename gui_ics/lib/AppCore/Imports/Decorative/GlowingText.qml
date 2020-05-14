import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/glowText.png
Item {
    id: root

    width: 64
    height: 64

    //COLOR glowColor
    property color glowColor: "#80ffffff"

    property int horizontalOffset: 0
    property int verticalOffset: 0
    property int samples: 32

    property real glowRadius: 16.0
    property real spread: 0.0

    function _colorText(color) {
        var text = "" + color;
        return text.substring(1, text.length - 1)
    }

    property string __glowAsset: "Decorative/"
                                 + GreenHouseThemeManager.theme.assetHash(
                                     "w" + root.width
                                     + "_h" + root.height
                                     + "_ho" + root.horizontalOffset
                                     + "_vo" + root.verticalOffset
                                     + "_s" + root.samples
                                     + "_sr" + root.glowRadius
                                     + "_sp" + root.spread
                                     + "_gc" + _colorText(root.glowColor)
                                     + "_" + root.text
                                     + "_fF" + root.font.family
                                     + "_fS" + root.font.styleName
                                     + "_fPxs" + root.font.pixelSize
                                     + "_fB" + root.font.bold
                                     + "_fI" + root.font.italic
                                     + "_fSO" + root.font.strikeout
                                     + "_fUL" + root.font.underline
                                     + "_e" + root.elide
                                     + "_wr" + root.wrapMode
                                     + "_hAL" + root.horizontalAlignment
                                     + "_vAL" + root.verticalAlignment
                                     + "_tc" + _colorText(textLabel.color))
                                 + "glowing_text.png"
    property Text __textItem: textLabel

    Image {
        anchors.centerIn: parent

        source: root.__inSync && GreenHouseThemeManager.theme.itemCacher === undefined
                && GreenHouseThemeManager.theme.cacheEffects ? GreenHouseThemeManager.theme.asset(root.__glowAsset)
                                                             : ""
    }

    Loader {
        id: loader

        anchors.fill: parent
    }

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

    property bool __inSync: false
    Component.onCompleted: {
        __inSync = true
        if (GreenHouseThemeManager.theme.itemCacher !== undefined
                || !GreenHouseThemeManager.theme.cacheEffects)
            loader.setSource("Internal/Cached_TextGlow.qml", { source: root })
    }
}
