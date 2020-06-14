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
// PVQ31-6G-40-01.  You can find it described in the series catelog
// available here: https://www.smcpneumatics.com/pdfs/PVQ.pdf
//
// On our board the solenoid output is connected to pin PA11 which
// can be driven by timer 1 channel 4.  We'll use that timer channel
// to control the solenoid

#include "hal.h"
#include "hal_stm32.h"
#include <algorithm>

void HalApi::InitPSOL() {
  // I'm using a 20kHz PWM frequency to drive the solenoid
  // This is somewhat arbitrary, but is high enough to ensure
  // that there won't be any audible noise from the switching
  const int pwmFreq = 5000;

  EnableClock(TIMER1_BASE);

  // Connect PA11 to timer 1
  // The STM32 datasheet has a table (table 17) which shows
  // which functions can be connected to each pin.  For
  // PA11 we select function 1 to connect it to timer 1.
  GPIO_PinAltFunc(GPIO_A_BASE, 11, 1);

  TimerRegs *tmr = TIMER1_BASE;

  // Set the frequency
  tmr->reload = (CPU_FREQ / pwmFreq) - 1;

  // Configure channel 4 in PWM output mode 1
  // with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow
  // register and copied to the active register
  // at the start of the next cycle.
  //
  // TODO - the ccMode and ccEnable registers of the timer
  // should really be converted to bit flags for better
  // readability
  tmr->ccMode[1] = 0x6800;

  tmr->ccEnable = 0x1000;

  // For timer 1 we need to disable the main output enable
  // (MOE) feature by setting bit 15 of the deadtime register.
  // See section 26.3.16 of the reference manual.
  tmr->deadTime = 0x8000;

  // Start with 0% duty cycle
  tmr->compare[3] = 0;

  // Load the shadow registers
  tmr->event = 1;

  // Start the counter
  tmr->ctrl1.s.arpe = 1;
  tmr->ctrl1.s.cen = 1;
}

// Set the PSOL output level to a value from 0 (fully closed)
// to 1 (fully open)
void HalApi::PSOL_Value(float val) {
  TimerRegs *tmr = TIMER1_BASE;

  val = std::clamp(val, 0.0f, 1.0f);

  // We get maximum volume at about 50% duty cycle
  float duty = val * static_cast<float>(tmr->reload);
  tmr->compare[3] = static_cast<int>(duty);
}

#endif
