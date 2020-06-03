import QtQuick 2.11
import QtQuick.Controls 2.4
import ".."

/*!
    \qmltype HeaderButton
    \brief A border-base button used in main header of the screen.
*/
Button {
    id: control

    implicitWidth:36; implicitHeight: 36

    background: Rectangle {
        antialiasing: true
        color: "transparent"
        border.color: Style.theme.color.headerButtonColor
        border.width: 1
        radius: 4
    }
}
