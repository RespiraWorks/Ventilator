import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Rectangle
//USES Image
//USES Text
//USES BorderImage
//USES Item

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    property int progressValue: 0
    //COLOR Figma::255_196_196_196
    property alias fi_Rectangle_21_color: fi_Rectangle_21___value_0.color
    //REAL 0
    property alias fi_Rectangle_21_opacity: fi_Rectangle_21___value_0.opacity
    //ASSET /245_822.png
    property alias fi_Union___value_0_source: fi_Union___value_0.source
    //REAL 1
    property alias fi_Union_opacity: fi_Union___value_0.opacity
    //ASSET /122_2244.png
    property alias fi_Union___value_100_source: fi_Union___value_100.source
    //ASSET /233_860.png
    property alias fi_Union___value_15_source: fi_Union___value_15.source
    //ASSET /233_824.png
    property alias fi_Union___value_20_source: fi_Union___value_20.source
    //ASSET /233_800.png
    property alias fi_Union___value_40_source: fi_Union___value_40.source
    //ASSET /233_786.png
    property alias fi_Union___value_60_source: fi_Union___value_60.source
    //ASSET /233_772.png
    property alias fi_Union___value_80_source: fi_Union___value_80.source
    //COLOR buttonAltTextColor
    property alias fi____value_0_color: fi____value_0.color
    //TEXTHAL 4
    property alias fi____value_0_horizontalAlignment: fi____value_0.horizontalAlignment
    //REAL 0.75
    property alias fi____value_0_opacity: fi____value_0.opacity
    //TRANSLATABLE !
    property alias fi____value_0_text: fi____value_0.text
    //TEXTVAL 32
    property alias fi____value_0_verticalAlignment: fi____value_0.verticalAlignment

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 72
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
        id: value_0_StateParent
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
            id: fi_Rectangle_21___value_0
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
        Image {
            id: fi_Union___value_0
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/245_822.png")
            fillMode: Image.Stretch
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        Text {
            id: fi____value_0
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.right: parent.right
            anchors.rightMargin: 32
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 2
            height: 20
            text: qsTr("!")
            font: GreenHouseThemeManager.theme.font("Figma::Source Sans Pro_SourceSansPro-Black_15_87_0_0")
            color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            opacity: 0.75
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            maximumLineCount: 2147483647
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_0
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 3
            source: GreenHouseThemeManager.theme.asset("/245_827.png")
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
    }
    Item {
        id: value_100_StateParent
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
            id: fi_Rectangle_21___value_100
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___value_0.color
            opacity: fi_Rectangle_21___value_0.opacity
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___value_100
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/122_2244.png")
            fillMode: Image.Stretch
            opacity: fi_Union___value_0.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_100
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 2
            source: GreenHouseThemeManager.theme.asset("/138_3294.png")
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
    }
    Item {
        id: value_15_StateParent
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
            id: fi_Rectangle_21___value_15
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___value_0.color
            opacity: fi_Rectangle_21___value_0.opacity
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___value_15
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/233_860.png")
            fillMode: Image.Stretch
            opacity: fi_Union___value_0.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_15
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 2
            source: GreenHouseThemeManager.theme.asset("/233_866.png")
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
    }
    Item {
        id: value_20_StateParent
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
            id: fi_Rectangle_21___value_20
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___value_0.color
            opacity: fi_Rectangle_21___value_0.opacity
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___value_20
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/233_824.png")
            fillMode: Image.Stretch
            opacity: fi_Union___value_0.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_20
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 2
            source: GreenHouseThemeManager.theme.asset("/233_831.png")
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
    }
    Item {
        id: value_40_StateParent
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
            id: fi_Rectangle_21___value_40
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___value_0.color
            opacity: fi_Rectangle_21___value_0.opacity
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___value_40
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/233_800.png")
            fillMode: Image.Stretch
            opacity: fi_Union___value_0.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_40
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 2
            source: GreenHouseThemeManager.theme.asset("/233_810.png")
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
    }
    Item {
        id: value_60_StateParent
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
            id: fi_Rectangle_21___value_60
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___value_0.color
            opacity: fi_Rectangle_21___value_0.opacity
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___value_60
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/233_786.png")
            fillMode: Image.Stretch
            opacity: fi_Union___value_0.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_60
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 2
            source: GreenHouseThemeManager.theme.asset("/233_796.png")
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
    }
    Item {
        id: value_80_StateParent
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
            id: fi_Rectangle_21___value_80
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: fi_Rectangle_21___value_0.color
            opacity: fi_Rectangle_21___value_0.opacity
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Image {
            id: fi_Union___value_80
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            z: 1
            height: 23
            source: GreenHouseThemeManager.theme.asset("/233_772.png")
            fillMode: Image.Stretch
            opacity: fi_Union___value_0.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        BorderImage {
            id: fi_Rectangle_469___value_80
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            z: 2
            source: GreenHouseThemeManager.theme.asset("/233_782.png")
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
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------
    StateGroup { //
        states: [
        State {
            when: root.progressValue < 15
            name: "value_0"
            PropertyChanges {
                target: value_0_StateParent
                visible: true
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: false
            }
        },
        State {
            when: root.progressValue >= 100
            name: "value_100"
            PropertyChanges {
                target: value_0_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: true
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: false
            }
        },
        State {
            when: root.progressValue >= 15 && root.progressValue < 20
            name: "value_15"
            PropertyChanges {
                target: value_0_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: true
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: false
            }
        },
        State {
            when: root.progressValue >= 20 && root.progressValue < 40
            name: "value_20"
            PropertyChanges {
                target: value_0_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: true
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: false
            }
        },
        State {
            when: root.progressValue >= 40 && root.progressValue < 60
            name: "value_40"
            PropertyChanges {
                target: value_0_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: true
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: false
            }
        },
        State {
            when: root.progressValue >= 60 && root.progressValue < 80
            name: "value_60"
            PropertyChanges {
                target: value_0_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: true
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: false
            }
        },
        State {
            when: root.progressValue >= 80 && root.progressValue < 100
            name: "value_80"
            PropertyChanges {
                target: value_0_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_100_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_15_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_20_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_40_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_60_StateParent
                visible: false
            }
            PropertyChanges {
                target: value_80_StateParent
                visible: true
            }
        }
        ]
    }

    //----------------------------------------------------------------------------------------------
}
