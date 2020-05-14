import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES Text

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //COLOR buttonAltTextColor
    property alias fi_Title_color: fi_Title.color
    //TEXTHAL 1
    property alias fi_Title_horizontalAlignment: fi_Title.horizontalAlignment
    //REAL 0.7000000000000001
    property alias fi_Title_opacity: fi_Title.opacity
    //TRANSLATABLE Flow (L/min)
    property alias fi_Title_text: fi_Title.text
    //TEXTVAL 128
    property alias fi_Title_verticalAlignment: fi_Title.verticalAlignment

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 750
    height: 130
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
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 4
        z: 1
        height: 24
        text: qsTr("Flow (L/min)")
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

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
