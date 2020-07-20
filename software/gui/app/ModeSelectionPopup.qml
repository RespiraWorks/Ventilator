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

    width: parent.width; height: parent.height
    parent: Overlay.overlay
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

        Item {
            id: header
            anchors { top: parent.top; topMargin: 24 }
            width: parent.width
            height: 120

            Image {
                id: icon
                width: 96; height: 96
                anchors {
                    left: parent.left; leftMargin: 24
                }
                sourceSize: Qt.size(96, 96)

                fillMode: Image.PreserveAspectFit
                source: 'qrc:/images/RW_log_24.svg'
            }

            Text {
                anchors {
                    left: icon.right; leftMargin: 24
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Select Ventilation mode")
                color: Style.theme.color.textPrimary
                font: Style.theme.font.modalHeader
            }

            Rectangle {
                anchors { bottom: parent.bottom }
                width: parent.width;
                height: 1;
                color: "#435360"
            }
        }

        ButtonGroup { id: buttonGroup }

        Row {

            spacing: 8
            anchors {
                top: header.bottom; topMargin: 56
                verticalCenter: parent.verticalCenter
                left: parent.left; leftMargin: 48
            }

            Repeater {
                id: buttonsRepeater
                model: modesModel

                delegate: PopupButton {
                    id: btn

                    property int pos: index

                    width: 304; height: 240
                    objectName: model.mode
                    checked: popup.currentMode.mode === objectName
                    text: model.title
                    ButtonGroup.group: buttonGroup
                    checkable: true
                    contentItem: Item {
                        width: btn.width; height: btn.height

                        Image {
                            id: delegateIcon
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                                top: parent.top; topMargin: 28
                            }

                            width: 40; height: 40
                            sourceSize: Qt.size(40, 40)
                            source: 'qrc:/images/RW_inspsenssettings_24.svg'
                        }

                        Text {
                            id: delegateTitle
                            anchors {
                                top: delegateIcon.bottom; topMargin: 10
                                left: parent.left; leftMargin: 16
                                right: parent.right; rightMargin: 16
                            }
                            text: btn.text
                            font: Style.theme.font.modalTitle
                            color: Style.theme.color.textPrimary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            anchors {
                                top: delegateTitle.bottom; topMargin: 0
                                left: parent.left; leftMargin: 16
                                right: parent.right; rightMargin: 16
                                bottom: parent.bottom; bottomMargin: 16
                            }
                            text: model.description
                            wrapMode: Text.WordWrap
                            font: Style.theme.font.modalContent
                            color: Style.theme.color.textAlternative
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignTop
                        }
                    }
                }
            }
        }

        PopupButton {
            anchors {
                left: parent.left; leftMargin: 48
                bottom: parent.bottom; bottomMargin: 40
            }

            text: qsTr("Cancel")
            onClicked: popup.close()
        }

        PopupButton {
            anchors {
                right: parent.right; rightMargin: 48
                bottom: parent.bottom; bottomMargin: 40
            }

            text: qsTr("Confirm")
            visible: modesModel.count > 0
            onClicked: {
                popup.selectedMode(modesModel.get(buttonGroup.checkedButton.pos))
                popup.close()
            }
        }
    }
}
