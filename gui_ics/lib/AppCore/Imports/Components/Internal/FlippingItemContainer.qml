import QtQuick 2.0

MouseArea {
    id: root

    property Item contents: null
    property int flipDuration: 500

    width: contents ? contents.width : 0
    height: contents ? contents.height : 0

    enabled: rotationAngle === 0
    visible: rotationAngle < 90.0 && rotationAngle > -90.0
    property double rotationAngle: 180.0
    Behavior on rotationAngle {
        NumberAnimation { duration: root.flipDuration }
    }
    transform: [
        Rotation {
            origin {
                x: root.width * 0.5
                y: root.height * 0.5
                z: 0.0
            }
            axis {
                x: 0.0
                y: 1.0
                z: 0.0
            }
            angle: root.rotationAngle
        }
    ]

    states: [
        State {
            when: root.contents && root.contents.parent === root
            AnchorChanges {
                target: root.contents
                anchors.left: root.left
                anchors.top: root.top
            }
        }
    ]
    onContentsChanged: {
        if (contents)
            contents.parent = root
    }
}
