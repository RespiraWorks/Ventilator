import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/spinBox.png
//PARENTING FALSE
Item {
    id: root

    //ASSET /placeholders/spin-background.png
    property alias backgroundAsset: background.source
    //ASSETFILLM
    property alias fillMode: background.fillMode

    width: 300
    height: 128

    Image {
        id: background

        anchors.fill: parent
    }

    //FONT spinBoxFont
    property alias font: label.font
    //COLOR spinBoxTextColor
    property alias textColor: label.color
    //TEXTHAL 4
    property alias horizontalAlignment: label.horizontalAlignment
    //TEXTVAL 128
    property alias verticalAlignment: label.verticalAlignment

    property int repeatInterval: 250
    property int decimals: 2
    property real min: 0.0
    property real max: 100.0
    property real value: 25.0
    property real step: 0.05
    signal valueSelected(real selectedValue)
    onValueChanged: valueSelected(value)

    function increment() {
        if (root.value + root.state <= root.max)
            root.value += root.step
        else
            root.value = root.max
    }

    function decrement() {
        if (root.value - root.state >= root.min)
            root.value -= root.step
        else
            root.value = root.min
    }

    Text {
        id: label

        anchors.fill: parent

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        text: root.value.toFixed(root.decimals)
    }

    //ASSET /placeholders/spin-up.png
    property string upArrowAsset: ""
    //ASSET /placeholders/spin-up-disabled.png
    property string upArrowDisabledAsset: ""
    Image {
        id: up

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.verticalCenter

        fillMode: Image.PreserveAspectFit
        source: enabled ? root.upArrowAsset : root.upArrowDisabledAsset
        enabled: root.value < root.max

        MouseArea {
            id: upMa

            anchors.fill: parent

            onClicked: {
                root.increment()
            }

            Timer {
                id: upTimer

                repeat: true
                interval: root.repeatInterval
                running: upMa.pressed && upMa.enabled
                onTriggered: {
                    root.increment()
                }
            }
        }
    }

    //ASSET /placeholders/spin-down.png
    property string downArrowAsset: ""
    //ASSET /placeholders/spin-down-disabled.png
    property string downArrowDisabledAsset: ""
    Image {
        id: down

        anchors.right: parent.right
        anchors.top: parent.verticalCenter
        anchors.bottom: parent.bottom

        fillMode: Image.PreserveAspectFit
        source: enabled ? root.downArrowAsset : root.downArrowDisabledAsset
        enabled: root.value > root.min

        MouseArea {
            id: downMa

            anchors.fill: parent

            onClicked: {
                root.decrement()
            }

            Timer {
                id: downTimer

                repeat: true
                interval: 250
                running: downMa.pressed && downMa.enabled
                onTriggered: {
                    root.decrement()
                }
            }
        }
    }
}
