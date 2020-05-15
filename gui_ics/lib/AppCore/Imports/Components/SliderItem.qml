import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/sliderItem.png
Item {
    id: root

    width: 128
    height: 22

    signal valueChangeRequested(int value)

    property int min: 0
    property int max: 100
    property int value: 25
    property int stepSize: 1

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
    property url knobSource: ""
    //ASSET
    property url fillSource: ""
    //ASSET
    property url bubbleSource: ""

    property bool touchPositioning: true
    property bool enableValueDisplay: false
    property bool bubbleAnchorTop: false
    property bool bubbleAnchorCenter: false
    property bool bubbleAnchorBottom: false
    property bool bubbleAnchorLeft: false
    property bool bubbleAnchorRight: false
    property bool knobInset: false

    //COLOR textColor
    property alias textColor: label.color
    //FONT textFont
    property alias textFont: label.font

    property bool __vertical: width < height

    BorderImage {
        id: background

        anchors.left: !root.__vertical ? parent.left : undefined
        anchors.right: !root.__vertical ? parent.right : undefined
        anchors.verticalCenter: !root.__vertical ? parent.verticalCenter : undefined
        anchors.bottom: !root.__vertical ? undefined : parent.bottom
        anchors.top: !root.__vertical ? undefined : parent.top
        anchors.horizontalCenter: !root.__vertical ? undefined : parent.horizontalCenter

        border.left: root.leftBackgroundAssetBorder
        border.right: root.rightBackgroundAssetBorder
        border.top: root.topBackgroundAssetBorder
        border.bottom: root.bottomBackgroundAssetBorder

        source: root.backgroundSource
    }

    BorderImage {
        id: fillImage

        anchors.left: background.left
        anchors.right: !root.__vertical ? knob.right : background.right
        anchors.top: background.top
        anchors.bottom: !root.__vertical ? background.bottom : knob.bottom
        anchors.margins: root.fillMargin

        border.left: root.leftFillAssetBorder
        border.right: root.rightFillAssetBorder
        border.top: root.topFillAssetBorder
        border.bottom: root.bottomFillAssetBorder

        source: root.fillSource
    }

    Image {
        id: knob

        property int adjWidth: root.knobInset ? -root.fillMargin : width * 0.5
        property int adjHeight: root.knobInset ? -root.fillMargin : height * 0.5

        anchors.verticalCenter: !root.__vertical ? parent.verticalCenter : undefined
        anchors.horizontalCenter: !root.__vertical ? undefined : parent.horizontalCenter

        x: {
            if (!root.__vertical && !knobMa.drag.active)
                return ((knobMa.drag.maximumX - knobMa.drag.minimumX)
                        * ((root.value - root.min) / Math.max(1, root.max - root.min))) - adjWidth
            return 0;
        }
        y: {
            if (root.__vertical && !knobMa.drag.active)
                return ((knobMa.drag.maximumY - knobMa.drag.minimumY)
                        * ((root.value - root.min) / Math.max(1, root.max - root.min))) - adjHeight
            return 0;
        }
        z: 1

        source: root.knobSource

        MouseArea {
            id: knobMa

            anchors.fill: parent

            property real handleCenter: !root.__vertical ? knob.width * 0.5 : knob.height * 0.5

            drag.target: pressed ? knob : undefined
            drag.axis: root.__vertical ? Drag.YAxis : Drag.XAxis
            drag.minimumX: root.knobInset ? root.fillMargin : - (knob.width * 0.5)
            drag.maximumX: {
                if (root.knobInset)
                    return root.width - (root.fillMargin + knob.width)
                else
                    return root.width - ((knob.width * 0.5) + root.rightBackgroundAssetBorder)
            }
            drag.minimumY: root.knobInset ? root.fillMargin : - (knob.height * 0.5)
            drag.maximumY: {
                if (root.knobInset)
                    return root.height - (root.fillMargin + knob.height)
                else
                    return root.height - (knob.height * 0.5)
            }
            onReleased: root.valueChangeRequested(internal.value)
        }
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            if (!knobMa.pressed && root.touchPositioning) {
                if (!root.__vertical) {
                    root.valueChangeRequested(internal.valueForPosition(mouse.x, width))
                } else {
                    root.valueChangeRequested(internal.valueForPosition(mouse.y, height))
                }
            }
        }
    }

    Image {
        id: labelHolder

        anchors.left: root.bubbleAnchorLeft ? knob.right : undefined
        anchors.horizontalCenter: !root.__vertical ? knob.horizontalCenter
                                                   : (root.bubbleAnchorCenter ? knob.horizontalCenter : undefined)
        anchors.right: root.bubbleAnchorRight ? knob.left : undefined
        anchors.top: root.bubbleAnchorBottom ? knob.bottom : undefined
        anchors.verticalCenter: root.__vertical ? knob.verticalCenter
                                                : (root.bubbleAnchorCenter ? knob.verticalCenter : undefined)
        anchors.bottom: root.bubbleAnchorTop ? knob.top : undefined

        source: root.bubbleSource
        visible: root.enableValueDisplay && knobMa.pressed
    }

    Text {
        id: label
        anchors.centerIn: parent
        parent: labelHolder
        text: internal.value
    }

    QtObject {
        id: internal
        property int value: valueForPosition((root.__vertical ? knob.y : knob.x) + knobMa.handleCenter, (root.__vertical ? root.height : root.width));
        function valueForPosition(position, span) {
            var value = Math.min(Math.max(root.min, root.min + ((position / Math.max(1, span))
                                                                * (root.max - root.min))),
                                 root.max);
            if (root.stepSize > 1) {
                var overShoot = value % root.stepSize;
                if (root.stepSize > 2 && overShoot >= (root.stepSize / 2)) {
                    value += root.stepSize - overShoot;
                } else {
                    value -= overShoot;
                }
            }
            return value;
        }
    }
}
