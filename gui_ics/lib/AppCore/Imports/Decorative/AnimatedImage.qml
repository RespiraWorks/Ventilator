import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/animated_image.png
Image {
    id: root

    //ASSET /placeholders/picture.png
    property string masterAsset: "qrc:/assets/img.png"
    property string frameId: ""
    property int frameIdLengh: 3
    property int frameCount: 16
    property int frameInterval: 15
    property int _frame: 0
    property int _actualFrame: 0
    property bool autoRestart: true
    property bool animate: true
    property string ignoredFramesRange: ""
    property var _ignoredFrameIds: []

    property int startFrame: 0

    onIgnoredFramesRangeChanged: {
        var ignoredFrameIds = []
        var ranges = str.split(";");
        for (var i = 0; i < ranges.length; ++i) {
            var range = ranges[i].split("-")
            if (range.length === 2) {
                var limit = parseInt(range[1])
                for (var j = parseInt(range[0]); j < limit + 1; ++j)
                    ignoredFrameIds.push(j)
            }
        }
        _ignoredFrameIds = ignoredFrameIds;
    }

    signal sequenceFinished()

    //HIDE property source
    source: {
        if (frameId !== "")
            return GreenHouseThemeManager.theme.animationFrameAsset(frameId, _actualFrame, frameIdLengh)
        return masterAsset
    }

    function __startIfValid() {
        if (frameId !== "" && frameCount > 0 && visible && animate) {
            _frame = startFrame
            _actualFrame = startFrame
            animator.start()
        } else {
            animator.stop()
        }
    }
    onVisibleChanged: __startIfValid()
    onFrameIdChanged: __startIfValid()
    onFrameCountChanged: __startIfValid()
    onAutoRestartChanged: __startIfValid()
    onAnimateChanged: __startIfValid()
    Timer {
        id: animator

        interval: root.frameInterval
        repeat: true
        onTriggered: {
            if (root._frame < root.frameCount) {
                root._frame += 1
                if (root._ignoredFrameIds.indexOf(root._frame) === -1)
                    root._actualFrame = root._frame
            } else if (root.autoRestart) {
                root._frame = startFrame
                root._actualFrame = startFrame
                root.sequenceFinished()
            } else {
                stop()
                root.sequenceFinished()
            }
        }
    }

    Component.onCompleted: __startIfValid()
}
