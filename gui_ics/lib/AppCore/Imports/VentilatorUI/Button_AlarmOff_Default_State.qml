import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0
import Decorative 1.0 as DecorativeSet

//USES DecorativeSet.DecoratedItem
//USES Image
//USES MouseArea

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //REAL 1
    property alias icon_opacity: icon.opacity
    //ASSET /Icons/AlarmOff__DefaultState.png
    property alias icon_source: icon.source

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------
    signal clicked() /*__mouseArea.clicked*/
    signal pressed() /*__mouseArea.pressed*/
    signal released() /*__mouseArea.released*/

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 90
    height: 90
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    DecorativeSet.DecoratedItem {
        id: fi_Ellipse_89
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        source: GreenHouseThemeManager.theme.asset("/210_23.png")
        opacity: 1
        enabled: true
        visible: true
        clip: false
        horizontalCenterOffset: 0
        rotation: 0
        scale: 1
        verticalCenterOffset: 4
    }
    Image {
        id: icon
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 0
        z: 1
        width: 90
        height: 90
        source: GreenHouseThemeManager.theme.asset("/Icons/AlarmOff__DefaultState.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    MouseArea {
        id: __mouseArea
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        opacity: 1
        enabled: true
        visible: true
        clip: false
        hoverEnabled: false
        rotation: 0
        scale: 1

        onClicked: {
            root.clicked();
        }
        onPressed: {
            root.pressed();
        }
        onReleased: {
            root.released();
        }
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
