import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import Respira 1.0
import "../controls"
import "../controls/parameters"
import "../controls/readings"
import "../controls/graphs"
import ".."

Mode {
    id: mode

    Rectangle {
        id: parameterDisplayPanel
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

            PipDisplay {}
            PeepDisplay {}
            FlowDisplay {}
            FiO2Display {}
            TvDisplay {}
            IerDisplay {}
            RrDisplay {}

        }
    }

    Rectangle {
        width: 784; height: 440
        radius: 8
        anchors {
            top: parent.top
            left: parent.left; leftMargin: 8
            right: parameterDisplayPanel.left; rightMargin: 8
            bottom: parameterButtonsPanel.top; bottomMargin: 8
        }

        color: "#05121C"

        ColumnLayout {
            id: scopeGridLayout
            anchors.fill: parent
            spacing: 0
            PressureGraph { Layout.fillHeight: true; Layout.fillWidth: true }
            FlowGraph { Layout.fillHeight: true; Layout.fillWidth: true }
            TvGraph { Layout.fillHeight: true; Layout.fillWidth: true }
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

        PipParameter {
            implicitWidth: 159
        }
        PeepParameter {
         implicitWidth: 159 }
        PSUPPParameter {
         implicitWidth: 159
        }
        FiO2Parameter {
         implicitWidth: 159
        }
        ITimeParameter {
         implicitWidth: 159
        }
        RrParameter {
         implicitWidth: 159
        }
    }
}
