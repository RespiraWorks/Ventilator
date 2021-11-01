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

#include "pwm.h"

/// \TODO: have mappings be maintained in / provided by a higher layer
/******************************************************************
 * PWM outputs
 *
 * The following four outputs could be driven
 * as PWM outputs:
 *
 * PA8  - Timer 1 Channel 1 - heater control
 * PA11 - Timer 1 Channel 4 - psol control
 * PB3  - Timer 2 Channel 2 - blower control
 * PB4  - Timer 3 Channel 1 - buzzer control
 *
 * These timers are documented in [RM] chapters 26 and 27.
 *****************************************************************/

#include <algorithm>
#if defined(BARE_STM32)

// helper function to get the proper timer register based on timer peripheral ID
TimerReg *register_for(const PeripheralID peripheral) {
  switch (peripheral) {
    case PeripheralID::Timer1:
      return Timer1Base;
    case PeripheralID::Timer2:
      return Timer2Base;
    case PeripheralID::Timer3:
      return Timer3Base;
    case PeripheralID::Timer6:
      return Timer6Base;
    case PeripheralID::Timer7:
      return Timer7Base;
    case PeripheralID::Timer15:
      return Timer15Base;
    case PeripheralID::Timer16:
      return Timer16Base;
    default:
      return nullptr;
  }
}

PWM::PWM(const Frequency pwm_freq, const PeripheralID peripheral, uint8_t channel,
         const Frequency cpu_frequency)
    : pwm_freq_(pwm_freq), timer_(register_for(peripheral)), channel_(channel) {
  enable_peripheral_clock(peripheral);

  // Set the frequency
  timer_->auto_reload = static_cast<uint32_t>(cpu_frequency.hertz() / pwm_freq_.hertz()) - 1;

  // Configure channel in PWM output mode 1 with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow register and copied to the active register
  // at the start of the next cycle.
  // TODO - abstract these in a Timer abstraction, which deals with settings per channel bits.
  timer_->capture_compare_mode[static_cast<size_t>((channel_ - 1) / 2)] =
      0x68 << ((channel_ - 1) % 2) * 8;

  timer_->capture_compare_enable = 0x01 << (channel_ - 1) * 4;

  // For timer 1 we need to disable the main output enable
  // (MOE) feature by setting bit 15 of the deadtime register. [RM] 26.3.16
  if (timer_ == Timer1Base) {
    timer_->dead_time = 0x8000;
  }

  // Start with 0% duty cycle
  timer_->capture_compare[channel_ - 1] = 0;

  // Load the shadow registers
  timer_->event = 1;

  // Start the counter
  timer_->control_reg1.bitfield.auto_reload_preload = 1;
  timer_->control_reg1.bitfield.counter_enable = 1;
}

// Set the PWM duty cycle.
void PWM::set(const float duty) {
  if (timer_ != nullptr && channel_ < 5) {
    auto value = static_cast<float>(timer_->auto_reload) * std::clamp(duty, 0.0f, 1.0f);
    timer_->capture_compare[channel_ - 1] = static_cast<uint32_t>(value);
  }
}

#else

PWM::PWM(const Frequency pwm_freq, const PeripheralID peripheral, uint8_t channel,
         const Frequency cpu_frequency)
    : pwm_freq_(pwm_freq), timer_(nullptr), channel_(channel){};

void PWM::set(float duty) { value_ = std::clamp(duty, 0.0f, 1.0f); }

float PWM::get() const { return value_; }

#endif
