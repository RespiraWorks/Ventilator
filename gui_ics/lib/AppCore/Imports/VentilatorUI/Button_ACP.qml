import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Image
//USES BorderImage
//USES Text
//USES Item
//USES MouseArea

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    property bool triggered: false
    //COLOR buttonAltTextColor
    property alias fi_AC_P_MODE___default_color: fi_AC_P_MODE___default.color
    //TEXTHAL 4
    property alias fi_AC_P_MODE___default_horizontalAlignment: fi_AC_P_MODE___default.horizontalAlignment
    //REAL 0.85
    property alias fi_AC_P_MODE___default_opacity: fi_AC_P_MODE___default.opacity
    //TRANSLATABLE AC/P
    property alias fi_AC_P_MODE___default_text: fi_AC_P_MODE___default.text
    //TEXTVAL 128
    property alias fi_AC_P_MODE___default_verticalAlignment: fi_AC_P_MODE___default.verticalAlignment
    //ASSET
    property alias fi_Subtract___default_source: fi_Subtract___default.source
    //REAL 1
    property alias fi_Subtract_opacity: fi_Subtract___default.opacity
    //ASSET /233_1735.png
    property alias fi_Subtract___pressed_source: fi_Subtract___pressed.source
    //ASSET /138_3507.png
    property alias fi_Subtract___triggered_source: fi_Subtract___triggered.source

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
    width: 86
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

        Image {
            id: fi_Subtract___default
            anchors.left: parent.left
            anchors.leftMargin: 3060
            anchors.right: parent.right
            anchors.rightMargin: -2974
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -2738
            z: 0
            height: 0
            source: ""
            fillMode: Image.Stretch
            opacity: 1
            enabled: true
            visible: true
            clip: false
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
            source: GreenHouseThemeManager.theme.asset("/138_665.png")
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
        Text {
            id: fi_AC_P_MODE___default
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            z: 2
            text: qsTr("AC/P")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_19_75_0_0")
            color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            opacity: 0.85
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            maximumLineCount: 2147483647
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

        Image {
            id: fi_Subtract___pressed
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 1
            source: GreenHouseThemeManager.theme.asset("/233_1735.png")
            fillMode: Image.Stretch
            opacity: fi_Subtract___default.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
    }
    Item {
        id: triggered_StateParent
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

        Image {
            id: fi_Subtract___triggered
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 1
            source: GreenHouseThemeManager.theme.asset("/138_3507.png")
            fillMode: Image.Stretch
            opacity: fi_Subtract___default.opacity
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
            when: !__mouseArea.pressed && !root.triggered
            name: "default"
            PropertyChanges {
                target: default_StateParent
                visible: true
            }
            PropertyChanges {
                target: pressed_StateParent
                visible: false
            }
            PropertyChanges {
                target: triggered_StateParent
                visible: false
            }
        },
        State {
            when: __mouseArea.pressed && !root.triggered
            name: "pressed"
            PropertyChanges {
                target: default_StateParent
                visible: false
            }
            PropertyChanges {
                target: pressed_StateParent
                visible: true
            }
            PropertyChanges {
                target: triggered_StateParent
                visible: false
            }
        },
        State {
            when: !__mouseArea.pressed && root.triggered
            name: "triggered"
            PropertyChanges {
                target: default_StateParent
                visible: false
            }
            PropertyChanges {
                target: pressed_StateParent
                visible: false
            }
            PropertyChanges {
                target: triggered_StateParent
                visible: true
            }
        }
        ]
    }

    //----------------------------------------------------------------------------------------------
}
