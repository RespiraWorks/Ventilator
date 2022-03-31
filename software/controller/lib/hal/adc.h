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

#include "units.h"

#if !defined(BARE_STM32)
#include <map>
#endif

// Location of analog sensors as labeled on the PCB(s). Note that this does not necessarily define
// their function and further mapping should be done in the higher layers of the software.
enum class AnalogPin {
  InterimBoardAnalogPressure,
  U3PatientPressure,
  U4InhaleFlow,
  U5ExhaleFlow,
  InterimBoardOxygenSensor,
};

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
  Voltage read(AnalogPin pin) const;

 private:
  // Total number of A/D inputs we're sampling
  // \TODO: consider templating this or somehow linking it to mapping enum size
  static constexpr int AdcChannels{5};

  // We need the sample history to be small for two reasons:
  // - We sum to a 32-bit float and will lose precision if we add in too many samples
  // - We want the A/D reading to be fast, so summing up a really large array might be too slow
  // If you get hit with this assertion you may need to rethink the way this function works.
  // \TODO: template this, or improve it somehow
  static constexpr uint32_t AdcSampleHistoryHardMax{100};

  uint32_t adc_sample_history_{1};
  float adc_scaler_{1.0f};

  //\TODO: possibly have parent allocate memory depending on number of samples
  // Presized under some assumptions, see implementation for initialize()
  volatile uint16_t oversample_buffer_[AdcSampleHistoryHardMax * AdcChannels];

#if !defined(BARE_STM32)
 public:
  void TESTSetAnalogPin(AnalogPin pin, Voltage value);

 private:
  std::map<AnalogPin, Voltage> analog_pin_values_;
#endif
};
