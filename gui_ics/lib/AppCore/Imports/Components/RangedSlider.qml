import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/rangedSlider.png
Item {
    id: root

    width: 128
    height: 22

    signal minValueChangeRequested(int minValue)
    signal maxValueChangeRequested(int maxValue)

    property int min: 0
    property int max: 100
    property int minValue: 25
    property int maxValue: 75

    property int leftBackgroundAssetBorder: 0
    property int rightBackgroundAssetBorder: 0
    property int topBackgroundAssetBorder: 0
    property int bottomBackgroundAssetBorder: 0

    property int leftFillAssetBorder: 0
    property int rightFillAssetBorder: 0
    property int topFillAssetBorder: 0
    property int bottomFillAssetBorder: 0
    property int fillMargin: 0

    //ASSET /placeholders/slider/background.png
    property url backgroundSource: ""
    //ASSET /placeholders/slider/knob.png
    property url knobMinSource: ""
    //ASSET /placeholders/slider/knob.png
    property url knobMaxSource: ""
    //ASSET
    property url fillSource: ""
    //ASSET
    property url bubbleMinSource: ""
    //ASSET
    property url bubbleMaxSource: ""

    property bool knobMinInset: false
    property bool knobMaxInset: false

    property bool enableMinValueDisplay: false
    property bool bubbleMinAnchorTop: false
    property bool bubbleMinAnchorCenter: false
    property bool bubbleMinAnchorBottom: false
    property bool bubbleMinAnchorLeft: false
    property bool bubbleMinAnchorRight: false

    property bool enableMaxValueDisplay: false
    property bool bubbleMaxAnchorTop: false
    property bool bubbleMaxAnchorCenter: false
    property bool bubbleMaxAnchorBottom: false
    property bool bubbleMaxAnchorLeft: false
    property bool bubbleMaxAnchorRight: false

    //COLOR textColor
    property alias textMinColor: labelMin.color
    //COLOR textColor
    property alias textMaxColor: labelMax.color

    //FONT textFont
    property alias textMinFont: labelMin.font
    //FONT textFont
    property alias textMaxFont: labelMax.font

    property bool __vertical: (width < height)

    BorderImage {
        id: background
        border.left: root.leftBackgroundAssetBorder
        border.right: root.rightBackgroundAssetBorder
        border.top: root.topBackgroundAssetBorder
        border.bottom: root.bottomBackgroundAssetBorder
        anchors.left: !root.__vertical ? parent.left : undefined
        anchors.right: !root.__vertical ? parent.right : undefined
        anchors.verticalCenter: !root.__vertical ? parent.verticalCenter : undefined
        anchors.bottom: !root.__vertical ? undefined : parent.bottom
        anchors.top: !root.__vertical ? undefined : parent.top
        anchors.horizontalCenter: !root.__vertical ? undefined : parent.horizontalCenter
        source: root.backgroundSource
    }

    BorderImage {
        id: fillImage

        anchors.left: !root.__vertical ? knobMin.horizontalCenter : background.left
        anchors.right: !root.__vertical ? knobMax.horizontalCenter : background.right
        anchors.top: !root.__vertical ? background.top : knobMax.verticalCenter
        anchors.bottom: !root.__vertical ? background.bottom : knobMin.verticalCenter

        border.left: root.leftFillAssetBorder
        border.right: root.rightFillAssetBorder
        border.top: root.topFillAssetBorder
        border.bottom: root.bottomFillAssetBorder
        anchors.margins: root.fillMargin

        source: root.fillSource
    }

    Image {
        id: knobMin

        property int adjWidth: root.knobMinInset ? -root.fillMargin : width * 0.5
        property int adjHeight: root.knobMinInset ? -root.fillMargin : height * 0.5

        anchors.verticalCenter: !root.__vertical ? parent.verticalCenter : undefined
        anchors.horizontalCenter: !root.__vertical ? undefined : parent.horizontalCenter

        x: {
            if (!root.__vertical && !knobMinMa.drag.active)
                return internal.xOfValue(root.minValue, root.width, adjWidth)
            return 0;
        }
        y: {
            if (root.__vertical && !knobMinMa.drag.active)
                return internal.yOfValue(root.minValue, root.height, adjHeight)
            return 0;
        }
        z: 99
        height: !root.__vertical ? sourceSize.height : sourceSize.width
        width:  !root.__vertical ? sourceSize.width : sourceSize.height

        source: root.knobMinSource

        MouseArea {
            id: knobMinMa

            property real handleCenter: !root.__vertical ? knobMin.width * 0.5 : knobMin.height * 0.5

            anchors.fill: parent

            drag.target: pressed ? knobMin : undefined
            drag.axis: root.__vertical ? Drag.YAxis : Drag.XAxis
            drag.minimumX: root.knobMinInset ? root.fillMargin : - (knobMin.width * 0.5)
            drag.maximumX: {
                if (root.knobMinInset)
                    return knobMax.x - (root.fillMargin + knobMin.width)
                else
                    return knobMax.x - (knobMin.width * 0.5)
            }
            drag.minimumY: root.knobMinInset ? root.fillMargin : - (knobMin.height * 0.5)
            drag.maximumY: {
                if (root.knobMinInset)
                    return knobMax.y - (root.fillMargin + knobMin.height)
                else
                    return knobMax.y - (knobMin.height * 0.5)
            }
            onReleased: root.minValueChangeRequested(internal.minValue)
        }
    }

    Image {
        id: labelHolderMin

        anchors.left: root.bubbleMinAnchorLeft ? knobMin.right : undefined
        anchors.horizontalCenter: !root.__vertical ? knobMin.horizontalCenter
                                                   : (root.bubbleMinAnchorCenter ? knobMin.horizontalCenter : undefined)
        anchors.right: root.bubbleMinAnchorRight ? knobMin.left : undefined
        anchors.top: root.bubbleMinAnchorBottom ? knobMin.bottom : undefined
        anchors.verticalCenter: root.__vertical ? knobMin.verticalCenter
                                                : (root.bubbleMinAnchorCenter ? knobMin.verticalCenter : undefined)
        anchors.bottom: root.bubbleMinAnchorTop ? knobMin.top : undefined

        source: root.bubbleMinSource
        visible: root.enableMinValueDisplay && knobMinMa.pressed
    }

    Text {
        id: labelMin
        anchors.centerIn: parent
        parent: labelHolderMin
        text: internal.minValue
    }

    Image {
        id: knobMax

        property int adjWidth: root.knobMaxInset ? -root.fillMargin : width * 0.5
        property int adjHeight: root.knobMaxInset ? -root.fillMargin : height * 0.5

        anchors.verticalCenter: !root.__vertical ? parent.verticalCenter : undefined
        anchors.horizontalCenter: !root.__vertical ? undefined : parent.horizontalCenter

        height: !root.__vertical ? sourceSize.height : sourceSize.width
        width:  !root.__vertical ? sourceSize.width : sourceSize.height

        x: {
            if (!root.__vertical && !knobMaxMa.drag.active)
                return internal.xOfValue(root.width, root.maxValue, adjWidth)
            return 0;
        }
        y: {
            if (root.__vertical && !knobMaxMa.drag.active)
                return internal.yOfValue(root.width, root.maxValue, adjHeight)
            return 0;
        }
        z: 99

        source: root.knobMaxSource

        MouseArea {
            id: knobMaxMa

            property real handleCenter: !root.__vertical ? knobMax.width * 0.5 : knobMax.height * 0.5

            anchors.fill: parent

            hoverEnabled: true
            drag.target: pressed ? knobMax : undefined
            drag.axis: root.__vertical ? Drag.YAxis : Drag.XAxis
            drag.minimumX: {
                if (root.knobMaxInset)
                    return knobMin.x - (root.fillMargin * 2)
                else
                    return knobMin.x + (knobMin.width *0.5)
            }
            drag.maximumX: root.knobMaxInset ? root.width - (root.fillMargin + knobMax.width)
                                             : root.width - (knobMax.width * 0.5)
            drag.minimumY: {
                if (root.knobMaxInset)
                    return knobMin.y - (root.fillMargin * 2)
                else
                    return knobMin.y + (knobMin.height *0.5)
            }
            drag.maximumY: root.knobMaxInset ? root.height - (root.fillMargin + knobMax.height)
                                             : root.height - (knobMax.height * 0.5)

            onReleased: root.maxValueChangeRequested(internal.maxValue)
        }
    }

    Image {
        id: labelHolderMax

        anchors.left: root.bubbleMaxAnchorLeft ? knobMax.right : undefined
        anchors.horizontalCenter: !root.__vertical ? knobMax.horizontalCenter
                                                   : (root.bubbleMaxAnchorCenter ? knobMax.horizontalCenter : undefined)
        anchors.right: root.bubbleMaxAnchorRight ? knobMax.left : undefined
        anchors.top: root.bubbleMaxAnchorBottom ? knobMax.bottom : undefined
        anchors.verticalCenter: root.__vertical ? knobMax.verticalCenter
                                                : (root.bubbleMaxAnchorCenter ? knobMax.verticalCenter : undefined)
        anchors.bottom: root.bubbleMaxAnchorTop ? knobMax.top : undefined

        source: root.bubbleMaxSource

        visible: root.enableMaxValueDisplay && knobMaxMa.pressed
    }

    Text {
        id: labelMax
        anchors.centerIn: parent
        parent: labelHolderMax
        text: internal.maxValue
    }

    QtObject {
        id: internal
        property int minValue: valueForPosition((root.__vertical ? knobMin.y : knobMin.x) + knobMinMa.handleCenter, (root.__vertical ? root.height : root.width));
        property int maxValue: valueForPosition((root.__vertical ? knobMax.y : knobMax.x) + knobMaxMa.handleCenter, (root.__vertical ? root.height : root.width));

        function xOfValue(value, areaWidth, adjWidth) {
            return (areaWidth * ((value - root.min) / Math.max(1, root.max - root.min))) - adjWidth
        }

        function yOfValue(value, areaWidth, adjHeight) {
            return (areaWidth * ((value - root.min) / Math.max(1, root.max - root.min))) - adjHeight
        }

        function valueForPosition(position, span) {
            var value = Math.min(Math.max(root.min, root.min + ((position / Math.max(1, span))
                                                                * (root.max - root.min))),
                                 root.max);
            return value;
        }
    }
}
