import QtQuick 2.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/text_ribbon.png
Text {
    id: root

    //TRANSLATABLE
    property string ribbonText: ""
    //HIDE property text

    onRibbonTextChanged: {
        if (text !== "") {
            ribbonAnimation.stop()
            ribbonAnimation.start()
        } else {
            text = ribbonText;
        }
    }

    property int slideInDistance: 64
    property int slideOutDistance: 64

    property double __xTranslation: 0.0
    transform: [
        Translate {
            x: root.__xTranslation
        }
    ]

    SequentialAnimation {
        id: ribbonAnimation
        ParallelAnimation {
            PropertyAnimation {
                target: root
                property: "__xTranslation"
                from: 0.0
                to: -root.slideOutDistance
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
            script: root.text = root.ribbonText
        }
        ParallelAnimation {
            PropertyAnimation {
                target: root
                property: "__xTranslation"
                from: root.slideInDistance
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
