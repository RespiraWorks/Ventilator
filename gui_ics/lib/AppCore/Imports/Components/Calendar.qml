import QtQuick 2.0

Item {
    id: root
    //DELEGATE
    property Component delegate: defaultDayDelegateComponent
    property int verticalSpacing: 10
    property int horizontalSpacing: 10

    property int year: 2020
    property int month: 2
    property int day: 23

    property bool vertical: false
    property bool startFromMonday: false

    //FONT textFont
    property font monthTitleFont: Qt.font({ family: "Roboto", pixelSize: 40 })
    //FONT textFont
    property font dayOfWeekTitleFont: Qt.font({ family: "Roboto", pixelSize: 24 })
    //COLOR textColor
    property color monthTitleColor: "#4b4b4b"
    //COLOR textColor
    property color dayOfWeekTitleColor: "#787878"

    signal dateClicked(int y, int m, int d)

    width: 560
    height: 460

    ListView {
        id: listView
        anchors.fill: parent
        model: 12
        orientation: root.vertical ? ListView.Vertical : ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightMoveDuration: 0
        currentIndex: root.month
        clip: true
        delegate: Item {
            id: monthDeletate
            width: listView.width
            height: listView.height
            Text {
                id: monthTitle
                anchors {
                    right: daysNamesRow.right
                }
                text: {
                    const monthIndex = new Date(root.year, index).getMonth()
                    return d.monthNames[monthIndex].toUpperCase()
                }
                font: root.monthTitleFont
                color: root.monthTitleColor
            }
            Row {
                id: daysNamesRow
                anchors {
                    top: monthTitle.bottom
                    left: daysItem.left
                }
                spacing: root.horizontalSpacing
                Repeater {
                    height: 50
                    model: d.daysNames.length
                    Item {
                        width: d.dayDelegateWidth
                        height: 50
                        Text {
                            id: dayNameText
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                                bottom: parent.bottom
                            }
                            text: {
                                if(!root.startFromMonday)
                                    return d.daysNames[index].substring(0, 3).toUpperCase()
                                else {
                                    var array = d.daysNames.slice()
                                    var sunday = array.shift()
                                    array.push(sunday)
                                    return array[index].substring(0, 3).toUpperCase()
                                }
                            }
                            font: root.dayOfWeekTitleFont
                            color: root.dayOfWeekTitleColor
                        }
                    }
                }
            }
            Item {
                id: daysItem
                anchors {
                    top: daysNamesRow.bottom
                    topMargin: 10
                    horizontalCenter: parent.horizontalCenter
                }
                width: (d.dayDelegateWidth + root.horizontalSpacing) * 7 - root.horizontalSpacing
                height: (d.dayDelegateHeight + root.verticalSpacing) * 6 - root.verticalSpacing
                property int dayOffset: {
                    var sundayShift = root.startFromMonday ? -1 : 0
                    if(sundayShift < 0)
                        sundayShift = 6
                    new Date(root.year, index).getDay() + sundayShift
                }
                property bool currentMonth: root.month == index
                property int monthIndex: index
                Repeater {
                    id: repeater
                    model: new Date(root.year, index + 1, 0).getDate()
                    Loader {
                        id: loader
                        sourceComponent: root.delegate
                        onLoaded: {
                            item.x = Qt.binding(function() {
                                return ((index + (daysItem.dayOffset % 7)) % 7) * (width + horizontalSpacing)
                            })
                            item.y = Qt.binding(function() {
                                return Math.floor((index + (daysItem.dayOffset % 7)) / 7) * (height + verticalSpacing)
                            })
                            item.fi_Label_text = index + 1
                            item.triggered = Qt.binding(function() {
                                return (index == root.day) && daysItem.currentMonth
                            })
                            d.dayDelegateWidth = width
                            d.dayDelegateHeight = height
                        }
                        Connections {
                            target: loader.status == Loader.Ready ? loader.item : null
                            onClicked: { root.dateClicked(root.year, daysItem.montIndex, index) }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: defaultDayDelegateComponent
        CalendarDelegate {}
    }

    QtObject {
        id: d
        property int dayDelegateWidth: 10
        property int dayDelegateHeight: 10

        readonly property var daysNames: ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
        readonly property var monthNames: [
            "January", "February", "March", "April",
            "May", "June", "July", "August",
            "September", "October", "November", "December"
        ]
    }
}
