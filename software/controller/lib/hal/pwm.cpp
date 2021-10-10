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

#if defined(BARE_STM32)

#include <algorithm>

#include "clocks.h"
#include "gpio.h"
#include "timers.h"

/// \TODO generalize to have pins and frequency be maintained by caller
void PWM::initialize(const uint32_t cpu_frequency_hz) {
  auto [tmr, channel, peripheral, port, pin_number,
        alternate_function] = [&]() -> std::tuple<TimerReg*, uint8_t, PeripheralID, GPIO::Port,
                                                  uint8_t, GPIO::AlternativeFunction> {
    switch (pin_) {
      case PwmPin::Blower:
        // Connect PB3 to timer 2 channel 2
        // [DS] Table 17 (pg 77)
        return {Timer2Base,    2, PeripheralID::Timer2,
                GPIO::Port::B, 3, GPIO::AlternativeFunction::AF1};
      case PwmPin::Buzzer:
        // Connect PB4 to timer 3 channel 1
        // [DS] Table 17 (pg 77)
        return {Timer3Base,    1, PeripheralID::Timer3,
                GPIO::Port::B, 4, GPIO::AlternativeFunction::AF2};
      case PwmPin::Psol:
        // Connect PA11 to timer 1 channel 4
        // [DS] Table 17 (pg 77)
        return {Timer1Base,    4,  PeripheralID::Timer1,
                GPIO::Port::A, 11, GPIO::AlternativeFunction::AF1};
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  // remember tmr reg and channel for set() member function
  tmr_ = tmr;
  channel_ = channel;

  enable_peripheral_clock(peripheral);

  GPIO::alternate_function(port, pin_number, alternate_function);

  // Set the frequency
  tmr_->auto_reload = (cpu_frequency_hz / pwm_freq_hz_) - 1;

  // Configure channel in PWM output mode 1 with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow register and copied to the active register
  // at the start of the next cycle.
  // TODO - abstract these in a Timer abstraction, which deals with settings per channel bits.
  tmr_->capture_compare_mode[static_cast<size_t>((channel_ - 1) / 2)] = 0x68
                                                                        << ((channel_ - 1) % 2) * 8;

  tmr_->capture_compare_enable = 0x01 << (channel_ - 1) * 4;

  // For timer 1 we need to disable the main output enable
  // (MOE) feature by setting bit 15 of the deadtime register. [RM] 26.3.16
  if (tmr_ == Timer1Base) {
    tmr_->dead_time = 0x8000;
  }

  // Start with 0% duty cycle
  tmr_->capture_compare[channel_ - 1] = 0;

  // Load the shadow registers
  tmr_->event = 1;

  // Start the counter
  tmr_->control_reg1.bitfield.auto_reload_preload = 1;
  tmr_->control_reg1.bitfield.counter_enable = 1;
}

// Set the PWM period.
void PWM::set(const float duty) {
  if (tmr_ != nullptr && channel_ < 5) {
    auto value = static_cast<float>(tmr_->auto_reload) * std::clamp(duty, 0.0f, 1.0f);
    tmr_->capture_compare[channel_ - 1] = static_cast<uint32_t>(value);
  }
}

#else

#include <cassert>

void PWM::initialize(const uint32_t cpu_frequency_hz) {}

void PWM::set_pin_mode(GPIO::PinMode mode) { pwm_pin_modes_[pin_] = mode; }

void PWM::set(float duty) {
  if (pwm_pin_modes_[pin_] != GPIO::PinMode::Output) {
    assert(false && "Can only write to an OUTPUT pin");
  }
  pwm_pin_values_[pin_] = duty;
}

#endif
