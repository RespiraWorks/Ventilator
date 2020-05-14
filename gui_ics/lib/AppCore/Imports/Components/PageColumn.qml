import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/pageColumn.png
//PARENTING FALSE
Flickable {
    id: root

    width: 40
    height: 120

    contentWidth: width
    contentHeight: column.height
    flickableDirection: Flickable.VerticalFlick
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    //INT 500
    property alias slideDuration: slideAnimation.duration
    property bool flickingBlocked: false
    interactive: !flickingBlocked

    default property alias rowContents: column.data
    function positionAt(index, force) {
        cancelFlick();
        if (column.currentIndex === index && !slideAnimation.running && !force)
            return;
        slideAnimation.requestStop();
        slideAnimation.from = contentY;
        slideAnimation.to = Math.max(0, Math.min(column.children.length - 1, Math.max(0, index)) * height);
        if (slideAnimation.from === slideAnimation.to)
            return;
        slideAnimation.start();
    }

    property int indexRequestedByBackend: 0
    onIndexRequestedByBackendChanged: positionAt(indexRequestedByBackend, false)
    signal movingStoppedAtIndex(var index);
    onFlickStarted: {
        if (verticalVelocity < 0) {
            positionAt(column.currentIndex - 1, false)
        } else {
            positionAt(column.currentIndex + 1, false)
        }
    }
    onMovementEnded: {
        if (!slideAnimation.running) {
            positionAt(Math.round(contentY/height), true)
        }
    }

    PropertyAnimation {
        id: slideAnimation

        target: root
        property: "contentY"
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
                column.currentIndex = Math.floor(root.contentY/Math.max(0, root.height));
                root.movingStoppedAtIndex(column.currentIndex);
            }
        }
    }

    onWidthChanged: column.__sync()
    onHeightChanged: {
        column.__sync()
        contentY = column.currentIndex * height
        returnToBounds()
    }

    //HIDE property childContainer
    property Item childContainer: column
    Item {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        height: childrenRect.height
        property int currentIndex: 0

        function __sync() {
            for (var i = 0; i < children.length; ++i) {
                var child = children[i];
                if (child) {
                    child.x = 0;
                    child.y = i * root.height;
                    child.width = root.width;
                    child.height = root.height;
                    child.clip = true;
                }
            }
        }

        onChildrenChanged: __sync()
    }
}
