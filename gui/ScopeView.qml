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
    property bool openGL: true
    property bool openGLSupported: true
    antialiasing: true
    legend.labelColor: "white"
    margins.bottom: 0
    margins.top: 0
    margins.right: 0
    margins.left: 0
    onWidthChanged:
    {
        lineSeries.width =  Math.sqrt(Math.pow(width, 2) + Math.pow(height, 2)) * .0025
//        print("Width: " + lineSeries.width)
    }
    onHeightChanged:
    {
        lineSeries.width =  Math.sqrt(Math.pow(width, 2) + Math.pow(height, 2)) * .0025
//        print("Width: " + lineSeries.width)
    }

    onOpenGLChanged:
    {
        if (openGLSupported)
        {
            series(chartView.name).useOpenGL = openGL;
        }
    }
    Component.onCompleted:
    {
        if (!series(chartView.name).useOpenGL) {
            openGLSupported = false
            openGL = false
        }
    }

    ValueAxis
    {
        id: axisY
        min: 0
        max: 3
        labelsColor: chartView.color
    }


    ValueAxis
    {
        id: axisX
        min: 0
        max: 1000
        labelsColor: chartView.color
    }

    LineSeries
    {
        id: lineSeries
        name: chartView.name
        axisX: axisX
        axisY: axisY
        useOpenGL: chartView.openGL
        color: chartView.color
        pointLabelsColor: chartView.color
        width: 1

    }



    function createAxis(min, max)
    {
        // The following creates a ValueAxis object that can be then set as a x or y axis for a series
        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: "
                                  + min + "; max: " + max + " }", chartView);
    }

    function setAnimations(enabled)
    {
        if (enabled)
        {
            chartView.animationOptions = ChartView.SeriesAnimations;
        }
        else
        {
            chartView.animationOptions = ChartView.NoAnimation;
        }
    }

    function changeRefreshRate(rate)
    {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }
}
