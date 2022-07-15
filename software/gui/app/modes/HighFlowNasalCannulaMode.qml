import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import ".."
import "../controls"
import "../controls/parameters"
import "../controls/readings"
import "../controls/graphs"
import ".."


Mode {
    Text {
        anchors.centerIn: parent
        text: "High Flow Nasal Cannula Mode"
        color: Style.theme.color.textPrimary
        font.pointSize: 38
    }

    Rectangle {

        width: 216; height: 440
        color: "#05121C"
        radius: 8
        anchors {
            top: parent.top;
            right: parent.right; rightMargin: 8
        }


        GridLayout {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            columns: 2
            columnSpacing: 8
            rowSpacing: 8


            FiO2Display {}
            FlowDisplay{}


        }
    }


    Row {
        id: parameterButtonsPanel
        spacing: 8

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom; bottomMargin: 8
        }

        Layout.alignment: Qt.AlignHCenter

        FiO2Parameter {}
        Flowlmp {}
    }
}
