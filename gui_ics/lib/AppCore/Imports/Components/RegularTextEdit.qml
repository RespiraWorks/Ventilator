import QtQuick 2.6

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/textEdit.png
FocusScope {
    id: root
    width: 200; height: 300
    Keys.forwardTo: input
    signal textEntered (string text)
    //STRING
    property alias text: input.text
    //COLOR textColor
    property alias color: input.color
    //FONT textFont
    property alias font: input.font
    //BOOL
    property bool autoClear: false
    Flickable {
        id: flick
        anchors.fill: parent
        clip: true
        contentHeight: input.contentHeight
        contentWidth: input.contentWidth

        function ensureVisible(r) {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX + width <= r.x + r.width)
                contentX = r.x + r.width - width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY + height <= r.y + r.height)
                contentY = r.y + r.height - height;
        }

        TextEdit {
            id: input
            //HIDE property focus
            focus: true
            width: flick.width
            wrapMode: TextEdit.Wrap
            onTextChanged: root.textEntered(text)
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

            onVisibleChanged: {
                if (root.autoClear)
                    input.clear()
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: root.forceActiveFocus()
    }
}
