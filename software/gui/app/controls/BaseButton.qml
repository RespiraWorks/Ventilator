import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import ".."
/*!
    \qmltype BaseButton
    \brief A styled button used as base for all buttons
*/
Button {
    id: root
    implicitWidth: 144
    implicitHeight: 64

    background: Rectangle {
        radius: 4
        color: root.checked || root.down ?
                   Style.theme.color.modalButtonHighlighted :
                   Style.theme.color.modalButton

    }

    contentItem: Text {
        text: root.text
        font.bold: true
        font.pixelSize: 30
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
