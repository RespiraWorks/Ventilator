//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process

import QtQuick 2.0

SequentialAnimation {
    id: root

    property Item target

    ScriptAction {
        script: {

        }
    }

    PropertyAction { target: root.target; property: "visible"; value: false }
    ScriptAction {
        script: {

        }
    }

    onStopped: {
    }
}
