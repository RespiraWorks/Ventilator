import QtQuick 2.11
import QtQuick.Controls 2.4
import ".."

/*!
    \qmltype PopupButton
    \brief Simple styled button to be used in popups
*/
Button {
    id: root
    implicitWidth: 144
    implicitHeight: 64

    background: Rectangle {
        radius: 8
        width: root.width; height: parent.height
        enabled: true
        color: root.checked || root.down ?
                   Style.theme.color.modalButtonHighlighted :
                   Style.theme.color.modalButton

    }

    contentItem: Text {
        anchors.centerIn: parent
        text: root.text
        font: Style.theme.font.modalButton
        color: Style.theme.color.textPrimary
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

}
