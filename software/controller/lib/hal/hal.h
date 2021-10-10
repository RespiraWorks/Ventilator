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

#include "adc.h"
#include "led_indicators.h"
#include "pwm.h"
#include "units.h"

/// \TODO: Mock port interface should be with the tests, not main code
#if !defined(BARE_STM32)
#include <deque>
#include <vector>

class TestSerialPort {
 public:
  [[nodiscard]] uint16_t Write(const char *buf, uint16_t len);
  [[nodiscard]] uint16_t Read(char *buf, uint16_t len);
  uint16_t BytesAvailableForWrite();
  uint16_t BytesAvailableForRead();
  void PutIncomingData(const char *data, uint16_t len);
  uint16_t GetOutgoingData(char *data, uint16_t len);

 private:
  std::deque<std::vector<char>> incoming_data_;
  std::vector<char> outgoing_data_;
};
#endif

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
  ADC adc;
  LEDIndicators LEDs;
  // \TODO: previous implementation of buzzer used a 0.8 scaling factor to produce max volume
  // when we used set(1). We need to bring that back somehow (e.g make buzzer a pwm driven actuator
  // with calibration from 0 to 0.8, which works, but buzzer isn't really an actuator so making it
  // a member of Actuators feels wrong, and the initialization overhead is a bit much to live in
  // main in my view).
  PWM buzzer{PwmPin::Buzzer, 2400};

  static uint32_t GetCpuFreq();

  void Init();

  // Receives bytes from the GUI controller along the serial bus.
  //
  // Arduino's SerialIO will block if len > SerialBytesAvailableForRead(), but
  // this function will never block. Instead it returns the number of bytes
  // read.  It's up to you to check how many bytes were actually read and
  // handle "short reads" where we read fewer bytes than were requested.
  //
  // TODO(jlebar): Change the serial* functions to use uint32_t once we've
  // dropped support for Arduino.
  [[nodiscard]] uint16_t SerialRead(char *buf, uint16_t len);

  // Number of bytes we can read without blocking.
  uint16_t SerialBytesAvailableForRead();

  // Sends bytes to the GUI controller along the serial bus.
  //
  // Arduino's SerialIO will block if len > SerialBytesAvailableForWrite(),
  // but this function will never block.  Instead, it returns the number of
  // bytes written.  number of bytes written.  It's up to you to check how
  // many bytes were actually written and handle "short writes" where we wrote
  // less than the whole buffer.
  [[nodiscard]] uint16_t SerialWrite(const char *buf, uint16_t len);
  [[nodiscard]] uint16_t SerialWrite(uint8_t data) {
    return SerialWrite(reinterpret_cast<const char *>(&data), 1);
  }

  // Number of bytes we can write without blocking.
  uint16_t SerialBytesAvailableForWrite();

  // Serial port used for debugging
  [[nodiscard]] uint16_t DebugWrite(const char *buf, uint16_t len);
  [[nodiscard]] uint16_t DebugRead(char *buf, uint16_t len);
  uint16_t DebugBytesAvailableForWrite();
  uint16_t DebugBytesAvailableForRead();

  // Perform some early chip initialization before static constructors are run
  void EarlyInit();

  // Performs the device soft-reset
  [[noreturn]] void ResetDevice();

  // Start the loop timer
  void StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg);

  void init_PCB_ID_pins();

  void InitUARTs();

#if !defined(BARE_STM32)
 public:
  // Reads up to `len` bytes of data "sent" via SerialWrite.  Returns the
  // total number of bytes read.
  //
  // TODO: Once we have explicit message framing, this should simply read one
  // message.
  uint16_t TESTSerialGetOutgoingData(char *data, uint16_t len);

  // Simulates receiving serial data from the GUI controller.  Makes these
  // bytes available to be read by serialReadByte().
  //
  // Large buffers sent this way will be split into smaller buffers, to
  // simulate the fact that the Arduino has a small rx buffer.
  //
  // Furthermore, SerialRead() will not read across a
  // TESTSerialPutIncomingData() boundary.  This allows you to test short
  // reads.  For example, if you did
  //
  //   char buf1[8];
  //   char buf2[4];
  //   hal.TESTSerialPutIncomingData(buf1, 8);
  //   hal.TESTSerialPutIncomingData(buf2, 4);
  //
  // then the you'd have the following execution
  //
  //   char buf[16];
  //   hal.SerialBytesAvailableForRead() == 8
  //   hal.SerialRead(buf, 16) == 8
  //   hal.SerialBytesAvailableForRead() == 4
  //   hal.SerialRead(buf, 16) == 4
  //   hal.SerialBytesAvailableForRead() == 0
  //
  void TESTSerialPutIncomingData(const char *data, uint16_t len);

  // Same as above, but for the debug serial port.
  uint16_t TESTDebugGetOutgoingData(char *data, uint16_t len);
  void TESTDebugPutIncomingData(const char *data, uint16_t len);

 private:
  Time time_ = microsSinceStartup(0);

  TestSerialPort serial_port_;
  TestSerialPort debug_serial_port_;
#endif
};

extern HalApi hal;
