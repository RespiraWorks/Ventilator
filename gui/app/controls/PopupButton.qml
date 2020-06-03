import QtQuick 2.11
import QtQuick.Controls 2.4
import ".."

/*!
    \qmltype PopupButton
    \brief Simple styled button to be used in popups
*/
RadioButton {
    id: root
    implicitWidth: 720
    implicitHeight: 70

    indicator: Rectangle {
        implicitWidth: 32
        implicitHeight: 32
        x: 20
        y: 19
        radius: 32
        smooth: true
        color: "transparent"
        border.color: Style.theme.color.radioButtonColor
        border.width: 3

        Rectangle {
            anchors.centerIn: parent
            width: parent.width / 2
            height: parent.height / 2
            radius: width / 2
            smooth: true
            color: Style.theme.color.radioButtonColor
            visible: root.checked
        }
    }
    background: Rectangle {
        radius: 4
        color: root.checked || root.down ?
                   Style.theme.color.modalButtonHighlighted :
                   Style.theme.color.modalButton

    }

    contentItem: Text {
        text: root.text
        font.bold: true
        font.pixelSize: 38
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
