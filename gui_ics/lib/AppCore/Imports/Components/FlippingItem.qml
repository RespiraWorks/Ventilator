import QtQuick 2.2
import "Internal"

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/flip.png
Item {
    id: root

    width: 128
    height: 256
    //INT 500
    property alias flipDuration: sideA.flipDuration

    //ITEMREF
    property alias sideAContents: sideA.contents
    FlippingItemContainer {
        id: sideA

        anchors.centerIn: parent

        onClicked: {
            if (root.flipOnClick)
                root.flip()
        }
    }

    //ITEMREF
    property alias sideBContents: sideB.contents
    FlippingItemContainer {
        id: sideB

        anchors.centerIn: parent

        flipDuration: sideA.flipDuration

        onClicked: {
            if (root.flipOnClick)
                root.flip()
        }
    }

    function flip() {
        root.flipped = !root.flipped
    }

    property int currentSideWidth: 0
    Binding {
        when: sideA.visible
        target: root
        property: "currentSideWidth"
        value: sideA.width
    }
    Binding {
        when: sideB.visible
        target: root
        property: "currentSideWidth"
        value: sideB.width
    }

    property bool flipOnClick: true
    property bool flipped: false
    states: [
        State {
            name: "SideA"
            when: !root.flipped
            PropertyChanges {
                target: sideA
                rotationAngle: 0.0
            }
            PropertyChanges {
                target: sideB
                rotationAngle: 180.0
            }
        },
        State {
            name: "SideB"
            when: root.flipped
            PropertyChanges {
                target: sideA
                rotationAngle: -180.0
            }
            PropertyChanges {
                target: sideB
                rotationAngle: 0.0
            }
        }
    ]
}
