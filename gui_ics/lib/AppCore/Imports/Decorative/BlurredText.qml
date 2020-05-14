import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/bluredText.png
Item {
    id: root

    width: 64
    height: 64

    property int samples: 32

    property real blurRadius: 16.0
    property real blurDeviation: 1.0

    property string __blurAsset: "Decorative/"
                                 + GreenHouseThemeManager.theme.assetHash(
                                     "w_" + root.width
                                     + "_h_" + root.height
                                     + "_s_" + root.samples
                                     + "_br_" + root.blurRadius
                                     + "_bd_" + root.blurDeviation
                                     + "_" + root.text
                                     + "_fontF_" + root.font.family
                                     + "_fontS_" + root.font.styleName
                                     + "_fontPxs_" + root.font.pixelSize
                                     + "_fontB_" + root.font.bold
                                     + "_fontI_" + root.font.italic
                                     + "_fontSO_" + root.font.strikeout
                                     + "_fontUL_" + root.font.underline
                                     + "_elide_" + root.elide
                                     + "_wrap_" + root.wrapMode
                                     + "_hAL_" + root.horizontalAlignment
                                     + "_vAL_" + root.verticalAlignment
                                     + "_tca_" + Math.floor(textLabel.color.a * 255)
                                     + "_tcr_" + Math.floor(textLabel.color.r * 255)
                                     + "_tcg_" + Math.floor(textLabel.color.g * 255)
                                     + "_tcb_" + Math.floor(textLabel.color.b * 255))
                                 + "blur_text.png"
    property Text __sourceItem: textLabel

    Image {
        anchors.centerIn: parent

        source: root.__inSync && GreenHouseThemeManager.theme.itemCacher === undefined
                && GreenHouseThemeManager.theme.cacheEffects ? GreenHouseThemeManager.theme.asset(root.__blurAsset)
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
    property color color: "white"

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
        color: root.color

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        textFormat: root.richText ? Text.RichText : Text.AutoText
        visible: false
    }

    property bool __inSync: false
    Component.onCompleted: {
        __inSync = true
        if (GreenHouseThemeManager.theme.itemCacher !== undefined
                || !GreenHouseThemeManager.theme.cacheEffects)
            loader.setSource("Internal/Cached_Blur.qml", { source: root })
    }
}
