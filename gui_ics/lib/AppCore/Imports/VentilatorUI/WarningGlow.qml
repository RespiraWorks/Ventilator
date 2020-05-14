import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Image

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //REAL 1
    property alias fi_Rectangle_480_opacity: fi_Rectangle_480.opacity
    //ASSET /210_143.png
    property alias fi_Rectangle_480_source: fi_Rectangle_480.source

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 986
    height: 148
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Image {
        id: fi_Rectangle_480
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        source: GreenHouseThemeManager.theme.asset("/210_143.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
