import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES BorderImage
//USES Text

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //COLOR buttonAltTextColor
    property alias fi_Title_color: fi_Title.color
    //TEXTHAL 4
    property alias fi_Title_horizontalAlignment: fi_Title.horizontalAlignment
    //REAL 0.85
    property alias fi_Title_opacity: fi_Title.opacity
    //TRANSLATABLE Text
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
    width: 600
    height: 361
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    BorderImage {
        id: fi_BoundingBoxBackground_9seg_70_70_70_70
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        source: GreenHouseThemeManager.theme.asset("/233_976.png")
        opacity: 1
        enabled: true
        visible: true
        border.bottom: 70
        border.left: 70
        border.right: 70
        border.top: 70
        clip: false
        mirror: false
        rotation: 0
        scale: 1
    }
    Text {
        id: fi_Title
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        z: 1
        height: 40
        text: qsTr("Text")
        font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_24_75_0_0")
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

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
