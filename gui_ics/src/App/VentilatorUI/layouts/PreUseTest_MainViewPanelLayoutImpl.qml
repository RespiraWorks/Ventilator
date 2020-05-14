import GreenHouse 1.0
import QtQuick 2.6
import VentilatorUI 1.0 as VentilatorUISet
import GreenHouse.Theming 1.0

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

PreUseTest_MainViewPanelLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 29
    y: fillParent ? 0 : 80
    width: fillParent ? (parent ? parent.width : 0) : 966
    height: fillParent ? (parent ? parent.height : 0) : 501

    inputFallthroughBlocked: true

    VentilatorUISet.BoundingBox {
        id: fi_BoundingBox
        x: 0
        y: 1
        z: 0
        width: 964
        height: 499
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignHCenter
        fi_Title_opacity: 0.85
        fi_Title_text: qsTr("Pre-Use Test")
        fi_Title_verticalAlignment: Text.AlignVCenter
        rotation: 0
        scale: 1
    }
    Text {
        id: fi_WarningLabel
        x: 148
        y: 219
        z: 1
        width: 667
        height: 102
        text: qsTr("Ventilation will be stopped.<br/>Disconnect Patient from circuit before beginning test.")
        font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_28_75_0_0")
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
    VentilatorUISet.Button_TextPrimary_Default_State {
        id: fi_StartTest
        x: 332
        y: 390
        z: 2
        width: 300
        height: 90
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Label_horizontalAlignment: Text.AlignHCenter
        fi_Label_opacity: 1
        fi_Label_text: qsTr("Start Test")
        fi_Label_verticalAlignment: Text.AlignVCenter
        rotation: 0
        scale: 1

        onClicked: {
            root.itemInterface.call_VentilatorParametersInterface_startPreUseTest()
        }
    }
    Item {
        id: fi_WarningStatus
        x: 4
        y: 69
        z: 3
        width: 960
        height: 148
        opacity: 1
        enabled: true
        visible: true
        clip: false
        rotation: 0
        scale: 1

        VentilatorUISet.WarningGlow {
            id: fi_WarningGlow
            x: 8
            y: 0
            z: 0
            width: 952
            height: 148
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Rectangle_480_opacity: 1
            fi_Rectangle_480_source: GreenHouseThemeManager.theme.asset("/210_143.png")
            rotation: 0
            scale: 1
        }
        Image {
            id: icon_Icons_Alarm
            x: 319
            y: 45
            z: 1
            width: 90
            height: 83
            source: GreenHouseThemeManager.theme.asset("/Icons/Alarm__DefaultState.png")
            fillMode: Image.Stretch
            opacity: 1
            enabled: true
            visible: true
            clip: false
            rotation: 0
            scale: 1
        }
        Text {
            id: fi_Warning
            x: 403
            y: 57
            z: 2
            width: 236
            height: 51
            text: qsTr("WARNING")
            font: GreenHouseThemeManager.theme.font("Figma::Lato_Bold_48_75_0_0")
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
    }

    showAnimation: PreUseTest_MainViewPanelLayoutShowAnimation { target: root }
    hideAnimation: PreUseTest_MainViewPanelLayoutHideAnimation { target: root }
    initAction: PreUseTest_MainViewPanelLayoutInitAnimation { target: root }

    itemInterface: PreUseTest_MainViewPanelIFaceImpl {}
}
