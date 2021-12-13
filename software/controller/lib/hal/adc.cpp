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

// Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md

#include "adc.h"

#if defined(BARE_STM32)

#include <algorithm>

#include "dma.h"
#include "system_timer.h"

// [RM] 16.6 ADC Registers (for each ADC) (pg 450)
struct AdcStruct {
  // A/D specific registers (0x100 total length)
  struct {
    uint32_t status;
    uint32_t interrupts_enable;
    uint32_t control;
    struct {
      uint32_t dma_enable : 1;
      uint32_t dma_config : 1;
      uint32_t df_sdm_config : 1;
      uint32_t resolution : 2;
      uint32_t alignment : 1;
      uint32_t external_trigger_selection : 4;
      uint32_t external_trigger_enable : 2;
      uint32_t overrun_mode : 1;
      uint32_t continuous_conversion : 1;
      uint32_t delayed_conversion : 1;
      uint32_t reserved1 : 1;
      uint32_t discontinuous_mode : 1;
      uint32_t discontinuous_channel_count : 3;
      uint32_t discontinuous_injected_channels : 1;
      uint32_t jsqr_mode : 1;
      uint32_t single_channel_analog_watchdog1 : 1;
      uint32_t analog_watchdog1_enable : 1;
      uint32_t analog_watchdog1_on_injected : 1;
      uint32_t automatic_injected_conversion : 1;
      uint32_t analog_watchdog1_channel : 5;
      uint32_t injected_queue_disable : 1;
    } configuration1;  // 0x0C - [RM] 16.6.4 ADC Configuration Register
    // (ADC_CFGR) (pg 458)

    struct {
      uint32_t regular_oversampling : 1;
      uint32_t injected_oversampling : 1;
      uint32_t oversampling_ratio : 3;
      uint32_t oversampling_shift : 4;
      uint32_t triggered_oversampling : 1;
      uint32_t regular_oversampling_mode : 1;
      uint32_t reserved : 21;
    } configuration2;  // [RM] 16.6.5 ADC configuration register 2 (ADC_CFGR2)
    // (pg 462)

    union {
      struct {
        uint32_t ch0 : 3;
        uint32_t ch1 : 3;
        uint32_t ch2 : 3;
        uint32_t ch3 : 3;
        uint32_t ch4 : 3;
        uint32_t ch5 : 3;
        uint32_t ch6 : 3;
        uint32_t ch7 : 3;
        uint32_t ch8 : 3;
        uint32_t ch9 : 3;
        uint32_t reserved1 : 2;
        uint32_t ch10 : 3;
        uint32_t ch11 : 3;
        uint32_t ch12 : 3;
        uint32_t ch13 : 3;
        uint32_t ch14 : 3;
        uint32_t ch15 : 3;
        uint32_t ch16 : 3;
        uint32_t ch17 : 3;
        uint32_t ch18 : 3;
        uint32_t reserved2 : 5;
      } bitfield;
      uint32_t words[2];
    } sample_times;  // [RM] 16.6.6 ADC Sample Time Register 1 (ADC_SMPR1) (pg 464)

    uint32_t reserved1;
    uint32_t watchdogs[3];  // 0x20 - [RM] 16.6.8 ADC Watchdog
    uint32_t reserved2;

    // 4x sequence registers.  These registers are used
    // to define the number of A/D readings and the
    // channel numbers being read.
    union {
      struct {
        uint32_t length : 6;
        uint32_t sequence1 : 6;
        uint32_t sequence2 : 6;
        uint32_t sequence3 : 6;
        uint32_t sequence4 : 6;
        uint32_t reserved1 : 2;

        uint32_t sequence5 : 6;
        uint32_t sequence6 : 6;
        uint32_t sequence7 : 6;
        uint32_t sequence8 : 6;
        uint32_t sequence9 : 6;
        uint32_t reserved2 : 2;

        uint32_t sequence10 : 6;
        uint32_t sequence11 : 6;
        uint32_t sequence12 : 6;
        uint32_t sequence13 : 6;
        uint32_t sequence14 : 6;
        uint32_t reserved3 : 2;

        uint32_t sequence15 : 6;
        uint32_t sequence16 : 6;
        uint32_t reserved4 : 20;
      } bitfield;
      uint32_t words[4];
    } sequence;  // ADC Regular Sequence Register (pg 468)

    uint32_t data;
    uint32_t reserved3[2];
    uint32_t injected_sequence;
    uint32_t reserved4[4];
    uint32_t offset[4];
    uint32_t reserved5[4];
    uint32_t injected_data[4];
    uint32_t reserved6[4];
    uint32_t watchdoc_config[2];
    uint32_t reserved7[2];
    uint32_t differential_mode;
    uint32_t calibration;  // 0xB4 - Calibration Factors [RM] 16.6.22 (pg 476)
    uint32_t reserved8[18];
  } adc[2];  // Master ADC1, Slave ADC2

  uint32_t common_status;
  uint32_t reserved9;
  uint32_t common_control;
  uint32_t common_data;
};

typedef volatile AdcStruct AdcReg;
inline AdcReg *const AdcBase = reinterpret_cast<AdcReg *>(0X50040000);

// How long a period we want to average the A/D readings.
static constexpr Duration SampleHistoryTime{milliseconds(1)};

// Resolution of the ADC channels (in bits).
// We are using the default value (which is also the highest possible one - see [RM] 16.4.22).
static constexpr int AdcResolution{12};

// This constant controls how many times we have the A/D sample each input
// and sum them before moving on to the next input.  The constant is set
// as a log base 2, so a value of 3 for example would mean sample 8 times
// (2^3 == 8).  Legal values range from 0 to 8.
static constexpr int OversampleLog2{4};
static constexpr int OversampleCount{1 << OversampleLog2};

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

void ADC::SetAdcSampleTime(const uint8_t channel, const uint32_t sample_time) {
  if (channel > 18) return;

  AdcReg *adc = AdcBase;
  // We write the AdcSampleTimeReg value to the corresponding sample_times register:
  // in first word the first 9 channels and in the second one for channels 10 to 18,
  // and shifted by channel (modulo 10) * 3 bits
  size_t index = static_cast<size_t>(channel / 10);
  adc->adc[0].sample_times.words[index] |= sample_time << (channel % 10) * 3;
}

void ADC::SetAdcSequence(const uint8_t sequence_number, const uint8_t channel) {
  if (sequence_number < 1 || sequence_number > 16 || channel > 18) return;

  AdcReg *adc = AdcBase;
  // Set sequence registers in order, in the matching word (0 for sequence 1 to 4, 1 for 5 to 9,
  // and so on), and shifted by sequence_number (modulo 5) * 6 bits
  size_t index = static_cast<size_t>(sequence_number / 5);
  adc->adc[0].sequence.words[index] |= channel << (sequence_number % 5) * 6;
};

bool ADC::add_channel(const uint8_t channel) {
  if (n_channels_ >= MaxAdcChannels) return false;
  channels_[n_channels_++] = channel;
  return true;
};

bool ADC::initialize(const Frequency cpu_frequency) {
  adc_sample_history_ = static_cast<uint32_t>(SampleHistoryTime * cpu_frequency /
                                              AdcConversionTime / OversampleCount / MaxAdcChannels);

  // This scaler converts the sum of the A/D readings (a total of
  // adc_sample_history_) into a voltage.  The A/D is scaled so a value of 0
  // corresponds to 0 volts, and MaxAdcReading corresponds to 3.3V
  adc_scaler_ = VoltageRange.volts() / static_cast<float>(MaxAdcReading * adc_sample_history_);

  if (adc_sample_history_ > AdcSampleHistoryHardMax) return false;

  // Enable the clock to the A/D converter
  enable_peripheral_clock(PeripheralID::ADC);

  // Perform a power-up and calibration sequence on the A/D converter
  AdcReg *adc = AdcBase;

  // Exit deep power down mode and turn on the internal voltage regulator.
  adc->adc[0].control = 0x10000000;

  // Wait for the startup time ([RM] 16.4.6) specified in the STM32 [DS] for the voltage regulator
  // to become ready.  The time in the [DS] is 20 microseconds ([DS] 6.3.18) but I'll wait for 30
  // just to be extra conservative
  SystemTimer::singleton().delay(microseconds(30));

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

  // Set conversion sequence length (number of used channels - 1, per [RM] p468)
  adc->adc[0].sequence.bitfield.length = MaxAdcChannels - 1;

  // Set sample times ([RM] 16.4.12) for the channels we use and sequence order.
  for (size_t i = 0; i < n_channels_; ++i) {
    uint8_t channel = static_cast<uint8_t>(channels_[i]);
    SetAdcSampleTime(channel, static_cast<uint32_t>(AdcSampleTime));
    SetAdcSequence(static_cast<uint8_t>(i + 1), channel);
  }

  // I use DMA1 channel 1 to copy A/D readings into the buffer ([RM] 11.4.4)
  enable_peripheral_clock(PeripheralID::DMA1);
  DmaReg *dma = DMA::get_register(DMA::Base::DMA1);
  auto c1 = static_cast<uint8_t>(DMA::Channel::Chan1);

  /// \TODO: improve DMA abstraction to factor this out?
  dma->channel[c1].peripheral_address = &adc->adc[0].data;
  dma->channel[c1].memory_address = oversample_buffer_;
  dma->channel[c1].count = adc_sample_history_ * MaxAdcChannels;

  dma->channel[c1].config.enable = 0;
  dma->channel[c1].config.tx_complete_interrupt = 0;
  dma->channel[c1].config.half_tx_interrupt = 0;
  dma->channel[c1].config.tx_error_interrupt = 0;
  dma->channel[c1].config.direction = static_cast<uint32_t>(DMA::ChannelDir::PeripheralToMemory);
  dma->channel[c1].config.circular = 1;
  dma->channel[c1].config.peripheral_increment = 0;
  dma->channel[c1].config.memory_increment = 1;
  dma->channel[c1].config.peripheral_size = 1;
  dma->channel[c1].config.memory_size = 1;
  dma->channel[c1].config.priority = 0;
  dma->channel[c1].config.enable = 1;

  // Start the A/D converter (by setting bit 2 of the control register - per [RM] p457)
  adc->adc[0].control |= 0x00000004;

  return true;
}

// Read the specified analog input.
Voltage ADC::read(const uint8_t channel) const {
  size_t offset =
      std::distance(channels_.begin(), std::find(channels_.begin(), channels_.end(), channel));

  if (offset >= MaxAdcChannels) return volts(0);

  // We just run through the buffer and add up all the readings for
  // this channel.  The DMA is still writing to this buffer in the
  // background, but that shouldn't cause any problems because memory
  // accesses for 16-bit values are atomic.
  float sum = 0;
  for (int i = 0; i < adc_sample_history_; i++)
    sum += oversample_buffer_[i * MaxAdcChannels + offset];

  return volts(sum * adc_scaler_);
}

#else

bool ADC::initialize(const Frequency cpu_frequency) { return true; }

Voltage ADC::read(const uint8_t channel) const { return analog_pin_values_.at(channel); }

void ADC::TESTSetAnalogPin(const uint8_t channel, Voltage value) {
  analog_pin_values_[channel] = value;
}

#endif

namespace GPIO {
AnalogInputPin::AnalogInputPin(AdcChannel channel, ADC *adc)
    : Pin(channel.port, channel.pin, PinMode::Analog), adc_(adc), channel_(channel) {
#if defined(BARE_STM32)
  adc_->add_channel(channel_.adc_channel);
#endif
}

Voltage AnalogInputPin::read() const { return adc_->read(channel_.adc_channel); }
}  // namespace GPIO
