/* Copyright 2020-2021, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

////////////////////////////////////////////////////////////////////
//
// This file contains code to read the A/D inputs used in the system.
// The A/D converters are described in [RM] section 16.
//
// The A/D inputs all connect to relatively slow signals that we want
// to read precisely, so to improve the results we do a lot of
// averaging of the A/D values.
//
// The A/D itself can oversample its inputs a number of times and
// sum the result.  Those results are stored in a 16-bit register,
// so if the number of samples exceeds 16 then the sum can overflow.
// In that case there's a setting to down-shift the result we read
// from the A/D so that the max value will still fit in the 16-bit
// data register.
//
// There are some limitations to this built in oversampling though.
// For one it takes time, so we wouldn't want to set the hardware up
// to sample an A/D input say 256 times and wait around until it
// finishes.  It would be better if that happened in the background.
// Also, each channel is sampled in turn N times (for our setting N)
// before the A/D moves on to the next.  This means that the data from
// some channels will be more delayed then that from other channels
// when we read them, assuming the sampling is happening in the
// background.
//
// The way we get around this here and also increase the length of
// time that we can oversample is to use DMA to continuously store
// all the A/D readings to a circular buffer.  Each reading that is
// stored there will be the sum of N readings of that channel.  When
// we read the data we will sum up all the readings for a particular
// channel stored to the buffer and scale it appropriately.
// This allows us to efficiently average the A/D inputs for relatively
// long periods.
//
////////////////////////////////////////////////////////////////////

#if defined(BARE_STM32)

#include "hal.h"
#include "hal_stm32.h"

/*
Please refer to [PCB] as the ultimate source of which pin is used for which function.

The following pins are used as analog inputs on the rev-1 PCB:
- PC0 (ADC1_IN1)  interim board: analog pressure
- PA1 (ADC1_IN6)  U3 patient pressure
- PA4 (ADC1_IN9)  U4 inhale flow
- PB0 (ADC1_IN15) U5 exhale flow
- PC3 (ADC1_IN2)  interim board: oxygen sensor

Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
*/

// How long a period (in seconds) we want to average the A/D readings.
static constexpr float SampleHistoryTimeSec = 0.001f;

// Total number of A/D inputs we're sampling
static constexpr int AdcChannels = 5;

// Resolution of the ADC channels (in bits).
// We are using the default value (which is also the highest possible one - see [RM] 16.4.22).
static constexpr int AdcResolution = 12;

// This constant controls how many times we have the A/D sample each input
// and sum them before moving on to the next input.  The constant is set
// as a log base 2, so a value of 3 for example would mean sample 8 times
// (2^3 == 8).  Legal values range from 0 to 8.
static constexpr int OversampleLog2 = 4;
static constexpr int OversampleCount = 1 << OversampleLog2;

// [RM] 16.4.30: Oversampler (pg 425)
// This calculated constant gives the maximum A/D reading based on the number of samples.
// It is the maximum value reached when summing all samples in a 16 bits (internal) register,
// considering all samples have maximum value (2 ^ resolution).
// In other words, it is 2 ^ resolution * Number of samples, maxed out at 2^16
static constexpr int MaxAdcReading =
    (OversampleLog2 >= (16 - AdcResolution)) ? 65536 : (1 << (AdcResolution + OversampleLog2));

// Set sample time ([RM] 16.4.12).
enum class ADCSampleTimeReg {
  S2p5 = 0,    // 2.5 clock cycles
  S6p5 = 1,    // 6.5 clock cycles
  S12p5 = 2,   // 12.5 clock cycles
  S24p5 = 3,   // 24.5 clock cycles
  S47p5 = 4,   // 47.5 clock cycles
  S92p5 = 5,   // 92.5 clock cycles
  S247p5 = 6,  // 247.5 clock cycles
  S640p5 = 7   // 640.5 clock cycles
};
// I'm using 92.5 A/D clocks to sample.
// A/D sample time in CPU clock cycles.  Fixed for now.
// This is the time we give the analog input to charge the A/D sampling cap.
static constexpr ADCSampleTimeReg AdcSampleTime{ADCSampleTimeReg::S92p5};

// Time of an A/D conversion in CPU clock cycles (see [RM] 16.4.16):
// sample time (in clock cycles, rounded up) + 12 (one per bit of resolution).
static constexpr int AdcConversionTime = [] {
  switch (AdcSampleTime) {
    case ADCSampleTimeReg::S2p5:
      return 3 + AdcResolution;
    case ADCSampleTimeReg::S6p5:
      return 7 + AdcResolution;
    case ADCSampleTimeReg::S12p5:
      return 13 + AdcResolution;
    case ADCSampleTimeReg::S24p5:
      return 25 + AdcResolution;
    case ADCSampleTimeReg::S47p5:
      return 48 + AdcResolution;
    case ADCSampleTimeReg::S92p5:
      return 93 + AdcResolution;
    case ADCSampleTimeReg::S247p5:
      return 248 + AdcResolution;
    case ADCSampleTimeReg::S640p5:
      return 640 + AdcResolution;
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}();

// Calculate how long our history buffer needs to be based on the above.
static constexpr int AdcSampleHistory = static_cast<int>(
    SampleHistoryTimeSec * CPU_FREQ / AdcConversionTime / OversampleCount / AdcChannels);

// This scaler converts the sum of the A/D readings (a total of
// AdcSampleHistory) into a voltage.  The A/D is scaled so a value of 0
// corresponds to 0 volts, and MaxAdcReading corresponds to 3.3V
static constexpr float AdcScaler = 3.3f / (MaxAdcReading * AdcSampleHistory);

// This buffer will hold the readings from the A/D
static volatile uint16_t adc_buff[AdcSampleHistory * AdcChannels];

// NOTE - we need the sample history to be small for two reasons:
// - We sum to a 32-bit floating point number and will lose precision if we add in too many samples
// - We want the A/D reading to be fast, so summing up a really large array might be too slow.
//
// If you get hit with this assertion you may need to rethink the way this function works.
static_assert(AdcSampleHistory < 100);

void HalApi::InitADC() {
  // Enable the clock to the A/D converter
  EnableClock(AdcBase);

  // Configure the 5 pins used as analog inputs
  GpioPinMode(GpioCBase, 0, GPIOPinMode::Analog);  // PC0 (ADC1_IN1)  interim board: analog pressure
  GpioPinMode(GpioABase, 1, GPIOPinMode::Analog);  // PA1 (ADC1_IN6)  U3 patient pressure
  GpioPinMode(GpioABase, 4, GPIOPinMode::Analog);  // PA4 (ADC1_IN9)  U4 inhale flow
  GpioPinMode(GpioBBase, 0, GPIOPinMode::Analog);  // PB0 (ADC1_IN15) U5 exhale flow
  GpioPinMode(GpioCBase, 1, GPIOPinMode::Analog);  // PC3 (ADC1_IN2)  interim board: oxygen sensor

  // Perform a power-up and calibration sequence on the A/D converter
  AdcReg *adc = AdcBase;

  // Exit deep power down mode and turn on the internal voltage regulator.
  adc->adc[0].control = 0x10000000;

  // Wait for the startup time ([RM] 16.4.6) specified in the STM32 [DS] for the voltage regulator
  // to become ready.  The time in the [DS] is 20 microseconds ([DS] 6.3.18) but I'll wait for 30
  // just to be extra conservative
  hal.Delay(microseconds(30));

  // Calibrate the A/D for single ended channels ([RM] 16.4.8)
  adc->adc[0].control |= 0x80000000;

  // Wait until the CAL bit is cleared meaning calibration is finished
  while (adc->adc[0].control & 0x80000000) {
  }

  // Enable the A/D ([RM] 16.4.9)
  // Clear all the status bits
  adc->adc[0].status = 0x3FF;

  // Enable the A/D
  adc->adc[0].control |= 0x00000001;

  // Wait for the ADRDY status bit to be set
  while (!(adc->adc[0].status & 0x00000001)) {
  }

  adc->adc[0].configuration1.dma_enable = 1;
  adc->adc[0].configuration1.dma_config = 1;
  adc->adc[0].configuration1.continuous_conversion = 1;
  // note: since we are using default resolution (12 bits), we don't actually need this
  // values are from [RM] p461 (values for register RES[1:0])
  adc->adc[0].configuration1.resolution = [&] {
    switch (AdcResolution) {
      case 6:
        return 3;
      case 8:
        return 2;
      case 10:
        return 1;
      case 12:
      default:
        return 0;
    }
  }();

  adc->adc[0].configuration2.regular_oversampling = (OversampleLog2 > 0) ? 1 : 0;

  adc->adc[0].configuration2.oversampling_ratio = (OversampleLog2 > 0) ? OversampleLog2 - 1 : 0;

  // Set oversampling shift if necessary (see [RM] Table 66)
  adc->adc[0].configuration2.oversampling_shift = (OversampleLog2 < 4) ? 0 : (OversampleLog2 - 4);

  // Set sample times ([RM] 16.4.12).
  adc->adc[0].sample_times.ch1 = static_cast<uint32_t>(AdcSampleTime);
  adc->adc[0].sample_times.ch6 = static_cast<uint32_t>(AdcSampleTime);
  adc->adc[0].sample_times.ch9 = static_cast<uint32_t>(AdcSampleTime);
  adc->adc[0].sample_times.ch15 = static_cast<uint32_t>(AdcSampleTime);
  adc->adc[0].sample_times.ch2 = static_cast<uint32_t>(AdcSampleTime);

  // Set conversion sequence length (number of used channels - 1, per [RM] p468)
  adc->adc[0].sequence.length = AdcChannels - 1;

  adc->adc[0].sequence.sequence1 = 1;   // PC0 (ADC1_IN1)  interim board: analog pressure
  adc->adc[0].sequence.sequence2 = 6;   // PA1 (ADC1_IN6)  U3 patient pressure
  adc->adc[0].sequence.sequence3 = 9;   // PA4 (ADC1_IN9)  U4 inhale flow
  adc->adc[0].sequence.sequence4 = 15;  // PB0 (ADC1_IN15) U5 exhale flow
  adc->adc[0].sequence.sequence5 = 2;   // PC3 (ADC1_IN2)  interim board: oxygen sensor

  // I use DMA1 channel 1 to copy A/D readings into the buffer ([RM] 11.4.4)
  EnableClock(Dma1Base);
  DmaReg *dma = Dma1Base;
  int c1 = static_cast<int>(DmaChannel::Chan1);

  dma->channel[c1].peripheral_address = &adc->adc[0].data;
  dma->channel[c1].memory_address = adc_buff;
  dma->channel[c1].count = AdcSampleHistory * AdcChannels;

  dma->channel[c1].config.enable = 0;
  dma->channel[c1].config.tx_complete_interrupt = 0;
  dma->channel[c1].config.half_tx_interrupt = 0;
  dma->channel[c1].config.tx_error_interrupt = 0;
  dma->channel[c1].config.direction = static_cast<uint32_t>(DmaChannelDir::PeripheralToMemory);
  dma->channel[c1].config.circular = 1;
  dma->channel[c1].config.peripheral_increment = 0;
  dma->channel[c1].config.memory_increment = 1;
  dma->channel[c1].config.peripheral_size = 1;
  dma->channel[c1].config.memory_size = 1;
  dma->channel[c1].config.priority = 0;
  dma->channel[c1].config.enable = 1;

  // Start the A/D converter (by setting bit 2 of the control register - per [RM] p457)
  adc->adc[0].control |= 0x00000004;
}

// Read the specified analog input.
Voltage HalApi::AnalogRead(AnalogPin pin) const {
  int offset = [&] {
    switch (pin) {
      case AnalogPin::InterimBoardAnalogPressure:
        return 0;
      case AnalogPin::U3PatientPressure:
        return 1;
      case AnalogPin::U4InhaleFlow:
        return 2;
      case AnalogPin::U5ExhaleFlow:
        return 3;
      case AnalogPin::InterimBoardOxygenSensor:
        return 4;
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  // We just run through the buffer and add up all the readings for
  // this channel.  The DMA is still writing to this buffer in the
  // background, but that shouldn't cause any problems because memory
  // accesses for 16-bit values are atomic.
  float sum = 0;
  for (int i = 0; i < AdcSampleHistory; i++) sum += adc_buff[i * AdcChannels + offset];

  return volts(sum * AdcScaler);
}

#endif
