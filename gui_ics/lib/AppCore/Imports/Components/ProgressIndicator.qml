import QtQuick 2.6

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/progressIndicator.png
Item {
    id: root

    width: 128
    height: 16

    property int min: 0
    property int max: 100
    property int value: 25

    property int leftBackgroundAssetBorder: 2
    property int rightBackgroundAssetBorder: 2
    property int topBackgroundAssetBorder: 2
    property int bottomBackgroundAssetBorder: 2
    //ASSET /placeholders/progress/background.png
    property url backgroundSource: ""

    BorderImage {
        id: background

        anchors.fill: parent

        border.left: root.leftBackgroundAssetBorder
        border.right: root.rightBackgroundAssetBorder
        border.top: root.topBackgroundAssetBorder
        border.bottom: root.bottomBackgroundAssetBorder
        source: root.backgroundSource
    }

    property int leftIndicatorAssetBorder: 2
    property int rightIndicatorAssetBorder: 2
    property int topIndicatorAssetBorder: 2
    property int bottomIndicatorAssetBorder: 2
    property int indicatorAssetHorizontalOverflows: 0
    //INT
    property int fillMargin: 0
    //ASSET /placeholders/progress/fill.png
    property url indicatorSource: ""

    Item {
        property alias fillAnchors: fillImage.anchors
        id: indicator
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: background.width * Math.min(1.0 ,((root.value - root.min)/Math.max(1, root.max - root.min)))

        BorderImage {
            id: fillImage
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.top: parent.top
            anchors.margins: root.fillMargin - root.indicatorAssetHorizontalOverflows

            border.left: root.leftIndicatorAssetBorder + root.indicatorAssetHorizontalOverflows
            border.right: root.rightIndicatorAssetBorder + root.indicatorAssetHorizontalOverflows
            border.top: root.topIndicatorAssetBorder
            border.bottom: root.bottomIndicatorAssetBorder
            source: root.indicatorSource
        }
    }
    //BOOL false
    property alias labelVisible: label.visible
    //COLOR textColor
    property alias textColor: label.color
    //FONT textFont
    property alias textFont: label.font
    property bool parentBackground: false
    property bool parentIndicator: false
    property bool labelAnchorTop: false
    property bool labelAnchorVCenter: false
    property bool labelAnchorHCenter: false
    property bool labelAnchorBottom: false
    property bool labelAnchorLeft: false
    property bool labelAnchorRight: false

    //TRANSLATABLE
    property string valuePrefix: ""
    //TRANSLATABLE
    property string valueSuffix: ""

    Text {
        id: label
        anchors.bottom: root.labelAnchorTop ? parent.top : undefined
        anchors.top: root.labelAnchorBottom ? parent.bottom : undefined
        anchors.right: root.labelAnchorRight ? parent.left : undefined
        anchors.left: root.labelAnchorLeft ? parent.right : undefined
        anchors.verticalCenter: root.labelAnchorVCenter ? parent.verticalCenter : undefined
        anchors.horizontalCenter: root.labelAnchorHCenter ? parent.horizontalCenter : undefined
        opacity: {
            if ((root.labelAnchorTop && root.labelAnchorBottom && root.labelAnchorHCenter)
                    || (root.labelAnchorRight && root.labelAnchorLeft && root.labelAnchorVCenter)
                    || (root.labelAnchorHCenter && root.labelAnchorVCenter)) {
                if (parent.width < fm.advanceWidth(label.text) || parent.height < fm.height)
                    return 0.0
            }
            return 1.0
        }
        text: root.valuePrefix + root.value + root.valueSuffix
        visible: false
        parent: {
            if (root.parentBackground)
                return background
            else if (root.parentIndicator)
                return indicator
            return root
        }
    }

    FontMetrics {
        id: fm
        font: label.font
    }
}
