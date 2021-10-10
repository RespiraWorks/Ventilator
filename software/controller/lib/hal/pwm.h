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

#include <cstdint>

#include "timers.h"

#if !defined(BARE_STM32)
#include <map>

#include "gpio.h"
#endif

// Pulse-width modulated outputs from the controller.  These can be set to
// values in [0-255].
//
// Pins default to Input, so if you add a new pin here, be sure to update
// HalApi::Init() and set it to Output!
enum class PwmPin {
  Blower,  // Controls the fan speed.
  Buzzer,  // Controls buzzer volume.
  Psol,    // Controls proportionnal solenoid valve (for oxygen limb).
};

class PWM {
 public:
  PWM(const PwmPin pin, const uint32_t pwm_freq_hz) : pin_(pin), pwm_freq_hz_(pwm_freq_hz){};

  void initialize(uint32_t cpu_frequency_hz);

  // Causes `pin` to output a square wave with the given duty cycle (range [0, 1], with preemptive
  // clamping before setting the registers).
  void set(float duty);

 private:
  PwmPin pin_;

  // The selection of PWM frequency is a trade-off between latency and
  // resolution.  Higher frequencies give lower latency and lower resolution.
  //
  // Latency is the time between setting the value and it taking effect,
  // this is essentially the PWM period (1/frequency).  For example, a
  // 20kHz frequency would give a latency of up to 50 microseconds.
  //
  // Resolution is based on the ratio of the clock frequency (80MHz) to the
  // PWM frequency.  For example, a 20kHz PWM would have a resolution of one
  // part in 4000 (80000000/20000) or about 12 bits.
  uint32_t pwm_freq_hz_;

  TimerReg* tmr_{nullptr};
  uint8_t channel_{0};

#if !defined(BARE_STM32)
 public:
  /// \TODO: is this the best thing to test? Isn't this implementation-specific?
  void set_pin_mode(GPIO::PinMode mode);

 private:
  std::map<PwmPin, GPIO::PinMode> pwm_pin_modes_;
  std::map<PwmPin, float> pwm_pin_values_;
#endif
};
