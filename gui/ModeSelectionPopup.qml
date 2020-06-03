import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.4

import "controls"

import "."

/*!
    \qmltype ModelSelectionPopup
    \brief A popup used to switch ventilator's modes.
*/
Popup {
    id: popup

    width: 800; height: 470
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    modal: true
    focus: true
    padding: 0
    margins: 0

    // ListElement representing current mode in use
    property var currentMode

    // a model describing all possible modes buttons.
    // elements should have at least two fields:
    // model.mode, the id of the mode as a string.
    // model.title, display on the button as a string.
    property ListModel modesModel: ListModel{}


    // Once clicked on "Confirm" the popup will dismiss and return a ListElement of current selection
    // using this signal.
    signal selectedMode(var mode)

    background: Rectangle {
        radius: 4
        color: Style.theme.color.modalBackground
    }

    contentItem: Item {

        Rectangle {
            id: header
            width: parent.width
            height: 60
            color: Style.theme.color.modalHeaderColor
            Text {
                anchors.centerIn: parent
                text: qsTr("Mode Selection")
                color: "white"
                font {
                    bold: true
                    pixelSize: 38
                }
            }
        }

        ButtonGroup { id: buttonGroup }

        ColumnLayout {

            width: parent.width

            anchors {
                margins: 10
                top: header.bottom;
                left:parent.left
                bottom: parent.bottom
                right: parent.right
            }

            Repeater {
                id: buttonsRepeater
                model: modesModel

                delegate: PopupButton {
                    property int pos: index
                    objectName: model.mode
                    checked: popup.currentMode.mode === objectName
                    Layout.fillWidth: true
                    text: model.title
                    ButtonGroup.group: buttonGroup
                    checkable: true
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                PopupButton {
                    Layout.fillWidth: true
                    indicator: Item{}
                    text: qsTr("Cancel")
                    onClicked: popup.close()
                }

                PopupButton {
                    Layout.fillWidth: true
                    indicator: Item{}
                    text: qsTr("Confirm")
                    visible: modesModel.count > 0
                    onClicked: {
                        popup.selectedMode(modesModel.get(buttonGroup.checkedButton.pos))
                        popup.close()
                    }
                    checked: true
                }
            }
        }
    }
}
