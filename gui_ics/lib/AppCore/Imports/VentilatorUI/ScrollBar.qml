import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Item
//USES BorderImage

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 60
    height: 240
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Item {
        id: fi_background
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
        clip: true
        rotation: 0
        scale: 1
    }
    Item {
        id: fi_indicator
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        z: 1
        height: 140
        opacity: 1
        enabled: true
        visible: true
        clip: true
        rotation: 0
        scale: 1

        BorderImage {
            id: fi_indicator_9seg_40_40_40_40
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.right: parent.right
            anchors.rightMargin: -10
            anchors.top: parent.top
            anchors.topMargin: -10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -10
            z: 0
            source: GreenHouseThemeManager.theme.asset("/233_1399.png")
            opacity: 1
            enabled: true
            visible: true
            border.bottom: 40
            border.left: 40
            border.right: 40
            border.top: 40
            clip: true
            mirror: false
            rotation: 0
            scale: 1
        }
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
