/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "sweeptimer.h"

#include <QThread>

#include "global_constants.h"

SweepTimer::SweepTimer(QObject *parent)
    : QObject(parent)
    , m_sweep_timer(new QTimer(this))
{
    m_sweep_timer->setTimerType(Qt::PreciseTimer);

    connect(m_sweep_timer, &QTimer::timeout, this, &SweepTimer::renderSweep);
}

void SweepTimer::start()
{
    m_sweep_timer->start(int(1000.0f / (BASE_SWEEP_SPEED / PIXEL_PITCH)));
    connect(this->thread(), &QThread::finished, m_sweep_timer, &QTimer::stop);
}
