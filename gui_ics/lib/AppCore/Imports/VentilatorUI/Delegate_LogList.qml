import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0
import VentilatorUI 1.0 as VentilatorUISet

//USES Rectangle
//USES Text
//USES VentilatorUISet.PriorityIndicator
//USES Item

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    property int itemIndex: 0
    property int selectionIndex: -1
    //COLOR buttonAltTextColor
    property alias fi_DateLabel_color: fi_DateLabel___default.color
    //TEXTHAL 2
    property alias fi_DateLabel_horizontalAlignment: fi_DateLabel___default.horizontalAlignment
    //REAL 1
    property alias fi_DateLabel_opacity: fi_DateLabel___default.opacity
    //TRANSLATABLE 00/0/0000
    property alias fi_DateLabel_text: fi_DateLabel___default.text
    //TEXTVAL 32
    property alias fi_DateLabel_verticalAlignment: fi_DateLabel___default.verticalAlignment
    //COLOR buttonAltTextColor
    property alias fi_NameLabel_color: fi_NameLabel___default.color
    //TEXTHAL 1
    property alias fi_NameLabel_horizontalAlignment: fi_NameLabel___default.horizontalAlignment
    //REAL 1
    property alias fi_NameLabel_opacity: fi_NameLabel___default.opacity
    //TRANSLATABLE Label Name
    property alias fi_NameLabel_text: fi_NameLabel___default.text
    //TEXTVAL 128
    property alias fi_NameLabel_verticalAlignment: fi_NameLabel___default.verticalAlignment
    //TRANSLATABLE High
    property alias fi_PriorityIndicator_fi_StateLabel___high_text: fi_PriorityIndicator___default.fi_StateLabel___high_text
    //TRANSLATABLE Medium
    property alias fi_PriorityIndicator_fi_StateLabel___medium_init_text: fi_PriorityIndicator___default.fi_StateLabel___medium_init_text
    //COLOR buttonAltTextColor
    property alias fi_PriorityIndicator_fi_StateLabel_color: fi_PriorityIndicator___default.fi_StateLabel_color
    //TEXTHAL 1
    property alias fi_PriorityIndicator_fi_StateLabel_horizontalAlignment: fi_PriorityIndicator___default.fi_StateLabel_horizontalAlignment
    //REAL 1
    property alias fi_PriorityIndicator_fi_StateLabel_opacity: fi_PriorityIndicator___default.fi_StateLabel_opacity
    //TEXTVAL 128
    property alias fi_PriorityIndicator_fi_StateLabel_verticalAlignment: fi_PriorityIndicator___default.fi_StateLabel_verticalAlignment
    //BOOL false
    property alias fi_PriorityIndicator_high_flag: fi_PriorityIndicator___default.high_flag
    //ASSET /Icons/Indicator__HighState.png
    property alias fi_PriorityIndicator_icon___high_source: fi_PriorityIndicator___default.icon___high_source
    //ASSET /Icons/Indicator__MediumState.png
    property alias fi_PriorityIndicator_icon___medium_init_source: fi_PriorityIndicator___default.icon___medium_init_source
    //REAL 1
    property alias fi_PriorityIndicator_icon_opacity: fi_PriorityIndicator___default.icon_opacity
    //BOOL true
    property alias fi_PriorityIndicator_medium_init_flag: fi_PriorityIndicator___default.medium_init_flag
    //COLOR Figma::0_0_0_0
    property alias fi_Rectangle_479___default_color: fi_Rectangle_479___default.color
    //REAL 1
    property alias fi_Rectangle_479___default_opacity: fi_Rectangle_479___default.opacity
    //COLOR Figma::255_36_98_191
    property alias fi_Rectangle_479___selected_color: fi_Rectangle_479___selected.color
    //REAL 0.5
    property alias fi_Rectangle_479___selected_opacity: fi_Rectangle_479___selected.opacity
    //COLOR buttonAltTextColor
    property alias fi_TimeLabel_color: fi_TimeLabel___default.color
    //TEXTHAL 1
    property alias fi_TimeLabel_horizontalAlignment: fi_TimeLabel___default.horizontalAlignment
    //REAL 1
    property alias fi_TimeLabel_opacity: fi_TimeLabel___default.opacity
    //TRANSLATABLE 00:00
    property alias fi_TimeLabel_text: fi_TimeLabel___default.text
    //TEXTVAL 32
    property alias fi_TimeLabel_verticalAlignment: fi_TimeLabel___default.verticalAlignment

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 896
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
            id: fi_Rectangle_479___default
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            opacity: 1
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_NameLabel___default
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 556
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 1
            height: 33
            text: qsTr("Label Name")
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
        Text {
            id: fi_DateLabel___default
            anchors.left: parent.left
            anchors.leftMargin: 385
            anchors.right: parent.right
            anchors.rightMargin: 375
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 2
            height: 33
            text: qsTr("00/0/0000")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            opacity: 1
            horizontalAlignment: Text.AlignRight
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
        Text {
            id: fi_TimeLabel___default
            anchors.left: parent.left
            anchors.leftMargin: 571
            anchors.right: parent.right
            anchors.rightMargin: 252
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 3
            height: 33
            text: qsTr("00:00")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            opacity: 1
            horizontalAlignment: Text.AlignLeft
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
        VentilatorUISet.PriorityIndicator {
            id: fi_PriorityIndicator___default
            anchors.left: parent.left
            anchors.leftMargin: 718
            anchors.right: parent.right
            anchors.rightMargin: -9
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 4
            height: 33
            opacity: 1
            enabled: true
            visible: true
            clip: true
            fi_StateLabel___high_text: qsTr("High")
            fi_StateLabel___medium_init_text: qsTr("Medium")
            fi_StateLabel_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_StateLabel_horizontalAlignment: Text.AlignLeft
            fi_StateLabel_opacity: 1
            fi_StateLabel_verticalAlignment: Text.AlignVCenter
            high_flag: false
            icon___high_source: GreenHouseThemeManager.theme.asset("/Icons/Indicator__HighState.png")
            icon___medium_init_source: GreenHouseThemeManager.theme.asset("/Icons/Indicator__MediumState.png")
            icon_opacity: 1
            medium_init_flag: true
            rotation: 0
            scale: 1
        }
    }
    Item {
        id: selected_StateParent
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
            id: fi_Rectangle_479___selected
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 0
            color: GreenHouseThemeManager.theme.color("Figma::255_36_98_191")
            opacity: 0.5
            border.color: GreenHouseThemeManager.theme.color("Figma::0_0_0_0")
            border.width: 1
            enabled: true
            visible: true
            clip: false
            radius: 0
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_NameLabel___selected
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 556
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 1
            height: 33
            text: fi_NameLabel___default.text
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: fi_NameLabel___default.color
            opacity: fi_NameLabel___default.opacity
            horizontalAlignment: fi_NameLabel___default.horizontalAlignment
            verticalAlignment: fi_NameLabel___default.verticalAlignment
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            maximumLineCount: 2147483647
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_DateLabel___selected
            anchors.left: parent.left
            anchors.leftMargin: 385
            anchors.right: parent.right
            anchors.rightMargin: 375
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 2
            height: 33
            text: fi_DateLabel___default.text
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: fi_DateLabel___default.color
            opacity: fi_DateLabel___default.opacity
            horizontalAlignment: fi_DateLabel___default.horizontalAlignment
            verticalAlignment: fi_DateLabel___default.verticalAlignment
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            maximumLineCount: 2147483647
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_TimeLabel___selected
            anchors.left: parent.left
            anchors.leftMargin: 571
            anchors.right: parent.right
            anchors.rightMargin: 252
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 3
            height: 33
            text: fi_TimeLabel___default.text
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
            color: fi_TimeLabel___default.color
            opacity: fi_TimeLabel___default.opacity
            horizontalAlignment: fi_TimeLabel___default.horizontalAlignment
            verticalAlignment: fi_TimeLabel___default.verticalAlignment
            elide: Text.ElideNone
            wrapMode: Text.WordWrap
            maximumLineCount: 2147483647
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        VentilatorUISet.PriorityIndicator {
            id: fi_PriorityIndicator___selected
            anchors.left: parent.left
            anchors.leftMargin: 718
            anchors.right: parent.right
            anchors.rightMargin: -9
            anchors.top: parent.top
            anchors.topMargin: 12
            z: 4
            height: 33
            opacity: 1
            enabled: true
            visible: true
            clip: true
            fi_StateLabel___high_text: fi_PriorityIndicator___default.fi_StateLabel___high_text
            fi_StateLabel___medium_init_text: fi_PriorityIndicator___default.fi_StateLabel___medium_init_text
            fi_StateLabel_color: fi_PriorityIndicator___default.fi_StateLabel_color
            fi_StateLabel_horizontalAlignment: fi_PriorityIndicator___default.fi_StateLabel_horizontalAlignment
            fi_StateLabel_opacity: fi_PriorityIndicator___default.fi_StateLabel_opacity
            fi_StateLabel_verticalAlignment: fi_PriorityIndicator___default.fi_StateLabel_verticalAlignment
            high_flag: fi_PriorityIndicator___default.high_flag
            icon___high_source: fi_PriorityIndicator___default.icon___high_source
            icon___medium_init_source: fi_PriorityIndicator___default.icon___medium_init_source
            icon_opacity: fi_PriorityIndicator___default.icon_opacity
            medium_init_flag: fi_PriorityIndicator___default.medium_init_flag
            rotation: 0
            scale: 1
        }
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------
    StateGroup { //
        states: [
        State {
            when: !(root.selectionIndex == root.itemIndex)
            name: "default"
            PropertyChanges {
                target: default_StateParent
                visible: true
            }
            PropertyChanges {
                target: selected_StateParent
                visible: false
            }
        },
        State {
            when: (root.selectionIndex == root.itemIndex)
            name: "selected"
            PropertyChanges {
                target: default_StateParent
                visible: false
            }
            PropertyChanges {
                target: selected_StateParent
                visible: true
            }
        }
        ]
    }

    //----------------------------------------------------------------------------------------------
}
