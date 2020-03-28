/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.14
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.5


ColumnLayout
{
    //    property alias openGLButton: openGLButton
    //    property alias antialiasButton: antialiasButton
    spacing: 8
    Layout.fillHeight: true
    signal animationsEnabled(bool enabled)
    signal seriesTypeChanged(string type)
    signal signalSourceChanged(string source, int signalCount, int sampleCount);
    height: 600


    Text
    {
        text: "Ventilator"
        font.bold: true
        font.pointSize: 24
        color: "yellow"
        bottomPadding: 20
        topPadding: 10
    }




    SwipeView {
        id: swipeView
        width: 200
        height: 200
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: 0
        spacing: 50
        ButtonGroup {
            buttons: modesButtonColumn.children
        }

        Item {
            id: modePage
            Text {
                id: modesView
                color: "#ffffff"
                text: qsTr("Modes View")
                anchors.bottomMargin: 424
                visible: true
                font.bold: true
                anchors.fill: parent
                font.pixelSize: 20
            }

            Column {
                id: modesButtonColumn
                anchors.leftMargin: 0
                anchors.top: modesView.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.topMargin: 0
                spacing: 50



                DelayButton
                {
                    id: acmodeButton
                    height: 40
                    text: "A/C Mode"
                    autoExclusive: true
                    spacing: 50

                }

                DelayButton
                {
                    id: psButton
                    height: 40
                    text: "Pressure Support"
                    autoExclusive: true
                    spacing: 50

                    onActivated:
                    {

//                       checkable = false;

                    }
                    onPressed:
                    {
                        if(checked)
                        {
                            checked = true;

                        }
                    }
                }

                DelayButton
                {
                    id: offButton
                    height: 40
                    text: "Off"
                    autoExclusive: true
                    spacing: 50
                    delay: 2000
                }

            }


        }

        Item
        {
            id: settingsPage
            Text {
                id: settingsView
                color: "#ffffff"
                text: qsTr("Settings View")
                visible: true
                font.bold: true
                anchors.fill: parent
                font.pixelSize: 20
            }
        }
    }

    PageIndicator {
        id: indicator
        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom


        count: swipeView.count
        currentIndex: swipeView.currentIndex
        antialiasing: true
        delegate:
            Rectangle {
            implicitWidth: 15
            implicitHeight: 15
            color: "white"
            radius: width / 2
            opacity: index === swipeView.currentIndex ? 0.95 : pressed ? 0.7 : 0.45

            Behavior on opacity {
                OpacityAnimator {
                    duration: 100
                }
            }
        }

    }


}

/*##^##
Designer {
    D{i:6;anchors_height:400;anchors_width:200}
}
##^##*/
