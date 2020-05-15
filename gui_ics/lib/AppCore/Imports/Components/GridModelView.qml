import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/gridView.png
GridView {
    id: root


    //MODEL
    property alias viewModel: root.model
    //DELEGATE
    property alias viewDelegate: root.delegate
    property string displayRole: "textRole"
    property bool topToBottom: true
    //INT 80
    property alias itemWidth: root.cellWidth
    //INT 80
    property alias itemHeight: root.cellHeight
    //BOOL true
    property alias flickable: root.interactive
    property bool stopAtBounds: false
    property bool sizeToContents: false

    width: 128
    height: 256
    clip: true

    flow: topToBottom ? GridView.FlowTopToBottom :GridView.FlowLeftToRight
    boundsBehavior: stopAtBounds ? GridView.StopAtBounds : GridView.OvershootBounds

    Binding {
        when: root.sizeToContents
        target: root
        property: root.topToBottom ? "width" : "height"
        value: {
            var value = 0;
            if (root.topToBottom)
                value = Math.round(root.count / Math.floor(root.height/root.itemHeight)) * root.itemWidth
            else
                value = Math.round(root.count / Math.floor(root.width/root.itemWidth)) * root.itemHeight
            return value;
        }
    }
}
