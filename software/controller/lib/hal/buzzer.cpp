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

// The buzzer we use for generating alarms on the controller board is
// part number CT-1205H-SMT-TR made by CUI.
//
// The data sheet for this part can be found here:
// https://www.cuidevices.com/product/resource/ct-1205h-smt-tr.pdf
//
// This part is designed to be driven with a 2.4kHz square wave.
//
// On the controller board, pin PB4 is used to actuate the buzzer.
// This pin is tied to timer 3 channel 1, so we can use that timer
// to generate the square wave needed to power the buzzer.

#include "hal.h"
#include "hal_stm32.h"
#include <algorithm>

void HalApi::InitBuzzer() {
  const int buzzerFreqHz = 2400;

  EnableClock(TIMER3_BASE);

  // Connect PB4 to timer 3
  // The STM32 datasheet has a table (table 17) which shows
  // which functions can be connected to each pin.  For
  // PB4 we select function 2 to connect it to timer 3.
  GPIO_PinAltFunc(GPIO_B_BASE, 4, 2);

  TimerRegs *tmr = TIMER3_BASE;

  // Set the frequency
  tmr->reload = (CPU_FREQ / buzzerFreqHz) - 1;

  // Configure channel 1 in PWM output mode 1
  // with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow
  // register and copied to the active register
  // at the start of the next cycle.
  //
  // TODO - the ccMode and ccEnable registers of the timer
  // should really be converted to bit flags for better
  // readability
  tmr->ccMode[0] = 0x0068;

  tmr->ccEnable = 0x01;

  // Start with 0% duty cycle
  tmr->compare[0] = 0;

  // Load the shadow registers
  tmr->event = 1;

  // Start the counter
  tmr->ctrl1.s.arpe = 1;
  tmr->ctrl1.s.cen = 1;
}

void HalApi::BuzzerOff() {
  TimerRegs *tmr = TIMER3_BASE;
  tmr->compare[0] = 0;
}

// Set the buzzer on with the specified volume (0 to 1)
void HalApi::BuzzerOn(float volume) {
  TimerRegs *tmr = TIMER3_BASE;

  volume = std::clamp(volume, 0.0f, 1.0f);

  // We get maximum volume at about 80% duty cycle
  float duty = volume * 0.8f * static_cast<float>(tmr->reload);
  tmr->compare[0] = static_cast<int>(duty);
}

#endif
