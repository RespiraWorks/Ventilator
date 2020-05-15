import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/textInput.png
TextInput {
    id: root

    width: 256
    height: 48

    property bool inputFocus: false
    //HIDE property focus

    StateGroup {
        states: [
            State {
                when: root.visible
                PropertyChanges {
                    target: root
                    focus: root.inputFocus
                }
            },
            State {
                when: !root.visible
                PropertyChanges {
                    target: root
                    focus: false
                }
            }
        ]
    }

    property string regExp: ""
    function installRegExp() {
        if (regExp !== "")
            validator.regExp = new RegExp(regExp)
        else
            validator.regExp = new RegExp(".*")
    }
    onRegExpChanged: installRegExp()
    validator: RegExpValidator {
        id: validator
    }

    property bool autoClear: false
    onVisibleChanged: {
        if (autoClear)
            text = ""
    }

    Component.onCompleted: installRegExp()
}
