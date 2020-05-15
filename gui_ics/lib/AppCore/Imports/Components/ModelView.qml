import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/modelView.png
Item {
    id: root

    //MODEL
    property alias viewModel: lv.model
    //DELEGATE
    property alias viewDelegate: lv.delegate
    //DELEGATE
    property Component sectionDelegate: null
    //STRING
    property string sectionProperty: ""
    //INT 0
    property alias delegateSpacing: lv.spacing
    property int currentDelegate: -1
    property bool vertical: true
    //BOOL true
    property alias flickable: lv.interactive
    property bool stopAtBounds: false
    //INT
    readonly property alias contentWidth: lv.contentWidth
    //INT
    readonly property alias contentHeight: lv.contentHeight
    //INT
    readonly property alias count: lv.count
    property bool sizeToContents: false
    //DOUBLE
    property real prefferedHighlightBegin: 0
    //DOUBLE
    property real prefferedHighlightEnd: 0
    //BOOL
    property bool trackCurrentDelegate: true

    width: 128
    height: 256
    clip: true

    onCurrentDelegateChanged: {
        lv.currentIndex = currentDelegate
        lv.moveToCurrentDelegate()
    }

    property alias contentX: lv.contentX
    property alias contentY: lv.contentY
    ListView {
        id: lv

        anchors.fill: parent

        orientation: root.vertical ? ListView.Vertical : ListView.Horizontal
        boundsBehavior: root.stopAtBounds ? ListView.StopAtBounds : ListView.OvershootBounds

        section.delegate: root.sectionDelegate
        section.property: root.sectionProperty

        preferredHighlightBegin: root.prefferedHighlightBegin
        preferredHighlightEnd: root.prefferedHighlightEnd
        highlightRangeMode: preferredHighlightEnd > preferredHighlightBegin ? ListView.StrictlyEnforceRange : ListView.NoHighlightRange

        function moveToCurrentDelegate() {
            if(root.trackCurrentDelegate)
                lv.positionViewAtIndex(Math.max(0, currentIndex), ListView.Contain)
        }
        onContentWidthChanged: moveToCurrentDelegate()
        onContentHeightChanged: moveToCurrentDelegate()
    }

    Binding {
        when: root.sizeToContents
        target: root
        property: root.vertical ? "height" : "width"
        value: {
            var value = 0;
            if (root.vertical)
                value = lv.contentHeight
            else
                value = lv.contentWidth
            return value;
        }
    }
}
