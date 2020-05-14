import QtQuick 2.6

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/switch.png
Item {
    id: root

    width: 128
    height: 32

    property bool checked: false
    //BOOL true
    property alias checkable: ma.enabled
    //TRANSLATABLE Lorem
    property alias text: label.text
    //FONT textFont
    property alias font: label.font
    //COLOR textColor
    property color textColor: "white"
    //TEXTWRAPM 0
    property alias wrapMode: label.wrapMode
    //TEXTELIDEM 1
    property alias elide: label.elide
    //TEXTHAL 4
    property alias horizontalAlignment: label.horizontalAlignment
    //TEXTVAL 32
    property alias verticalAlignment: label.verticalAlignment

    property int backgroundOverflow: 0
    property int backgroundHoveredOverflow: 4
    property int backgroundBorderSize: 5
    property int backgroundHoveredBorderSize: 9

    //ASSET /placeholders/switch/bg.png
    property string backgroundAsset: ""
    //ASSET /placeholders/switch/bg_hover.png
    property string backgroundHoveredAsset: ""
    //ASSET /placeholders/switch/handle.png
    property string handleAsset: ""
    //ASSET /placeholders/switch/handle_active.png
    property string handleActiveAsset: ""

    signal clicked

    Text {
        id: label

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: switchItem.left
        anchors.rightMargin: 5

        elide: Text.ElideRight
        wrapMode: Text.NoWrap

        color: root.textColor
    }

    Item {
        id: switchItem

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        width: implicitWidth
        height: implicitHeight
        implicitWidth: 32
        implicitHeight: 17

        BorderImage {
            id: bg

            anchors.fill: parent
            property int overflow: 0
            anchors.margins: -overflow

            property int borderSize: 5
            border.left: borderSize
            border.right: borderSize
            border.top: borderSize
            border.bottom: borderSize
        }

        MouseArea {
            id: ma
            anchors.fill: parent

            hoverEnabled: true
            onClicked: {
                root.clicked()
            }
        }

        Image {
            id: handle

            anchors.verticalCenter: parent.verticalCenter
            x: root.checked ? switchItem.width - handle.width - 2 : 2
            Behavior on x {
                NumberAnimation { duration: 250; }
            }

            source: root.handleAsset

            Image {
                anchors.centerIn: parent

                source: root.handleActiveAsset
                opacity: root.checked ? 1.0 : 0.0
                Behavior on opacity {
                    NumberAnimation { duration: 500 }
                }
            }
        }

        states: [
            State {
                when: !ma.containsMouse
                PropertyChanges {
                    target: bg
                    source: root.backgroundAsset
                    overflow: root.backgroundOverflow
                    borderSize: root.backgroundBorderSize
                }
            },
            State {
                when:  ma.containsMouse
                PropertyChanges {
                    target: bg
                    source: root.backgroundHoveredAsset
                    overflow: root.backgroundHoveredOverflow
                    borderSize: root.backgroundHoveredBorderSize
                }
            }
        ]
    }
}
