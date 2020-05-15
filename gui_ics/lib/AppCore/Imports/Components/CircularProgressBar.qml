import QtQuick 2.6

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/circular-clockwise-arrow.png
Item {
    id: root

    width: 88
    height: 88

    property int min: 0
    property int max: 100
    property int value: 25
    property bool clockwise: true
    property bool drawBackground: false
    property bool roundedStroke: false
    //COLOR circularProgressColor
    property color barColor: "#6daf2f"
    //COLOR circularProgressBackgroundColor
    property color backgroundColor: "#7f8c8d"
    property int barWidth: 20
    property int startOffset: 0
    property int span: 360

    onMinChanged: progressCanvas.requestPaint()
    onMaxChanged: progressCanvas.requestPaint()
    onValueChanged: progressCanvas.requestPaint()
    onClockwiseChanged: progressCanvas.requestPaint()
    onDrawBackgroundChanged: progressCanvas.requestPaint()
    onRoundedStrokeChanged: progressCanvas.requestPaint()
    onBarColorChanged: progressCanvas.requestPaint()
    onBarWidthChanged: progressCanvas.requestPaint()
    onSpanChanged: progressCanvas.requestPaint()

    Canvas {
        id: progressCanvas

        anchors.centerIn: parent
        width: Math.min(root.height, root.width)
        height: width
        rotation: root.startOffset

        onPaint: {
            var ctx = progressCanvas.getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.lineCap = root.roundedStroke ? "round" : "butt"
            ctx.lineWidth = root.barWidth
            if (root.drawBackground) {
                ctx.strokeStyle = root.backgroundColor
                ctx.beginPath()
                ctx.arc(width * 0.5, height * 0.5, (width - root.barWidth) * 0.5, 0, root.span * (2 * Math.PI) / 360.)
                ctx.stroke()
            }
            ctx.strokeStyle = root.barColor
            ctx.beginPath()
            var val = Math.min(1.0, (Math.max(0, root.value - root.min)/Math.max(1, root.max - root.min)))
            ctx.arc(width * 0.5, height * 0.5, (width - root.barWidth) * 0.5, 0,
                    ( root.span * 2. * Math.PI / 360. ) * val,
                    !root.clockwise)
            ctx.stroke()
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
        anchors.bottom: labelAnchorTop ? parent.top : undefined
        anchors.top: labelAnchorBottom ? parent.bottom : undefined
        anchors.right: labelAnchorRight ? parent.left : undefined
        anchors.left: labelAnchorLeft ? parent.right : undefined
        anchors.verticalCenter: labelAnchorVCenter ? parent.verticalCenter : undefined
        anchors.horizontalCenter: labelAnchorHCenter ? parent.horizontalCenter : undefined
        opacity: {
            if ( (labelAnchorTop && labelAnchorBottom && labelAnchorHCenter)
                    || (labelAnchorRight && labelAnchorLeft && labelAnchorVCenter)
                    || (labelAnchorHCenter && labelAnchorVCenter))
                if (parent.width < fm.advanceWidth(label.text) || parent.height < fm.height)
                    return 0.0
            return 1.0
        }
        text: valuePrefix + value + valueSuffix
        visible: false
        parent: {
            if (parentBackground)
                return background
            else if (parentIndicator)
                return indicator
            return root
        }
    }

    FontMetrics {
        id: fm
        font: label.font
    }
}
