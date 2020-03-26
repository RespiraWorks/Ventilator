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

#include "datasource.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_index(-1)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    generateData(1000, 1000);
}

void DataSource::update(QAbstractSeries *series)
{
    if (series)
    {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        m_index++;
        if (m_index > m_data.count() - 1)
        {
            m_index = 0;
        }
        QVector<QPointF> points = m_data.at(m_index);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->replace(points);
    }
}

void DataSource::generateData(int rowCount, int colCount)
{
    // Remove previous data
    m_data.clear();


    // Append the new data depending on the type
    QVector<QPointF> points;
    points.reserve(colCount);

    for (int j(0); j < colCount; ++j) {
        qreal x(0);
        qreal y(0);

        y = (qSin(M_PI / 50 * j)> 0 ? 1 : 0) + QRandomGenerator::global()->generateDouble()/5;
        x = j;
        points.append(QPointF(x, y));
    }
    m_data.append(points);

    points.clear();
    // Append the new data depending on the type
    for (int i(1); i < rowCount; ++i)
    {
        for (int j(0); j < colCount - 1; ++j) {
            qreal x(0);
            qreal y(0);

            y = m_data[i-1][j+1].y();
            x = j;
            points.append(QPointF(x, y));
        }
        qreal x(0);
        qreal y(0);

        y = (qSin(M_PI / 50 * i)> 0 ? 1 : 0) + QRandomGenerator::global()->generateDouble()/5;
        x = colCount;
        points.append(QPointF(x, y));

        m_data.append(points);
        points.clear();

    }
}
