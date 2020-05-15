import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/button.png
MouseArea {
    id: root

    //TRANSLATABLE Button
    property string buttonText: ""
    //FONT buttonFont
    property alias font: label.font
    //TEXTHAL 4
    property alias horizontalAlignment: label.horizontalAlignment
    //TEXTVAL 128
    property alias verticalAlignment: label.verticalAlignment

    //ASSET /placeholders/button/default.png
    property string defaultBackgroundAsset: ""
    //ASSET /placeholders/button/disabled.png
    property string disabledBackgroundAsset: ""
    //ASSET /placeholders/button/hover.png
    property string hoverBackgroundAsset: ""
    //ASSET /placeholders/button/pressed.png
    property string pressedBackgroundAsset: ""
    //ASSET /placeholders/button/pressedselected.png
    property string pressedSelectedBackgroundAsset: ""
    //ASSET /placeholders/button/selected.png
    property string selectedBackgroundAsset: ""

    //COLOR buttonTextColor
    property color textColor: "black"
    //COLOR buttonDisabledTextColor
    property color disabledTextColor: "white"
    //COLOR buttonAltTextColor
    property color hoveredTextColor: "white"
    //COLOR buttonAltTextColor
    property color pressedTextColor: "white"
    //COLOR buttonAltTextColor
    property color pressedSelectedTextColor: "white"
    //COLOR buttonTextColor
    property color selectedTextColor: "white"

    property int itemIndex: 0
    property int selectionIndex: -1

    property int leftAssetBorder: 5
    property int rightAssetBorder: 5
    property int topAssetBorder: 5
    property int bottomAssetBorder: 5

    width: 150
    height: 50

    //HIDE property hoverEnabled
    hoverEnabled: true

    signal triggered()
    onClicked: root.triggered()

    BorderImage {
        id: bg

        anchors.fill: parent

        border {
            left: root.leftAssetBorder
            right: root.rightAssetBorder
            top: root.topAssetBorder
            bottom: root.bottomAssetBorder
        }

        source: root.defaultBackgroundAsset

        states: [
            State {
                name: "defaultState"
                when: root.enabled && !(root.containsMouse && root.pressed) && root.selectionIndex != root.itemIndex
                PropertyChanges {
                    target: bg
                    source: root.defaultBackgroundAsset
                }
                PropertyChanges {
                    target: label
                    color: root.textColor
                }
            },
            State {
                name: "disabled"
                when: !root.enabled
                PropertyChanges {
                    target: bg
                    source: root.disabledBackgroundAsset
                }
                PropertyChanges {
                    target: label
                    color: root.disabledTextColor
                }
            },
            State {
                name: "hovered"
                when: root.enabled && root.containsMouse && !root.pressed && root.selectionIndex != root.itemIndex
                PropertyChanges {
                    target: bg
                    source: root.hoverBackgroundAsset
                }
                PropertyChanges {
                    target: label
                    color: root.hoveredTextColor
                }
            },
            State {
                name: "pressed"
                when: root.enabled && root.containsMouse && root.pressed && root.selectionIndex != root.itemIndex
                PropertyChanges {
                    target: bg
                    source: root.pressedBackgroundAsset
                }
                PropertyChanges {
                    target: label
                    color: root.pressedTextColor
                }
            },
            State {
                name: "pressedSelected"
                when: root.enabled && root.containsMouse && root.pressed && root.selectionIndex == root.itemIndex
                PropertyChanges {
                    target: bg
                    source: root.pressedSelectedBackgroundAsset
                }
                PropertyChanges {
                    target: label
                    color: root.pressedSelectedTextColor
                }
            },
            State {
                name: "selected"
                when: root.enabled && !root.pressed && root.selectionIndex == root.itemIndex
                PropertyChanges {
                    target: bg
                    source: root.pressedSelectedBackgroundAsset
                }
                PropertyChanges {
                    target: label
                    color: root.selectedTextColor
                }
            }
        ]

        Text {
            id: label

            anchors.fill: parent

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            elide: Text.ElideRight

            font.pixelSize: 16

            text: root.buttonText
        }
    }
}
