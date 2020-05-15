/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

static constexpr double DATA_RANGE = 1.e3; // 8 significant digits of data
static constexpr int DATA_RATE = 5000; // 5kHZ data
static constexpr float PIXEL_PITCH = 0.213f;
static constexpr int UPDATE_RATE = 16;
static constexpr float BASE_SWEEP_SPEED = 12.5;
static constexpr int NUMBER_WAVEFORMS = 12;
static constexpr int MAX_RAW_PLOT_DATA_SIZE = 1000;
static constexpr int MAX_RAW_PLOT_DATA_SIZE_HALF = MAX_RAW_PLOT_DATA_SIZE / 2;

#endif // GLOBAL_CONSTANTS_H
