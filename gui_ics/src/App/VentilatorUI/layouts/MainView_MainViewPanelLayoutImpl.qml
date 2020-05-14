import GreenHouse 1.0
import QtQuick 2.6
import Decorative 1.0 as DecorativeSet
import GreenHouse.Theming 1.0
import VentilatorUI 1.0 as VentilatorUISet
import Components 1.0 as ComponentsSet
import MedicalPlotting 1.0 as MedicalPlottingSet

import "qrc:/VentilatorUI/private/"
import "qrc:/VentilatorUI/animations/"
import "qrc:/VentilatorUI/interfaces/"

import com.ics.ventilatorparameters 1.0

MainView_MainViewPanelLayout {
    id: root

    property bool fillParent: false
    x: fillParent ? 0 : 0
    y: fillParent ? 0 : 60
    width: fillParent ? (parent ? parent.width : 0) : 1024
    height: fillParent ? (parent ? parent.height : 0) : 540

    inputFallthroughBlocked: true

    DecorativeSet.DecoratedItem {
        id: fi_VizualizerFrameDecoration
        x: 20
        y: 0
        z: 1
        width: 984
        height: 388
        source: GreenHouseThemeManager.theme.asset("/194_2075.png")
        opacity: 1
        enabled: true
        visible: true
        clip: true
        horizontalCenterOffset: 0
        rotation: 0
        scale: 1
        verticalCenterOffset: 0
    }
    VentilatorUISet.WaveFormContainer {
        id: fi_FlowWaveFormContainer
        x: 20
        y: 0
        z: 2
        width: 760
        height: 130
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("Flow (L/min)")
        fi_Title_verticalAlignment: Text.AlignVCenter
        rotation: 0
        scale: 1
    }
    VentilatorUISet.WaveFormContainer {
        id: fi_VolumeWaveFormContainer
        x: 20
        y: 129
        z: 3
        width: 760
        height: 130
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("Volume (mL)")
        fi_Title_verticalAlignment: Text.AlignVCenter
        rotation: 0
        scale: 1
    }
    VentilatorUISet.WaveFormContainer {
        id: fi_PressureWaveFormContainer
        x: 20
        y: 258
        z: 4
        width: 760
        height: 130
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("Pressure (cmH2O)")
        fi_Title_verticalAlignment: Text.AlignVCenter
        rotation: 0
        scale: 1
    }
    VentilatorUISet.Output_Status_Default_State {
        id: fi_OutputRate
        x: 780
        y: -1
        z: 5
        width: 224
        height: 98
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_BottomBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_BottomBar_opacity: 1
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("Rate")
        fi_Title_verticalAlignment: Text.AlignVCenter
        fi_TopBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_TopBar_opacity: 1
        fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Value_horizontalAlignment: Text.AlignHCenter
        fi_Value_opacity: 1
        fi_Value_text: VentilatorParametersInterface.currentRate
        fi_Value_verticalAlignment: Text.AlignTop
        rotation: 0
        scale: 1
    }
    VentilatorUISet.Output_Status_Default_State {
        id: fi_OutputPEEP
        x: 780
        y: 96
        z: 6
        width: 224
        height: 98
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_BottomBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_BottomBar_opacity: 1
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("PEEP")
        fi_Title_verticalAlignment: Text.AlignVCenter
        fi_TopBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_TopBar_opacity: 1
        fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Value_horizontalAlignment: Text.AlignHCenter
        fi_Value_opacity: 1
        fi_Value_text: VentilatorParametersInterface.currentPeep
        fi_Value_verticalAlignment: Text.AlignTop
        rotation: 0
        scale: 1
    }
    VentilatorUISet.Output_Status_Default_State {
        id: fi_OutputTidalVolume
        x: 780
        y: 193
        z: 7
        width: 224
        height: 98
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_BottomBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_BottomBar_opacity: 1
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("Tidal Volume")
        fi_Title_verticalAlignment: Text.AlignVCenter
        fi_TopBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_TopBar_opacity: 1
        fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Value_horizontalAlignment: Text.AlignHCenter
        fi_Value_opacity: 1
        fi_Value_text: VentilatorParametersInterface.currentTidalV
        fi_Value_verticalAlignment: Text.AlignTop
        rotation: 0
        scale: 1
    }
    VentilatorUISet.Output_Status_Default_State {
        id: fi_OutputAdditionalValue
        x: 780
        y: 290
        z: 8
        width: 224
        height: 98
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_BottomBar_color: GreenHouseThemeManager.theme.color("Figma::255_8_85_162")
        fi_BottomBar_opacity: 1
        fi_Title_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Title_horizontalAlignment: Text.AlignLeft
        fi_Title_opacity: 0.7000000000000001
        fi_Title_text: qsTr("Additional Value")
        fi_Title_verticalAlignment: Text.AlignVCenter
        fi_TopBar_color: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
        fi_TopBar_opacity: 1
        fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
        fi_Value_horizontalAlignment: Text.AlignHCenter
        fi_Value_opacity: 1
        fi_Value_text: VentilatorParametersInterface.currentAdditionalValue
        fi_Value_verticalAlignment: Text.AlignTop
        rotation: 0
        scale: 1
    }
    ComponentsSet.FlickableRow {
        id: fi_bottomButtonFrame
        x: 110
        y: 414
        z: 12
        width: 208 * Math.min(4, visibleElementsCount()) - 28
        height: 100
        spacing: 28
        layoutDirection: Qt.LeftToRight
        opacity: 1
        enabled: true
        visible: true
        clip: true
        stopAtBounds: true

        interactive: false
        anchors.horizontalCenter: parent.horizontalCenter

        property int currentIndex: 0
        onCurrentIndexChanged: {
            if (currentIndex >= 0 && currentIndex < rowContents.length) {
                contentX = Math.max(0, Math.min(contentWidth - width, rowContents[currentIndex].x))
            }
        }

        function scrollLeft() {
            if ((currentIndex > 0) && !atXBeginning) {
                --currentIndex
            }
        }

        function scrollRight() {
            if ((currentIndex < rowContents.length - 1) && !atXEnd) {
                ++currentIndex
            }
        }

        function visibleElementsCount() {
            var count = 0;
            for (var index = 0; index < rowContents.length; ++index) {
                if (rowContents[index].visible) {
                    ++count
                }
            }

            return count
        }

        Behavior on contentX {
            NumberAnimation {
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }

        VentilatorUISet.Button_Carousel_Default_State {
            id: fi_ButtonTidal
            y: 0
            z: 0
            width: 180
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignLeft
            fi_Label_opacity: 0.6
            fi_Label_text: qsTr("Tidal V (cm)")
            fi_Label_verticalAlignment: Text.AlignVCenter
            fi_Rectangle_468_opacity: 1
            fi_Rectangle_468_source: GreenHouseThemeManager.theme.asset("/138_618.png")
            fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Value_horizontalAlignment: Text.AlignHCenter
            fi_Value_opacity: 1
            fi_Value_text: VentilatorParametersInterface.tidalV
            fi_Value_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.set_VentilatorParametersInterface_tidalVpopup(VentilatorParametersInterface.tidalV)
                root.itemInterface.surface_Popups_MoveTo_AdjustTidalV()
            }
        }
        VentilatorUISet.Button_Carousel_Default_State {
            id: fi_ButtonPEEP
            y: 0
            z: 1
            width: 180
            height: 100
            opacity: 1
            enabled: true
            visible: ((VentilatorParametersInterface.acpMode === VentilatorParameters.ACPMode)
                      || (VentilatorParametersInterface.acpMode === VentilatorParameters.PCVMode))
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignLeft
            fi_Label_opacity: 0.6
            fi_Label_text: qsTr("PEEP (cmH20) ")
            fi_Label_verticalAlignment: Text.AlignVCenter
            fi_Rectangle_468_opacity: 1
            fi_Rectangle_468_source: GreenHouseThemeManager.theme.asset("/138_618.png")
            fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Value_horizontalAlignment: Text.AlignHCenter
            fi_Value_opacity: 1
            fi_Value_text: VentilatorParametersInterface.peep
            fi_Value_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.set_VentilatorParametersInterface_peepPopup(VentilatorParametersInterface.peep)
                root.itemInterface.surface_Popups_MoveTo_AdjustPEEP()
            }
        }
        VentilatorUISet.Button_Carousel_Default_State {
            id: fi_ButtonRRMin
            y: 0
            z: 2
            width: 180
            height: 100
            opacity: 1
            enabled: true
            visible: VentilatorParametersInterface.acpMode === VentilatorParameters.ACPMode
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignLeft
            fi_Label_opacity: 0.6
            fi_Label_text: qsTr("RR-Min (BPM)")
            fi_Label_verticalAlignment: Text.AlignVCenter
            fi_Rectangle_468_opacity: 1
            fi_Rectangle_468_source: GreenHouseThemeManager.theme.asset("/138_618.png")
            fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Value_horizontalAlignment: Text.AlignHCenter
            fi_Value_opacity: 1
            fi_Value_text: VentilatorParametersInterface.rrMin
            fi_Value_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1

            onClicked: {
                root.itemInterface.set_VentilatorParametersInterface_rrMinPopup(VentilatorParametersInterface.rrMin)
                root.itemInterface.surface_Popups_MoveTo_AdjustRRMin()
            }
        }
        VentilatorUISet.Button_Carousel_Default_State {
            id: fi_ButtonRatio
            y: 0
            z: 3
            width: 180
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignLeft
            fi_Label_opacity: 0.6
            fi_Label_text: qsTr("I:E Ratio")
            fi_Label_verticalAlignment: Text.AlignVCenter
            fi_Rectangle_468_opacity: 1
            fi_Rectangle_468_source: GreenHouseThemeManager.theme.asset("/138_618.png")
            fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Value_horizontalAlignment: Text.AlignHCenter
            fi_Value_opacity: 1
            fi_Value_text: qsTr("1:2")
            fi_Value_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
//TODO Remove fi_ButtonTest (added for test scroll of components in bottom row)
        VentilatorUISet.Button_Carousel_Default_State {
            id: fi_ButtonTest
            y: 0
            z: 33
            width: 180
            height: 100
            opacity: 1
            enabled: true
            visible: true
            clip: false
            fi_Label_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Label_horizontalAlignment: Text.AlignLeft
            fi_Label_opacity: 0.6
            fi_Label_text: qsTr("Test element")
            fi_Label_verticalAlignment: Text.AlignVCenter
            fi_Rectangle_468_opacity: 1
            fi_Rectangle_468_source: GreenHouseThemeManager.theme.asset("/138_618.png")
            fi_Value_color: GreenHouseThemeManager.theme.color("buttonAltTextColor")
            fi_Value_horizontalAlignment: Text.AlignHCenter
            fi_Value_opacity: 1
            fi_Value_text: qsTr("0000")
            fi_Value_verticalAlignment: Text.AlignVCenter
            rotation: 0
            scale: 1
        }
    }
    Item {
        id: fi_FlowWaveForm
        x: 20
        y: 0
        z: 9
        width: 761
        height: 128
        opacity: 1
        enabled: true
        visible: true
        clip: true
        rotation: 0
        scale: 1

        MedicalPlottingSet.GlowingPlot {
            id: glowingplot0
            x: 0.9090908893872438
            y: 34
            z: 0
            width: 758
            height: 88
            opacity: 1
            enabled: true
            visible: true
            clip: false
            dataPath: ":/MedicalPlottingDemoData/b0_3s.txt"
            dottedLine: false
            glowOpacity: 0.5
            glowRadius: 12
            leftFadeStart: 0.05
            plotColor: GreenHouseThemeManager.theme.color("Pressure Wave")
            plotScale: 2500
            rotation: 0
            scale: 1
            waveformPlotType: 0
            waveformType: 0
            waveformWidth: 2
        }
    }
    Item {
        id: fi_VolumeWaveForm
        x: 20
        y: 129
        z: 10
        width: 760
        height: 129
        opacity: 1
        enabled: true
        visible: true
        clip: true
        rotation: 0
        scale: 1

        MedicalPlottingSet.GlowingPlot {
            id: glowingplot1
            x: 4
            y: 35
            z: 0
            width: 752
            height: 87
            opacity: 1
            enabled: true
            visible: true
            clip: false
            dataPath: ":/MedicalPlottingDemoData/b0_3s.txt"
            dottedLine: false
            glowOpacity: 0.5
            glowRadius: 12
            leftFadeStart: 0.05
            plotColor: GreenHouseThemeManager.theme.color("Figma::255_16_86_161")
            plotScale: 2500
            rotation: 0
            scale: 1
            waveformPlotType: 0
            waveformType: 1
            waveformWidth: 2
        }
    }
    Item {
        id: fi_PressureWaveForm
        x: 20
        y: 259
        z: 11
        width: 760
        height: 129
        opacity: 1
        enabled: true
        visible: true
        clip: true
        rotation: 0
        scale: 1

        MedicalPlottingSet.GlowingPlot {
            id: glowingplot2
            x: 0.9090908893872438
            y: 34
            z: 0
            width: 754
            height: 89
            opacity: 1
            enabled: true
            visible: true
            clip: false
            dataPath: ":/MedicalPlottingDemoData/b0_3s.txt"
            dottedLine: false
            glowOpacity: 0.5
            glowRadius: 12
            leftFadeStart: 0.05
            plotColor: GreenHouseThemeManager.theme.color("shadowRectColor")
            plotScale: 2500
            rotation: 0
            scale: 1
            waveformPlotType: 0
            waveformType: 2
            waveformWidth: 2
        }
    }
    VentilatorUISet.PanelBackground {
        id: fi_PanelBackground
        x: 0
        y: 388
        z: 0
        width: 1024
        height: 152
        opacity: 1
        enabled: true
        visible: true
        clip: false
        fi_Rectangle_483_opacity: 1
        fi_Rectangle_483_source: GreenHouseThemeManager.theme.asset("/256_813.png")
        rotation: 0
        scale: 1

        VentilatorUISet.Button_ArrowLeft_Default_State {
            id: button_arrowleft_default_state0
            x: 20
            y: 46
            z: 0
            width: 60
            height: 60
            opacity: fi_bottomButtonFrame.atXBeginning ? 0.25 : 1
            enabled: true
            visible: true
            clip: false
            icon_opacity: 1
            icon_source: GreenHouseThemeManager.theme.asset("/Icons/ArrowLeft__DefaultState.png")
            rotation: 0
            scale: 1

            onClicked: {
                fi_bottomButtonFrame.scrollLeft()
            }
        }
        VentilatorUISet.Button_ArrowRight_Default_State {
            id: button_arrowright_default_state0
            x: 944
            y: 46
            z: 0
            width: 60
            height: 60
            opacity: fi_bottomButtonFrame.atXEnd ? 0.25 : 1
            enabled: true
            visible: true
            clip: false
            icon_opacity: 1
            icon_source: GreenHouseThemeManager.theme.asset("/Icons/ArrowRight__DefaultState.png")
            rotation: 0
            scale: 1

            onClicked: {
                fi_bottomButtonFrame.scrollRight()
            }
        }
    }

    showAnimation: MainView_MainViewPanelLayoutShowAnimation { target: root }
    hideAnimation: MainView_MainViewPanelLayoutHideAnimation { target: root }
    initAction: MainView_MainViewPanelLayoutInitAnimation { target: root }

    itemInterface: MainView_MainViewPanelIFaceImpl {}
}
