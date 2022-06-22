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

#include <algorithm>
#include <cstdint>

#include "led_indicators.h"
#include "units.h"

// Singleton class which implements a hardware abstraction layer.
//
// Access this via the `hal` global variable, e.g. `hal.millis()`.
//
// TODO: Make Hal a namespace rather than a class.  Then this header won't need
// any ifdefs for different platforms, and all of the "global variables" can
// move into the hal_foo.cpp files.
class HalApi {
 public:
  /// \TODO: likely these should not even be members
  LEDIndicators LEDs;

  static Frequency GetCpuFreq();

  void Init();

  // Perform some early chip initialization before static constructors are run
  void EarlyInit();

  // Performs the device soft-reset
  [[noreturn]] void ResetDevice();

  // Start the loop timer
  void StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg);

  void init_PCB_ID_pins();

#if !defined(BARE_STM32)
 private:
  Time time_ = microsSinceStartup(0);
#endif
};

extern HalApi hal;
