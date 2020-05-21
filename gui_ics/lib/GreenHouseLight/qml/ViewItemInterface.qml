import QtQuick 2.0

Displayable {
    id: root

    property string itemName: ""
    property string surfaceName: ""

    signal requestItem(string name)
    signal requestDefault()
}
