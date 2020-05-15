import QtQuick 2.0

Item {
    id: root

    property ViewItemInterface itemInterface: ViewItemInterface { }
    visible: itemInterface.allowedToDisplay
}
