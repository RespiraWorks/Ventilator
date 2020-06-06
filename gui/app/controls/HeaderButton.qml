import QtQuick 2.11
import QtQuick.Controls 2.4
import ".."

/*!
    \qmltype HeaderButton
    \brief A border-base button used in main header of the screen.
*/
Button {
    id: control

    implicitWidth:40; implicitHeight: 40

    background: Rectangle {
        antialiasing: true
        color: control.down ? Style.theme.color.headerButtonHighlighted :
                              Style.theme.color.headerButton
        radius: 8
    }

    contentItem: Text {
        text: control.text
        font: Style.theme.font.headerButton
        color: Style.theme.color.textPrimary
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
