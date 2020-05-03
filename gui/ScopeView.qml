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

import QtQuick 2.11
import QtCharts 2.1

ChartView
{
    id: chartView
    animationOptions: ChartView.NoAnimation

    plotAreaColor: "#000000"
    backgroundColor: "#000000"
    property string color
    property string name
    antialiasing: true
    legend.labelColor: "white"
    margins.bottom: 0
    margins.top: 0
    margins.right: 0
    margins.left: 0
    onWidthChanged:
    {
        lineSeries.width =  Math.sqrt(Math.pow(width, 2) + Math.pow(height, 2)) * .0025
    }
    onHeightChanged:
    {
        lineSeries.width =  Math.sqrt(Math.pow(width, 2) + Math.pow(height, 2)) * .0025
    }

    ValueAxis
    {
        id: timeAxis
        titleText: "Seconds ago"
        min: -30
        max: 0
        labelsColor: chartView.color
    }

    ValueAxis
    {
        id: valueAxis
        min: -1.5
        max: 1.5
        labelsColor: chartView.color
    }

    LineSeries
    {
        id: lineSeries
        name: chartView.name
        axisX: timeAxis
        axisY: valueAxis
        color: chartView.color
        pointLabelsColor: chartView.color
        width: 1
        useOpenGL: true
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
