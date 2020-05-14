/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef WAVEFORMFILTER_H
#define WAVEFORMFILTER_H

#include <QVector>

#include "filteredsample.h"
#include "waveformsample.h"

namespace WaveformFilter {
FilteredSample filterWaveformMinMax(const QVector<WaveformSample> &data);
}

#endif // WAVEFORMFILTER_H
