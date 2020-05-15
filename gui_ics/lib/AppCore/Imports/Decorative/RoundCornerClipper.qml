import QtQuick 2.0
import QtGraphicalEffects 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/rounded-rectangle-clip.png
Item {
    id: root

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: RoundCornerRectangle {
            width: root.width
            height: root.height
            color: "black"

            topLeftRadius: root.topLeftRadius
            topRightRadius: root.topRightRadius
            bottomLeftRadius: root.bottomLeftRadius
            bottomRightRadius: root.bottomRightRadius
        }
    }

    property int topLeftRadius: 10
    property int topRightRadius: 0
    property int bottomLeftRadius: 0
    property int bottomRightRadius: 10
}
