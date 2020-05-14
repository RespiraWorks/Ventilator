/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef SWEEPTIMER_H
#define SWEEPTIMER_H

#include <QTimer>

class SweepTimer : public QObject
{
    Q_OBJECT

public:
    SweepTimer(QObject *parent = nullptr);

public slots:
    void start();

signals:
    void renderSweep();

private:
    QTimer *m_sweep_timer;
};

#endif // SWEEPTIMER_H
