import QtQuick 2.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/cover_flicker.png
Image {
    id: root

    //ASSET /placeholders/picture.png
    property string coverAsset: "qrc:/assets/img.png"

    //HIDE property source

    onCoverAssetChanged: {
        if (status === Image.Ready) {
            flickAnimation.stop()
            flickAnimation.start()
        } else {
            root.source = coverAsset
        }
    }

    property real __flickAngle: 0.0
    transform: [
        Rotation {
            origin {
                x: 0
                y: root.height * 0.5
            }
            axis {
                x: 0
                y: 1
                z: 0
            }
            angle: root.__flickAngle
        }
    ]

    SequentialAnimation {
        id: flickAnimation
        ParallelAnimation {
            PropertyAnimation {
                target: root
                property: "__flickAngle"
                from: 0.0
                to: -75.0
                duration: 500
            }
            PropertyAnimation {
                target: root
                property: "opacity"
                from: 1.0
                to: 0.0
                duration: 500
            }
        }
        ScriptAction {
            script: root.source = root.coverAsset
        }
        ParallelAnimation {
            PropertyAnimation {
                target: root
                property: "__flickAngle"
                from: 75.0
                to: 0.0
                duration: 500
            }
            PropertyAnimation {
                target: root
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: 500
            }
        }
    }
}
