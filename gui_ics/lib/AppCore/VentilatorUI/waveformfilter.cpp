/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "waveformfilter.h"

FilteredSample WaveformFilter::filterWaveformMinMax(const QVector<WaveformSample> &data)
{
    int min_val = 0;
    int max_val = 0;
    int lead_val = 0;
    int trail_val = 0;

    unsigned int status = 0;

    for (int k = 0; k < data.size(); ++k) {
        int val = data.at(k).value;

        // bitwise OR of bit flags
        status = data.at(k).status | status;

        if (k == 0) {
            min_val = val;
            max_val = val;
            lead_val = data.first().value;
            trail_val = data.last().value;
        } else {
            min_val = qMin(val, min_val);
            max_val = qMax(val, max_val);
        }
    }

    return FilteredSample(lead_val, trail_val, min_val, max_val, status);
}
