import QtQuick 2.0
import GreenHouse.Theming 1.0
import QtGraphicalEffects 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/gradientText.png
Item {
    id: root

    width: 64
    height: 64

    function _colorText(color) {
        var text = "" + color;
        return text.substring(1, text.length - 1)
    }

    property string __gradientAsset: "Decorative/"
                                     + GreenHouseThemeManager.theme.assetHash(
                                         "w_" + root.width
                                         + "_h_" + root.height
                                         + "_scol" + _colorText(root.startColor)
                                         + "_ecol" + _colorText(root.endColor)
                                         + "_" + root.text
                                         + "_fontF_" + root.font.family
                                         + "_fontS_" + root.font.styleName
                                         + "_fontPxs_" + root.font.pixelSize
                                         + "_fontB_" + root.font.bold
                                         + "_fontI_" + root.font.italic
                                         + "_fontSO_" + root.font.strikeout
                                         + "_fontUL_" + root.font.underline)
                                     + "gradient_text.png"
    property Text __sourceItem: textLabel

    Image {
        anchors.centerIn: parent

        source: root.__inSync && GreenHouseThemeManager.theme.itemCacher === undefined
                && GreenHouseThemeManager.theme.cacheEffects ? GreenHouseThemeManager.theme.asset(root.__gradientAsset)
                                                             : ""
    }

    Loader {
        id: loader
        anchors.fill: parent
        width: parent.width
    }

    //DOUBLE startPointX
    property real startPointX: 0
    //DOUBLE startPointY
    property real startPointY: 0
    //DOUBLE endPointX
    property real endPointX: 0
    //DOUBLE endPointY
    property real endPointY: 0
    //COLOR startColor
    property color startColor: "white"
    //COLOR endColor
    property color endColor: "white"

    //TRANSLATABLE
    property string text: "Lorem ipsum"
    //FONT textFont
    property alias font: textLabel.font

    property alias contentHeight: textLabel.contentHeight
    property alias contentWidth: textLabel.contentWidth

    property alias lineHeight: textLabel.lineHeight
    property alias lineHeightMode: textLabel.lineHeightMode
    property alias linkColor: textLabel.linkColor
    property alias maximumLineCount: textLabel.maximumLineCount
    property alias minimumPixelSize: textLabel.minimumPixelSize
    property alias minimumPointSize: textLabel.minimumPointSize
    property alias paintedWidth: textLabel.paintedWidth
    property alias paintedHeight: textLabel.paintedHeight
    property alias renderType: textLabel.renderType
    property bool richText: false

    implicitHeight: textLabel.contentHeight
    implicitWidth: textLabel.contentWidth

    Text {
        id: textLabel

        text: root.text
        color: "black"

        textFormat: root.richText ? Text.RichText : Text.AutoText
        visible: false
    }

    property bool __inSync: false
    Component.onCompleted: {
        __inSync = true
        if (GreenHouseThemeManager.theme.itemCacher !== undefined
                || !GreenHouseThemeManager.theme.cacheEffects)
            loader.setSource("Internal/Cached_Gradient.qml", { source: root })
    }
}
