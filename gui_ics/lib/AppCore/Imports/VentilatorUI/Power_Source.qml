import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Rectangle
//USES BorderImage
//USES Image

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //COLOR Figma::255_196_196_196
    property alias fi_Rectangle_26_color: fi_Rectangle_26.color
    //REAL 0
    property alias fi_Rectangle_26_opacity: fi_Rectangle_26.opacity
    //REAL 1
    property alias fi_Union_opacity: fi_Union.opacity
    //ASSET /122_2237.png
    property alias fi_Union_source: fi_Union.source

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 113
    height: 60
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Rectangle {
        id: fi_Rectangle_26
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        color: GreenHouseThemeManager.theme.color("Figma::255_196_196_196")
        opacity: 0
        border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
        border.width: 1
        enabled: true
        visible: true
        clip: false
        radius: 0
        rotation: 0
        scale: 1
    }
    BorderImage {
        id: fi_Rectangle_469
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
        z: 1
        source: ""
        opacity: 1
        enabled: true
        visible: true
        border.bottom: 5
        border.left: 5
        border.right: 5
        border.top: 5
        clip: false
        mirror: false
        rotation: 0
        scale: 1
    }
    Image {
        id: fi_Union
        anchors.left: parent.left
        anchors.leftMargin: 23
        anchors.right: parent.right
        anchors.rightMargin: 22
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        z: 2
        source: GreenHouseThemeManager.theme.asset("/122_2237.png")
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
