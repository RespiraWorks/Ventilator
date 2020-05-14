/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef WAVEFORMSAMPLE_H
#define WAVEFORMSAMPLE_H

struct WaveformSample {
    WaveformSample() = default;
    WaveformSample(const WaveformSample &sample) = default;
    WaveformSample(int value, unsigned int status);

    int value = 0;
    unsigned int status = 0;
};

#endif // WAVEFORMSAMPLE_H
