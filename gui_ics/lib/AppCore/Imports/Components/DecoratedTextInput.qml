import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/decoratedTextInput.png
Item {
    id: root

    width: 256
    height: 48

    signal textEntered(string inputText)
    signal accepted()

    property int iconLeftPadding: 5
    property int iconTopPadding: 0
    property int iconBottomPadding: 0

    //ASSET /placeholders/picture.png
    property alias icon: icon.source
    //BOOL true
    property alias iconVisible: icon.visible
    Image {
        id: icon

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: root.iconLeftPadding
        anchors.topMargin: root.iconTopPadding
        anchors.bottomMargin: root.iconTopPadding

        fillMode: Image.PreserveAspectFit
    }

    property int inputLeftPadding: 5
    property int inputRightPadding: 5
    property int inputTopPadding: 0
    property int inputBottomPadding: 0

    //COLOR textColor
    property alias color: input.color
    //TEXTHAL 1
    property alias horizontalAlignment: input.horizontalAlignment
    //TEXTVAL 128
    property alias verticalAlignment: input.verticalAlignment
    //TEXTWRAPM 0
    property alias wrapMode: input.wrapMode
    //FONT textFont
    property alias font: input.font
    //TRANSLATABLE
    property alias text: input.text
    //INT 32767
    property alias maximumLength: input.maximumLength
    //TEXTECHOMODE 0
    property alias echoMode: input.echoMode
    property bool inputFocus: false
    //TEXTINPUTHINTS 0
    property alias inputMethodHints: input.inputMethodHints
    //TEXT
    property string regExp: ""
    function installRegExp() {
        if (regExp !== "")
            input.validator.regExp = new RegExp(regExp)
        else
            input.validator.regExp = new RegExp(".*")
    }
    onRegExpChanged: installRegExp()
    TextInput {
        id: input

        anchors.left: icon.visible ? icon.right : parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: root.inputLeftPadding
        anchors.rightMargin: root.inputRightPadding
        anchors.topMargin: root.inputTopPadding
        anchors.bottomMargin: root.inputBottomPadding

        clip: true

        onTextChanged: {
            root.textEntered(text)
        }
        onAccepted: {
            root.accepted()
        }

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

        validator: RegExpValidator {
            id: validator
        }
    }

    //COLOR textColor
    property alias placeholderColor: placeholder.color
    //TEXTHAL 4
    property alias placeholderHorizontalAlignment: placeholder.horizontalAlignment
    //TEXTVAL 128
    property alias placeholderVerticalAlignment: placeholder.verticalAlignment
    //TEXTWRAPM 0
    property alias placeholderWrapMode: placeholder.wrapMode
    //FONT textFont
    property alias placeholderFont: placeholder.font
    //TRANSLATABLE placeholder
    property alias placeholderText: placeholder.text
    //TEXTELIDEM 3
    property alias placeholderelide: placeholder.elide
    Text {
        id: placeholder

        anchors.fill: input

        visible: input.text === "" && !input.focus
    }

    property bool autoClear: false
    onVisibleChanged: {
        if (autoClear)
            input.text = ""
    }
    Component.onCompleted: installRegExp()
}
