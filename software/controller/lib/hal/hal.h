/* Copyright 2020-2022, RespiraWorks

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

// A hardware abstraction layer that supports faking for tests.
//
// The canonical list of hardware and the pins they connect to is:
// https://bit.ly/3aERr69
//
// When running in test mode, HalApi is pedantically a fake, not a mock.  All
// that means is that the methods have "fake" implementations, trying to
// loosely mimic the hardware in some defined way.  For example, instead of
// telling the actual amount of time since boot, Now() stays constant, and
// doesn't advance unless you call Delay().
//
// It would be possible to mock some methods in HAL if we needed to.  A mock is
// a method whose behavior can be controlled by the test itself; it might do
// nothing, or it might throw an exception, or whatever.  Tests can also
// observe whether mocked methods are called.  So far that hasn't been
// necessary.

#include <cstdint>

#include "function.h"
#include "units.h"
#include "vars.h"

#if defined(BARE_STM32)
/// \TODO use truly abstract UART and I2C classes instead
#include "i2c.h"
#include "uart_dma.h"
#endif

class HalApi {
 public:
  // Perform some early chip initialization before static constructors are run
  void EarlyInit();

  void Init(Frequency cpu_frequency);

  // Performs the device soft-reset
  [[noreturn]] void ResetDevice();

  // Start the loop timer
  //  void StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg);
  //  void StartLoopTimer(const Duration &period, naive_function<void(void)> callback);

  void set_timer15_callback(naive_function callback);

#if defined(BARE_STM32)
  void bind_channels(I2C::Channel *i2c, UART::DMAChannel *rpi, UART::Channel *debug);

  // local static functions.  We don't want to add any private functions to the Hal class to avoid
  // complexity with other builds. Those are Interrupt Service Routines, i.e callback functions for
  // the interrupt handlers. They are referenced in the Interrupt Vector Table.
  static void Timer6ISR();
  static void Timer15ISR();
  static void Uart2ISR();
  static void Uart3ISR();
  static void DMA1Channel2ISR();
  static void DMA1Channel3ISR();
  static void I2c1EventISR();
  static void I2c1ErrorISR();
  static void DMA2Channel6ISR();
  static void DMA2Channel7ISR();

 private:
  I2C::Channel *i2c_{nullptr};
  UART::DMAChannel *rpi_uart_{nullptr};
  UART::Channel *debug_uart_{nullptr};
  naive_function timer15_callback_;

#endif

 private:
  Frequency cpu_frequency_{hertz(0)};
};

extern HalApi hal;
