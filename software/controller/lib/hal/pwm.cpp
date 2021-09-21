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

#include <algorithm>
#include <cassert>

#include "clocks.h"
#include "gpio.h"
#include "timers.h"

/******************************************************************
 * PWM outputs
 *
 * The following four outputs could be driven
 * as PWM outputs:
 *
 * PA8  - Timer 1 Channel 1 - heater control
 * PB3  - Timer 2 Channel 2 - blower control
 *
 * For now I'll just set up the blower since that's the only
 * one called out in the HAL
 *
 * These timers are documented in [RM] chapters 26 and 27.
 *****************************************************************/

#if defined(BARE_STM32)

void PWM::initialize(const uint32_t cpu_frequency_hz) {
  enable_peripheral_clock(PeripheralID::Timer2);

  // Connect PB3 to timer 2
  // [DS] Table 17 (pg 77)
  GPIO::alternate_function(GPIO::Port::B, /*pin =*/3,
                           GPIO::AlternativeFuncion::AF1);  // TIM2_CH2

  TimerReg *tmr = Timer2Base;

  // Set the frequency
  tmr->auto_reload = (cpu_frequency_hz / pwm_freq_hz_) - 1;

  // Configure channel 2 in PWM output mode 1
  // with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow
  // register and copied to the active register
  // at the start of the next cycle.
  tmr->capture_compare_mode[0] = 0x6800;

  tmr->capture_compare_enable = 0x10;

  // Start with 0% duty cycle
  tmr->capture_compare[1] = 0;

  // Load the shadow registers
  tmr->event = 1;

  // Start the counter
  tmr->control_reg1.bitfield.auto_reload_preload = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
}

// Set the PWM period.
void PWM::set(const PwmPin pin, const float duty) {
  auto [tmr, chan] = [&]() -> std::pair<TimerReg *, int> {
    switch (pin) {
      case PwmPin::Blower:
        return {Timer2Base, 1};
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  tmr->capture_compare[chan] = static_cast<uint32_t>(static_cast<float>(tmr->auto_reload) * duty);
}

#else

void PWM::initialize(const uint32_t cpu_frequency_hz) {}

void PWM::set_pin_mode(PwmPin pin, GPIO::PinMode mode) { pwm_pin_modes_[pin] = mode; }

void PWM::set(PwmPin pin, float duty) {
  if (pwm_pin_modes_[pin] != GPIO::PinMode::Output) {
    assert(false && "Can only write to an OUTPUT pin");
  }
  pwm_pin_values_[pin] = duty;
}

#endif
