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
#include "hal.h"

#if defined(BARE_STM32)

// This file implements the interface to the Proportional Solenoid
// valve (commonly abbreviated as PSOL) used to control the flow of
// O2 in our system.
//
// The function of the PSOL is similar to that of the pinch valve,
// but the PSOL handles the higher pressure of the O2 better then
// a pinch valve would, which is why it's used for that purpose.
//
// The PSOL we're using is made by SMC pneumatics and is model number
// PVQ31-6G-40-01.  You can find it described in the series catalog
// available here: https://www.smcpneumatics.com/pdfs/PVQ.pdf
//
// On our board the solenoid output is connected to pin PA11 which
// can be driven by timer 1 channel 4.  We'll use that timer channel
// to control the solenoid

#include <algorithm>

#include "hal_stm32.h"
#include "vars.h"

// Testing in Edwin's garage, we found that the psol was fully closed at
// somewhere between 0.75 and 0.80 (i.e. definitely zero at 0.75 and probably
// zero a bit above that) and fully open at 0.90.

// UFlow IBV19M value
static Debug::Variable::Float dbg_psol_pwm_closed("psol_pwm_closed",
                                                  Debug::Variable::Access::ReadWrite, 0.35f,
                                                  "ratio", "PWM power that closes the psol [0,1]");

// UFlow IBV19M value
static Debug::Variable::Float dbg_psol_pwm_open("psol_pwm_open", Debug::Variable::Access::ReadWrite,
                                                0.75f, "ratio",
                                                "PWM power that opens the psol [0,1]");

void HalApi::InitPSOL() {
  // I'm using a 20kHz PWM frequency to drive the solenoid
  // This is somewhat arbitrary, but is high enough to ensure
  // that there won't be any audible noise from the switching
  static constexpr int PwmFreq = 5000;

  EnableClock(Timer1Base);

  // Connect PA11 to timer 1
  // [DS] table 17 shows which functions can be connected to each pin.
  // For PA11 we select function 1 to connect it to timer 1.
  GpioPinAltFunc(GpioABase, 11, 1);

  TimerReg *tmr = Timer1Base;

  // Set the frequency
  tmr->auto_reload = (CPU_FREQ / PwmFreq) - 1;

  // Configure channel 4 in PWM output mode 1
  // with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow
  // register and copied to the active register
  // at the start of the next cycle.
  //
  // TODO - the capture_compare_mode and capture_compare_enable registers of
  // the timer should really be converted to bit flags for better readability
  tmr->capture_compare_mode[1] = 0x6800;

  tmr->capture_compare_enable = 0x1000;

  // For timer 1 we need to disable the main output enable
  // (MOE) feature by setting bit 15 of the deadtime register. [RM] 26.3.16
  tmr->dead_time = 0x8000;

  // Start with 0% duty cycle
  tmr->capture_compare[3] = 0;

  // Load the shadow registers
  tmr->event = 1;

  // Start the counter
  tmr->control_reg1.bitfield.auto_reload_preload = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
}

// Set the PSOL output level to a value from 0 (fully closed)
// to 1 (fully open)
void HalApi::PSolValue(float val) {
  TimerReg *tmr = Timer1Base;

  val = std::clamp(val, 0.0f, 1.0f);

  // Scale linearly between our fully-open and fully-closed PWM values, except
  // to be a little paranoid, we say "0 means 0".
  float scaled;
  if (val == 0) {
    scaled = 0;
  } else {
    float open_pwm = dbg_psol_pwm_open.Get();
    float closed_pwm = dbg_psol_pwm_closed.Get();
    scaled = closed_pwm + val * (open_pwm - closed_pwm);
  }
  float duty = scaled * static_cast<float>(tmr->auto_reload);
  tmr->capture_compare[3] = static_cast<int>(duty);
}

#else
void HalApi::PSolValue(float val) {}
#endif
