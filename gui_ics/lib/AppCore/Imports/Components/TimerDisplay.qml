import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/timerDisplay.png
Item {
    id: root

    //FONT textFont
    property alias font: label.font
    //COLOR textColor
    property alias color: label.color
    property int minutes: 0
    property bool addLeadingZero: true
    property bool negative: false
    //TEXTHAL 4
    property alias horizontalAlignment: label.horizontalAlignment

    implicitHeight: label.implicitHeight
    implicitWidth: label.contentWidth
    Text {
        id: label

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        color: "white"
        text: {
            var res = root.negative ? "-" : "";
            var h = Math.floor(root.minutes / 60);
            if (h < 10 && root.addLeadingZero)
                res += "0";
            res += h;
            res += ":"
            var m = Math.max(0, root.minutes - (h * 60));
            if (m < 10)
                res += "0"
            res += m;
            return res
        }

        elide: Text.ElideNone
        wrapMode: Text.NoWrap
        horizontalAlignment: Text.AlignHCenter
    }
}
