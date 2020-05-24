/* Copyright 2020, RespiraWorks

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
//
// The A/D inputs all connect to relatively slow signals that we want
// to read precisely, so to improve the results we do a lot of
// averaging of the A/D values.
//
// The A/D itself can oversample it's inputs a number of times and
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

/******************************************************************
 * A/D inputs.
 *
 * The following pins are used as analog inputs on the rev-1 PCB
 *
 * Please refer to the PCB schematic as the ultimate source of which
 * pin is used for which function.  A less definitive, but perhaps
 * easier to read spreadsheet is availabe here:
 * https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit#gid=0
 *
 * PA0 (ADC1_IN5)  - vin (not currently used)
 * PA1 (ADC1_IN6)  - pressure
 * PA4 (ADC1_IN9)  - inhale flow
 * PB0 (ADC1_IN15) - exhale flow
 *
 * See chapter 16 of the reference manual
 *
 *****************************************************************/

// How long a period (in seconds) we want to average the A/D readings.
static constexpr float sample_history_time_sec = 0.001f;

// Total number of A/D inputs we're sampling
static constexpr int adc_channels = 3;

// This constant controls how many times we have the A/D sample each input
// and sum them before moving on to the next input.  The constant is set
// as a log base 2, so a value of 3 for example would mean sample 8 times
// (2^3 == 8).  Legal values range from 0 to 8.
static constexpr int oversample_log2 = 4;
static constexpr int oversample_count = 1 << oversample_log2;

// This calculated constant gives the maximum A/D reading based on the
// number of samples.
static constexpr int max_adc_reading =
    (oversample_log2 >= 4) ? 65536 : (1 << (12 + oversample_log2));

// A/D sample time in CPU clock cycles.  Fixed for now
// This is the time we give the analog input to charge the A/D sampling cap.
static constexpr int adc_samp_time = 92;

// Time of an A/D conversion in CPU clock cycles.  This is the sample time + 13
static constexpr int adc_conversion_time = adc_samp_time + 13;

// Calculate how long our history buffer needs to be based on the above.
static constexpr int adc_samp_history =
    static_cast<int>(sample_history_time_sec * CPU_FREQ / adc_conversion_time /
                     oversample_count / adc_channels);

// This scaler converts the sum of the A/D readings (a total of
// adc_samp_history) into a voltage.  The A/D is scaled so a value of 0
// corresponds to 0 volts, and max_adc_reading corresponds to 3.3V
static constexpr float adc_scaler = 3.3f / (max_adc_reading * adc_samp_history);

// This buffer will hold the readings from the A/D
static volatile uint16_t adc_buff[adc_samp_history * adc_channels];

// NOTE - we expect the sample history to be small for two reasons:
// - We sum to a 32-bit floating point number and will lose precision
//   if we add in too many samples
// - We want the A/D reading to be fast, so summing up a really large
//   array might be too slow.
//
// If you get hit with this assertion you may beed to rethink the
// way this function works.
static_assert(adc_samp_history < 100);

void HalApi::InitADC() {

  // Enable the clock to the A/D converter
  EnableClock(ADC_BASE);

  // Configure the 4 pins used as analog inputs
  GPIO_PinMode(GPIO_A_BASE, 0, GPIO_PinMode::ANALOG);
  GPIO_PinMode(GPIO_A_BASE, 1, GPIO_PinMode::ANALOG);
  GPIO_PinMode(GPIO_A_BASE, 4, GPIO_PinMode::ANALOG);
  GPIO_PinMode(GPIO_B_BASE, 0, GPIO_PinMode::ANALOG);

  // Perform a power-up and calibration sequence on
  // the A/D converter
  ADC_Regs *adc = ADC_BASE;

  // Exit deep power down mode and turn on the
  // internal voltage regulator.
  adc->adc[0].ctrl = 0x10000000;

  // Wait for the startup time specified in the STM32
  // datasheet for the voltage regulator to become ready.
  // The time in the datasheet is 20 microseconds, but
  // I'll wait for 30 just to be extra conservative
  BusyWaitUsec(30);

  // Calibrate the A/D for single ended channels
  adc->adc[0].ctrl |= 0x80000000;

  // Wait until the CAL bit is cleared meaning
  // calibration is finished
  while (adc->adc[0].ctrl & 0x80000000) {
  }

  // Clear all the status bits
  adc->adc[0].stat = 0x3FF;

  // Enable the A/D
  adc->adc[0].ctrl |= 0x00000001;

  // Wait for the ADRDY status bit to be set
  while (!(adc->adc[0].stat & 0x00000001)) {
  }

  adc->adc[0].cfg1.dmaen = 1;
  adc->adc[0].cfg1.dmacfg = 1;
  adc->adc[0].cfg1.cont = 1;

  // Enable ADC oversampling.
  adc->adc[0].cfg2.rovse = (oversample_log2 > 0) ? 1 : 0;

  // The over sample setting is the log base 2 of the number of samples
  // less one.
  // For example, for 16 samples we would use log2(16)-1 = 3
  adc->adc[0].cfg2.ovsr = (oversample_log2 > 0) ? oversample_log2 - 1 : 0;

  // The A/D data register can only return 16 bits, so we could get
  // truncated data if our oversampling is higher then 16.  In this
  // case I need to tell the hardware to downshift the sum
  adc->adc[0].cfg2.ovss = (oversample_log2 < 4) ? 0 : (oversample_log2 - 4);

  // Set sample time. I'm using 92.5 A/D clocks to sample.
  static_assert(adc_samp_time == 92);
  adc->adc[0].samp.smp5 = 5;
  adc->adc[0].samp.smp6 = 5;
  adc->adc[0].samp.smp9 = 5;
  adc->adc[0].samp.smp15 = 5;

  // Set the conversion sequence
  adc->adc[0].seq.len = adc_channels - 1;
  adc->adc[0].seq.sq1 = 6;
  adc->adc[0].seq.sq2 = 9;
  adc->adc[0].seq.sq3 = 15;

  // I use DMA1 channel 1 to copy my A/D readings into my buffer
  EnableClock(DMA1_BASE);
  DMA_Regs *dma = DMA1_BASE;
  int C1 = static_cast<int>(DMA_Chan::C1);

  dma->channel[C1].pAddr = &adc->adc[0].data;
  dma->channel[C1].mAddr = adc_buff;
  dma->channel[C1].count = adc_samp_history * adc_channels;

  dma->channel[C1].config.enable = 0;
  dma->channel[C1].config.tcie = 0;
  dma->channel[C1].config.htie = 0;
  dma->channel[C1].config.teie = 0;
  dma->channel[C1].config.dir =
      static_cast<REG>(DmaChannelDir::PERIPHERAL_TO_MEM);
  dma->channel[C1].config.circular = 1;
  dma->channel[C1].config.perInc = 0;
  dma->channel[C1].config.memInc = 1;
  dma->channel[C1].config.psize = 1;
  dma->channel[C1].config.msize = 1;
  dma->channel[C1].config.priority = 0;
  dma->channel[C1].config.enable = 1;

  // Start the A/D converter
  adc->adc[0].ctrl |= 4;
}

// Read the specified analog input.
Voltage HalApi::analogRead(AnalogPin pin) {
  int offset = [&] {
    switch (pin) {
    case AnalogPin::PATIENT_PRESSURE:
      return 0;
    case AnalogPin::INFLOW_PRESSURE_DIFF:
      return 1;
    case AnalogPin::OUTFLOW_PRESSURE_DIFF:
      return 2;
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  // We just run through the buffer and add up all the readings for
  // this channel.  The DMA is still writing to this buffer in the
  // background, but that shouldn't cause any problems because memory
  // accesses for 16-bit values are atomic.
  float sum = 0;
  for (int i = 0; i < adc_samp_history; i++)
    sum += adc_buff[i * adc_channels + offset];

  return volts(sum * adc_scaler);
}

#endif
