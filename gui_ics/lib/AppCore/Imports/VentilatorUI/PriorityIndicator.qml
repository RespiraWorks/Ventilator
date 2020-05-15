import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Text
//USES Image
//USES Item

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    property bool high_flag: false
    property bool medium_init_flag: true
    //TRANSLATABLE High
    property alias fi_StateLabel___high_text: fi_StateLabel___high.text
    //COLOR buttonAltTextColor
    property alias fi_StateLabel_color: fi_StateLabel___high.color
    //TEXTHAL 1
    property alias fi_StateLabel_horizontalAlignment: fi_StateLabel___high.horizontalAlignment
    //REAL 1
    property alias fi_StateLabel_opacity: fi_StateLabel___high.opacity
    //TEXTVAL 128
    property alias fi_StateLabel_verticalAlignment: fi_StateLabel___high.verticalAlignment
    //TRANSLATABLE Medium
    property alias fi_StateLabel___medium_init_text: fi_StateLabel___medium_init.text
    //ASSET /Icons/Indicator__HighState.png
    property alias icon___high_source: icon___high.source
    //REAL 1
    property alias icon_opacity: icon___high.opacity
    //ASSET /Icons/Indicator__MediumState.png
    property alias icon___medium_init_source: icon___medium_init.source

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 187
    height: 33
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Item {
        id: high_StateParent
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
        visible: false
        clip: true
        rotation: 0
        scale: 1

        Text {
            id: fi_StateLabel___high
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            text: qsTr("High")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            opacity: 1
            horizontalAlignment: Text.AlignLeft
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
        Image {
            id: icon___high
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            z: 1
            width: 20
            height: 20
            source: GreenHouseThemeManager.theme.asset("/Icons/Indicator__HighState.png")
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
        id: medium_init_StateParent
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
        visible: false
        clip: true
        rotation: 0
        scale: 1

        Text {
            id: fi_StateLabel___medium_init
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            text: qsTr("Medium")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: fi_StateLabel___high.color
            opacity: fi_StateLabel___high.opacity
            horizontalAlignment: fi_StateLabel___high.horizontalAlignment
            verticalAlignment: fi_StateLabel___high.verticalAlignment
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            maximumLineCount: 2147483647
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        Image {
            id: icon___medium_init
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            z: 1
            width: 20
            height: 20
            source: GreenHouseThemeManager.theme.asset("/Icons/Indicator__MediumState.png")
            fillMode: Image.Stretch
            opacity: icon___high.opacity
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------
    StateGroup { //  generic_high
        states: [
        State {
            when: root.high_flag
            name: "high"
            PropertyChanges {
                target: high_StateParent
                visible: true
                z: 1
            }
        }
        ]
    }
    StateGroup { //  generic_medium_init
        states: [
        State {
            when: root.medium_init_flag
            name: "medium_init"
            PropertyChanges {
                target: medium_init_StateParent
                visible: true
                z: 1
            }
        }
        ]
    }

    //----------------------------------------------------------------------------------------------
}
