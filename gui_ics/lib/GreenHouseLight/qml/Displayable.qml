import QtQuick 2.0

QtObject {
    id: root

    property bool allowedToDisplay: false
    property var entryArguments
    property var exitArguments
    property string replaces: ""
    property string replacedBy: ""

    signal init()
    signal show()
    signal hide()
}
