import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0

//USES BorderImage
//USES Image
//USES Text
//USES MouseArea

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //COLOR buttonAltTextColor
    property alias fi_Label_color: fi_Label.color
    //TEXTHAL 1
    property alias fi_Label_horizontalAlignment: fi_Label.horizontalAlignment
    //REAL 0.6
    property alias fi_Label_opacity: fi_Label.opacity
    //TRANSLATABLE Text
    property alias fi_Label_text: fi_Label.text
    //TEXTVAL 128
    property alias fi_Label_verticalAlignment: fi_Label.verticalAlignment
    //REAL 1
    property alias fi_Rectangle_468_opacity: fi_Rectangle_468.opacity
    //ASSET /138_618.png
    property alias fi_Rectangle_468_source: fi_Rectangle_468.source
    //COLOR buttonAltTextColor
    property alias fi_Value_color: fi_Value.color
    //TEXTHAL 4
    property alias fi_Value_horizontalAlignment: fi_Value.horizontalAlignment
    //REAL 1
    property alias fi_Value_opacity: fi_Value.opacity
    //TRANSLATABLE 00
    property alias fi_Value_text: fi_Value.text
    //TEXTVAL 128
    property alias fi_Value_verticalAlignment: fi_Value.verticalAlignment

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
    width: 180
    height: 100
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    BorderImage {
        id: fi_Rectangle_15
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        source: GreenHouseThemeManager.theme.asset("/122_2253.png")
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
        id: fi_Rectangle_468
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        z: 1
        height: 34
        source: GreenHouseThemeManager.theme.asset("/138_618.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    Text {
        id: fi_Value
        anchors.left: parent.left
        anchors.leftMargin: 48
        anchors.right: parent.right
        anchors.rightMargin: 48
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 9
        z: 2
        height: 53
        text: qsTr("00")
        font: GreenHouseThemeManager.theme.font("Figma::Noto Serif_NotoSerif-Bold_38_75_0_0")
        color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        opacity: 1
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
    Text {
        id: fi_Label
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        z: 3
        height: 33
        text: qsTr("Text")
        font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_20_75_0_0")
        color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        opacity: 0.6
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

    //----------------------------------------------------------------------------------------------
}
