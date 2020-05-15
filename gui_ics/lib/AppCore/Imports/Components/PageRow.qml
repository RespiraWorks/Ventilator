import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/pageRow.png
//PARENTING FALSE
Flickable {
    id: root

    width: 120
    height: 40

    contentWidth: row.width
    contentHeight: height
    flickableDirection: Flickable.HorizontalFlick
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    //INT 500
    property alias slideDuration: slideAnimation.duration
    property bool flickingBlocked: false
    interactive: !flickingBlocked

    function positionAt(index, force) {
        cancelFlick();
        if (row.currentIndex === index && !slideAnimation.running && !force)
            return;
        slideAnimation.requestStop();
        slideAnimation.from = contentX;
        slideAnimation.to = Math.max(0, Math.min(row.children.length - 1, Math.max(0, index)) * width);
        if (slideAnimation.from === slideAnimation.to)
            return;
        slideAnimation.start();
    }

    property int indexRequestedByBackend: 0
    onIndexRequestedByBackendChanged: positionAt(indexRequestedByBackend, false)
    signal movingStoppedAtIndex(var index);
    onFlickStarted: {
        if (horizontalVelocity < 0) {
            positionAt(row.currentIndex - 1, false)
        } else {
            positionAt(row.currentIndex + 1, false)
        }
    }
    onMovementEnded: {
        if (!slideAnimation.running) {
            positionAt(Math.round(contentX/width), true)
        }
    }

    PropertyAnimation {
        id: slideAnimation

        target: root
        property: "contentX"
        duration: 500
        easing.type: Easing.InOutQuad

        property bool manualStop: false
        function requestStop() {
            if (running) {
                manualStop = true
                stop();
            }
        }

        onStarted: manualStop = false
        onStopped: {
            if (!manualStop) {
                row.currentIndex = Math.floor(root.contentX/Math.max(0, root.width));
                root.movingStoppedAtIndex(row.currentIndex);
            }
        }
    }

    onWidthChanged: {
        row.__sync()
        contentX = row.currentIndex * width
        returnToBounds()
    }
    onHeightChanged: row.__sync()

    //HIDE property childContainer
    property Item childContainer: row
    Item {
        id: row
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: childrenRect.width
        property int currentIndex: 0

        function __sync() {
            for (var i = 0; i < children.length; ++i) {
                var child = children[i];
                if (child) {
                    child.x = i * root.width;
                    child.y = 0;
                    child.width = root.width;
                    child.height = root.height;
                    child.clip = true;
                }
            }
        }

        onChildrenChanged: __sync()
    }
}
