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

#pragma once

#include <array>

#include "gpio.h"
#include "units.h"

#if !defined(BARE_STM32)
#include <map>
#endif

// Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md

/// \TODO: could be explicit singleton?
class ADC {
 public:
  ADC() = default;

  /// \returns true if buffer size was sufficient. If false, consider changing
  ///          AdcSampleHistoryHardMax below
  [[nodiscard]] bool initialize(Frequency cpu_frequency);

  // Reads from analog sensor using an analog-to-digital converter.
  // Returns a voltage.  On STM32 this can range from 0 to 3.3V.
  // In test mode, will return the last value set via TESTSetAnalogPin.
  Voltage read(uint8_t channel) const;

  // Add a new channel to the ADC
  bool add_channel(uint8_t channel);

  // Voltage range for the ADC
  static constexpr Voltage VoltageRange{volts(3.3f)};

 private:
  // Total number of A/D inputs we're sampling
  // \TODO: consider templating this or somehow linking it to mapping enum size
  static constexpr int MaxAdcChannels{5};

  // We need the sample history to be small for two reasons:
  // - We sum to a 32-bit float and will lose precision if we add in too many samples
  // - We want the A/D reading to be fast, so summing up a really large array might be too slow
  // If you get hit with this assertion you may need to rethink the way this function works.
  // \TODO: template this, or improve it somehow
  static constexpr uint32_t AdcSampleHistoryHardMax{100};

  uint32_t adc_sample_history_{1};
  float adc_scaler_{1.0f};

  // Array of ADC channels we use.
  std::array<uint8_t, MaxAdcChannels> channels_;
  uint8_t n_channels_{0};

  /// \TODO: possibly have parent allocate memory depending on number of samples
  // Presized under some assumptions, see implementation for initialize()
  volatile uint16_t oversample_buffer_[AdcSampleHistoryHardMax * MaxAdcChannels];

// \TODO: subclass as STM32 and mock classes to make this easier to read, and have the
// mock class be part of the tests rather than the main code.
#if defined(BARE_STM32)
  // helper functions to manipulate ADC registers
  void SetAdcSampleTime(uint8_t channel, uint32_t sample_time);
  void SetAdcSequence(uint8_t sequence_number, uint8_t channel);

#else
 public:
  void TESTSetAnalogPin(uint8_t channel, Voltage value);

 private:
  std::map<uint8_t, Voltage> analog_pin_values_;
#endif
};

namespace GPIO {
// Analog input pin, linked to an ADC channel.
class AnalogInputPin : public Pin {
 public:
  AnalogInputPin(AdcChannel channel, ADC *adc);
  Voltage read() const;

 private:
  ADC *adc_{nullptr};
  AdcChannel channel_;
};
}  // namespace GPIO
