import QtQuick 2.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/decorations.png
Item {
    id: root

    //ASSET
    property alias source: decorations.source
    property int horizontalCenterOffset: 0
    property int verticalCenterOffset: 0
    Image {
        id: decorations

        anchors.horizontalCenterOffset: root.horizontalCenterOffset
        anchors.verticalCenterOffset: root.verticalCenterOffset

        StateGroup {
            id: horizontalAnchoringGroup

            states: [
                State {
                    when: decorations.status === Image.Ready
                          && decorations.source !== ""
                          && decorations.sourceSize.width <= root.width
                    AnchorChanges {
                        target: decorations
                        anchors.left: root.left
                        anchors.right: root.right
                        anchors.horizontalCenter: undefined
                    }
                },
                State {
                    when: decorations.status !== Image.Ready
                          || decorations.source === ""
                          || (decorations.status === Image.Ready
                              && decorations.sourceSize.width > root.width)
                    AnchorChanges {
                        target: decorations
                        anchors.left: undefined
                        anchors.right: undefined
                        anchors.horizontalCenter: root.horizontalCenter
                    }
                }
            ]
        }

        StateGroup {
            id: verticalAnchoringGroup

            states: [
                State {
                    when:  decorations.status === Image.Ready
                           && decorations.source !== ""
                           && decorations.sourceSize.height <= root.height
                    AnchorChanges {
                        target: decorations
                        anchors.top: root.top
                        anchors.bottom: root.bottom
                        anchors.verticalCenter: undefined
                    }
                },
                State {
                    when: decorations.status !== Image.Ready
                          || decorations.source === ""
                          || (decorations.status === Image.Ready
                              && decorations.sourceSize.height > root.height)
                    AnchorChanges {
                        target: decorations
                        anchors.top: undefined
                        anchors.bottom: undefined
                        anchors.verticalCenter: root.verticalCenter
                    }
                }
            ]
        }

        z: -999
    }
}
