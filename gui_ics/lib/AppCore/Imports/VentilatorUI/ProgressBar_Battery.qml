import GreenHouse 1.0
import QtQuick 2.6
import GreenHouse.Theming 1.0
import Components 1.0 as ComponentsSet

//USES ComponentsSet.ProgressIndicator

//EDITABLE TRUE
Item {
    id: root

    //Exposed properties----------------------------------------------------------------------------
    //BOOL false
    property alias progressBar_labelVisible: progressBar.labelVisible
    //INT 100
    property alias progressBar_max: progressBar.max
    //INT 0
    property alias progressBar_min: progressBar.min
    //INT 25
    property alias progressBar_value: progressBar.value

    //----------------------------------------------------------------------------------------------

    //Exposed signals-------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    //Local bindings--------------------------------------------------------------------------------
    x: 0
    y: 0
    z: 0
    width: 35
    height: 14
    opacity: 1
    enabled: true
    visible: true
    clip: true
    rotation: 0
    scale: 1

    //----------------------------------------------------------------------------------------------

    //Children--------------------------------------------------------------------------------------
    ComponentsSet.ProgressIndicator {
        id: progressBar
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        z: 0
        backgroundSource: GreenHouseThemeManager.theme.asset("/255_994.png")
        indicatorSource: GreenHouseThemeManager.theme.asset("/255_995.png")
        textFont: GreenHouseThemeManager.theme.font("textFont")
        textColor: GreenHouseThemeManager.theme.color("textColor")
        opacity: 1
        min: 0
        max: 100
        value: 25
        enabled: true
        visible: true
        bottomBackgroundAssetBorder: 2
        bottomIndicatorAssetBorder: 2
        clip: true
        fillMargin: 1
        indicatorAssetHorizontalOverflows: 0
        labelAnchorBottom: false
        labelAnchorHCenter: false
        labelAnchorLeft: false
        labelAnchorRight: false
        labelAnchorTop: false
        labelAnchorVCenter: false
        labelVisible: false
        leftBackgroundAssetBorder: 2
        leftIndicatorAssetBorder: 2
        parentBackground: false
        parentIndicator: false
        rightBackgroundAssetBorder: 2
        rightIndicatorAssetBorder: 2
        rotation: 0
        scale: 1
        topBackgroundAssetBorder: 2
        topIndicatorAssetBorder: 2
        valuePrefix: qsTr("")
        valueSuffix: qsTr("")
    }

    //----------------------------------------------------------------------------------------------

    //States----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
}
