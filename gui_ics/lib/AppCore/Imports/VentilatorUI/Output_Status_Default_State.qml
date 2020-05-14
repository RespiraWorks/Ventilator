import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Text
//USES Rectangle

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //COLOR Figma::255_16_86_161
    property alias fi_BottomBar_color: fi_BottomBar.color
    //REAL 1
    property alias fi_BottomBar_opacity: fi_BottomBar.opacity
    //COLOR buttonAltTextColor
    property alias fi_Title_color: fi_Title.color
    //TEXTHAL 1
    property alias fi_Title_horizontalAlignment: fi_Title.horizontalAlignment
    //REAL 0.7000000000000001
    property alias fi_Title_opacity: fi_Title.opacity
    //TRANSLATABLE Text
    property alias fi_Title_text: fi_Title.text
    //TEXTVAL 128
    property alias fi_Title_verticalAlignment: fi_Title.verticalAlignment
    //COLOR Figma::255_16_86_161
    property alias fi_TopBar_color: fi_TopBar.color
    //REAL 1
    property alias fi_TopBar_opacity: fi_TopBar.opacity
    //COLOR buttonAltTextColor
    property alias fi_Value_color: fi_Value.color
    //TEXTHAL 4
    property alias fi_Value_horizontalAlignment: fi_Value.horizontalAlignment
    //REAL 1
    property alias fi_Value_opacity: fi_Value.opacity
    //TRANSLATABLE 00
    property alias fi_Value_text: fi_Value.text
    //TEXTVAL 32
    property alias fi_Value_verticalAlignment: fi_Value.verticalAlignment

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 224
    height: 98
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Text {
        id: fi_Title
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 9
        anchors.top: parent.top
        anchors.topMargin: 5
        z: 1
        height: 24
        text: qsTr("Text")
        font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_20_75_0_0")
        color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        opacity: 0.7000000000000001
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
        id: fi_Value
        anchors.left: parent.left
        anchors.leftMargin: 63
        anchors.right: parent.right
        anchors.rightMargin: 58
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 21
        z: 2
        height: 43
        text: qsTr("00")
        font: GreenHouseThemeManager.theme.font("Figma::Noto Serif_NotoSerif-Bold_38_75_0_0")
        color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        opacity: 1
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
    Rectangle {
        id: fi_TopBar
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        z: 3
        height: 1
        color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
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
    Rectangle {
        id: fi_BottomBar
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 4
        height: 1
        color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
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

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
