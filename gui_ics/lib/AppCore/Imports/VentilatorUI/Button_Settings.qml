import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Rectangle
//USES BorderImage
//USES Image
//USES Item
//USES MouseArea

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //COLOR Figma::255_196_196_196
    property alias fi_Rectangle_21_color: fi_Rectangle_21___default.color
    //REAL 0
    property alias fi_Rectangle_21_opacity: fi_Rectangle_21___default.opacity
    //ASSET /122_2230.png
    property alias fi_Union___default_source: fi_Union___default.source
    //REAL 1
    property alias fi_Union_opacity: fi_Union___default.opacity
    //ASSET /233_1692.png
    property alias fi_Union___pressed_source: fi_Union___pressed.source

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
    width: 56
    height: 60
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Item {
        id: default_StateParent
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
        rotation: 0
        scale: 1

        Rectangle {
            id: fi_Rectangle_21___default
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
            id: fi_Rectangle_469___default
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 1
            source: GreenHouseThemeManager.theme.asset("/138_711.png")
            opacity: 1
            enabled: true
            visible: true
            border.bottom: 4
            border.left: 4
            border.right: 4
            border.top: 4
            clip: false
            mirror: false
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___default
            anchors.left: parent.left
            anchors.leftMargin: 17
            anchors.right: parent.right
            anchors.rightMargin: 17
            anchors.top: parent.top
            anchors.topMargin: 19
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 19
            z: 2
            source: GreenHouseThemeManager.theme.asset("/122_2230.png")
            fillMode: Image.Stretch
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
    }
    Item {
        id: pressed_StateParent
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
        rotation: 0
        scale: 1

        Rectangle {
            id: fi_Rectangle_21___pressed
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___default.color
            opacity: fi_Rectangle_21___default.opacity
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
            id: fi_Rectangle_469___pressed
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 1
            source: GreenHouseThemeManager.theme.asset("/233_1691.png")
            opacity: 1
            enabled: true
            visible: true
            border.bottom: 4
            border.left: 4
            border.right: 4
            border.top: 4
            clip: false
            mirror: false
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___pressed
            anchors.left: parent.left
            anchors.leftMargin: 17
            anchors.right: parent.right
            anchors.rightMargin: 17
            anchors.top: parent.top
            anchors.topMargin: 19
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 19
            z: 2
            source: GreenHouseThemeManager.theme.asset("/233_1692.png")
            fillMode: Image.Stretch
            opacity: fi_Union___default.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
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
    StateGroup { //
        states: [
        State {
            when: !__mouseArea.pressed
            name: "default"
            PropertyChanges {
                target: default_StateParent
                visible: true
            }
            PropertyChanges {
                target: pressed_StateParent
                visible: false
            }
        },
        State {
            when: __mouseArea.pressed
            name: "pressed"
            PropertyChanges {
                target: default_StateParent
                visible: false
            }
            PropertyChanges {
                target: pressed_StateParent
                visible: true
            }
        }
        ]
    }

    //----------------------------------------------------------------------------------------------
}
