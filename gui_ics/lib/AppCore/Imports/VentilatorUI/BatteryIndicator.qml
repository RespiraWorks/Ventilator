import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0
import VentilatorUI 1.0 as VentilatorUISet

//USES Rectangle
//USES Image
//USES BorderImage
//USES VentilatorUISet.ProgressBar_Battery

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //BOOL false
    property alias fi_ProgressBar_Battery_progressBar_labelVisible: fi_ProgressBar_Battery.progressBar_labelVisible
    //INT 100
    property alias fi_ProgressBar_Battery_progressBar_max: fi_ProgressBar_Battery.progressBar_max
    //INT 0
    property alias fi_ProgressBar_Battery_progressBar_min: fi_ProgressBar_Battery.progressBar_min
    //INT 25
    property alias fi_ProgressBar_Battery_progressBar_value: fi_ProgressBar_Battery.progressBar_value
    //COLOR Figma::255_196_196_196
    property alias fi_Rectangle_21_color: fi_Rectangle_21.color
    //REAL 0
    property alias fi_Rectangle_21_opacity: fi_Rectangle_21.opacity
    //REAL 1
    property alias fi_Rectangle_28_opacity: fi_Rectangle_28.opacity
    //ASSET /728_30.png
    property alias fi_Rectangle_28_source: fi_Rectangle_28.source
    //REAL 1
    property alias fi_Union_opacity: fi_Union.opacity
    //ASSET /254_142.png
    property alias fi_Union_source: fi_Union.source

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 84
    height: 60
    opacity: 1
    enabled: true
    visible: true
    clip: false
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    Rectangle {
        id: fi_Rectangle_21
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
        id: fi_Union
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.right: parent.right
        anchors.rightMargin: 21
        anchors.top: parent.top
        anchors.topMargin: 18
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 18
        z: 1
        source: GreenHouseThemeManager.theme.asset("/254_142.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    Image {
        id: fi_Rectangle_28
        anchors.left: parent.left
        anchors.leftMargin: 64
        anchors.right: parent.right
        anchors.rightMargin: 18
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 24
        z: 2
        source: GreenHouseThemeManager.theme.asset("/728_30.png")
        fillMode: Image.Stretch
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1
    }
    BorderImage {
        id: fi_Rectangle_469
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
        z: 3
        source: GreenHouseThemeManager.theme.asset("/254_152.png")
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
    VentilatorUISet.ProgressBar_Battery {
        id: fi_ProgressBar_Battery
        anchors.left: parent.left
        anchors.leftMargin: 23
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.top: parent.top
        anchors.topMargin: 23
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 23
        z: 4
        opacity: 1
        enabled: true
        visible: true
        clip: true
        progressBar_labelVisible: false
        progressBar_max: 100
        progressBar_min: 0
        progressBar_value: 25
        rotation: 0
        scale: 1
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
