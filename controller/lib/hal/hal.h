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
#ifndef HAL_H
#define HAL_H

// A hardware abstraction layer that supports faking for tests.
//
// The canonical list of hardware and the pins they connect to is:
// https://bit.ly/3aERr69
//
// When running in test mode, HalApi is pedantically a fake, not a mock.  All
// that means is that the methods have "fake" implementations, trying to
// loosely mimic the hardware in some defined way.  For example, instead of
// telling the actual amount of time since boot, millis() stays constant, and
// doesn't advance unless you call delay().
//
// It would be possible to mock some methods in HAL if we needed to.  A mock is
// a method whose behavior can be controlled by the test itself; it might do
// nothing, or it might throw an exception, or whatever.  Tests can also
// observe whether mocked methods are called.  So far that hasn't been
// necessary.

#include "algorithm.h"
#include "units.h"
#include <stdint.h>

#ifdef TEST_MODE

#if defined(BARE_STM32)
#error "TEST_MODE intended to be run only on native, but BARE_STM32 is defined"
#endif

#include "checksum.h"
#include <cstring>
#include <deque>
#include <map>
#include <vector>

#else // !TEST_MODE

#if !defined(BARE_STM32)
#error "When running without TEST_MODE, expecting BARE_STM32 to be defined"
#endif

#endif // TEST_MODE

// ---------------------------------------------------------------
// Strongly typed analogues of some Arduino types.
// "Strongly typed" means that it will be a compile error, e.g.,
// to pass a PWM pin id to a function expecting an analog pin id.
// ---------------------------------------------------------------

// Mode of a digital pin.
// Usage: PinMode::INPUT etc.
enum class PinMode {
  // Test code relies on INPUT being the first enumeration (to get the behavior
  // that INPUT pins are the default).
  INPUT,
  OUTPUT,
  INPUT_PULLUP
};

// Voltage level of a digital pin.
// Usage: VoltageLevel::HIGH, LOW
enum class VoltageLevel { HIGH, LOW };

enum class AnalogPin {
  // MPXV5004DP pressure sensors.
  //
  // PATIENT_PRESSURE reads an absolute pressure value at the patient.
  //
  // {INFLOW,OUTFLOW}_PRESSURE_DIFF, read a differential across a venturi.
  // They let us measure volumetric flow into and out of the patient.
  PATIENT_PRESSURE,
  INFLOW_PRESSURE_DIFF,
  OUTFLOW_PRESSURE_DIFF,
};

// Pulse-width modulated outputs from the controller.  These can be set to
// values in [0-255].
//
// Pins default to INPUT, so if you add a new pin here, be sure to update
// HalApi::init() and set it to OUTPUT!
enum class PwmPin {
  // Controls the fan speed.
  BLOWER,
};

// Binary pins set by the controller -- these are booleans, HIGH or LOW.
//
// PWM pins can of course be HIGH or LOW too, but we separate out purely on/off
// pins from PWM pins for reasons of "strong typing".
//
// Pins default to INPUT, so if you add a new pin here, be sure to update
// HalApi::init() and set it to OUTPUT!
enum class BinaryPin {
  SOLENOID,
};

// Interrupts on the STM32 are prioritized.  This allows
// more important interrupts to interrupt less important
// ones.  When interrupts are enabled we give a priority
// value to indicate how important the interrupt is.
// The lower the priority number the more important the
// interrupt.  The range is 0 to 15, but I only use a few
// here.  Hard faults, NMI, resets, etc have a fixed
// priority of -1, so they can always interrupt any other
// priority level.
enum class IntPriority {
  CRITICAL = 2, // Very important interrupt
  STANDARD = 5, // Normal hardware interrupts
  LOW = 8,      // Less important.  Hardware interrutps can interrupt this
};

// Singleton class which implements a hardware abstraction layer.
//
// Access this via the `Hal` global variable, e.g. `Hal.millis()`.
//
// TODO: Make Hal a namespace rather than a class.  Then this header won't need
// any ifdefs for different platforms, and all of the "global variables" can
// move into the hal_foo.cpp files.
class HalApi {
public:
  void init();

  // Amount of time that has passed since the board started running the
  // program.
  //
  // Faked when testing.  Time doesn't advance unless you call delay().
  Time now();

  // Sleeps for some number of milliseconds.
  //
  // Faked when testing.  Does not sleep, but does advance the time returned by
  // millis().
  void delay(Duration d);

  // Caveat for people new to Arduino: analogRead and analogWrite are completely
  // separate from each other and do not even refer to the same pins.
  // analogRead() reads the value of an analog input pin. analogWrite() writes
  // to a PWM pin - some of the digital pins are PWM pins.

  // Reads from analog sensor using an analog-to-digital converter.
  //
  // Returns a voltage.  On STM32 this can range from 0 to 3.3V.
  //
  // In test mode, will return the last value set via test_setAnalogPin.
  Voltage analogRead(AnalogPin pin);

#ifdef TEST_MODE
  void test_setAnalogPin(AnalogPin pin, Voltage value);
#endif

  // Causes `pin` to output a square wave with the given duty cycle (range
  // [0, 1]).
  //
  // Perhaps a better name would be "pwmWrite", but we also want to be somewhat
  // consistent with the Arduino API that people are familiar with.
  void analogWrite(PwmPin pin, float duty);

  // Sets `pin` to high or low.
  void digitalWrite(BinaryPin pin, VoltageLevel value);

  // Receives bytes from the GUI controller along the serial bus.
  //
  // Arduino's SerialIO will block if len > serialBytesAvailableForRead(), but
  // this function will never block. Instead it returns the number of bytes
  // read.  It's up to you to check how many bytes were actually read and
  // handle "short reads" where we read fewer bytes than were requested.
  //
  // TODO(jlebar): Change the serial* functions to use uint32_t once we've
  // dropped support for Arduino.
  [[nodiscard]] uint16_t serialRead(char *buf, uint16_t len);

  // Number of bytes we can read without blocking.
  uint16_t serialBytesAvailableForRead();

  // Sends bytes to the GUI controller along the serial bus.
  //
  // Arduino's SerialIO will block if len > serialBytesAvailableForWrite(), but
  // this function will never block.  Instead, it returns the number of bytes
  // written.  number of bytes written.  It's up to you to check how many bytes
  // were actually written and handle "short writes" where we wrote less than
  // the whole buffer.
  [[nodiscard]] uint16_t serialWrite(const char *buf, uint16_t len);
  [[nodiscard]] uint16_t serialWrite(uint8_t data) {
    return serialWrite(reinterpret_cast<const char *>(&data), 1);
  }

  // Number of bytes we can write without blocking.
  uint16_t serialBytesAvailableForWrite();

  // Serial port used for debugging
  uint16_t debugWrite(const char *buf, uint16_t len);
  uint16_t debugRead(char *buf, uint16_t len);

#ifndef TEST_MODE
  // Translates to a numeric pin that can be passed to the Arduino API.
  uint8_t rawPin(PwmPin pin);
  uint8_t rawPin(AnalogPin pin);
  uint8_t rawPin(BinaryPin pin);

  // Perform some early chip initialization before static constructors are run
  void EarlyInit();

#else
  // Reads up to `len` bytes of data "sent" via serialWrite.  Returns the total
  // number of bytes read.
  //
  // TODO: Once we have explicit message framing, this should simply read one
  // message.
  uint16_t test_serialGetOutgoingData(char *data, uint16_t len);

  // Simulates receiving serial data from the GUI controller.  Makes these
  // bytes available to be read by serialReadByte().
  //
  // Large buffers sent this way will be split into smaller buffers, to
  // simulate the fact that the Arduino has a small rx buffer.
  //
  // Furthermore, serialRead() will not read across a
  // test_serialPutIncomingData() boundary.  This allows you to test short
  // reads.  For example, if you did
  //
  //   char buf1[8];
  //   char buf2[4];
  //   Hal.test_serialPutIncomingData(buf1, 8);
  //   Hal.test_serialPutIncomingData(buf2, 4);
  //
  // then the you'd have the following execution
  //
  //   char buf[16];
  //   Hal.serialBytesAvailableForRead() == 8
  //   Hal.serialRead(buf, 16) == 8
  //   Hal.serialBytesAvailableForRead() == 4
  //   Hal.serialRead(buf, 16) == 4
  //   Hal.serialBytesAvailableForRead() == 0
  //
  void test_serialPutIncomingData(const char *data, uint16_t len);
#endif

  // Performs the device soft-reset
  [[noreturn]] void reset_device();

  // Start the loop timer
  void startLoopTimer(const Duration &period, void (*callback)(void *),
                      void *arg);

  // Pets the watchdog, this makes the watchdog not reset the
  // system for configured amount of time
  void watchdog_handler();

  // Interrupt enable/disable functions.
  //
  // Where possible, prefer using the BlockInterrupts RAII class.
  //
  // NOTE: Interrupts should only be disabled for short periods of time and
  // only for very good reasons.  Leaving interrupts disabled for long can
  // cause loss of serial data and other bad effects.
  void disableInterrupts();
  void enableInterrupts();

  // Returns true if interrupts are currently enabled.
  //
  // Where possible, prefer using the BlockInterrupts RAII class.
  bool interruptsEnabled();

  // Calculate CRC32 for data buffer
  uint32_t crc32(uint8_t *data, uint32_t length);

private:
  // Initializes watchdog, sets appropriate pins to OUTPUT, etc.  Called by
  // HalApi::init
  void watchdog_init();

#ifdef BARE_STM32
  // Initializes CRC32 peripheral
  void crc32_init();
  // Reset the hardware peripheral in STM32
  void crc32_reset();
  // Accumulate CRC32 in STM32 hardware
  void crc32_accumulate(uint8_t d);
  // Get CRC32 accumulated in STM32 hardware
  uint32_t crc32_get();

  void InitGPIO();
  void InitADC();
  void InitSysTimer();
  void BusyWaitUsec(uint16_t usec);
  void InitPwmOut();
  void InitUARTs();
  void EnableClock(void *ptr);
  void EnableInterrupt(int addr, IntPriority pri);
  void StepperMotorInit();
#endif

  void setDigitalPinMode(PwmPin pin, PinMode mode);
  void setDigitalPinMode(BinaryPin pin, PinMode mode);

#ifdef TEST_MODE
  Time time_ = millisSinceStartup(0);
  bool interruptsEnabled_ = true;

  // The default pin mode on Arduino is INPUT, which happens to be the first
  // enumerator in PinMode and so the default in these maps!
  //
  // Source: https://www.arduino.cc/en/Tutorial/DigitalPins
  // "Arduino (Atmega) pins default to input"
  std::map<PwmPin, PinMode> pwm_pin_modes_;
  std::map<BinaryPin, PinMode> binary_pin_modes_;

  std::map<AnalogPin, Voltage> analog_pin_values_;
  std::map<BinaryPin, VoltageLevel> binary_pin_values_;
  std::map<PwmPin, float> pwm_pin_values_;

  std::deque<std::vector<char>> serialIncomingData_;
  std::vector<char> serialOutgoingData_;
#endif
};

extern HalApi Hal;

// RAII class that disables interrupts.  For example:
//
// {
//   BlockInterrupts block;
//   // Interrupts are disabled until the close brace.
// }
//
// This class is reentrant, i.e. it's safe to BlockInterrupts even when
// interrupts are already disabled.
class [[nodiscard]] BlockInterrupts {
public:
  BlockInterrupts() : active_(Hal.interruptsEnabled()) {
    if (active_) {
      Hal.disableInterrupts();
    }
  }

  // Not copyable or moveable.  (Technically we could make this class
  // moveable if necessary, but it probably isn't!)
  BlockInterrupts(const BlockInterrupts &) = delete;
  BlockInterrupts(BlockInterrupts &&) = delete;
  BlockInterrupts &operator=(const BlockInterrupts &) = delete;
  BlockInterrupts &operator=(BlockInterrupts &&) = delete;

  ~BlockInterrupts() {
    if (active_) {
      Hal.enableInterrupts();
    }
  }

private:
  bool active_;
};

#if defined(BARE_STM32)

inline void HalApi::disableInterrupts() {
  asm volatile("cpsid i" ::: "memory");
}
inline void HalApi::enableInterrupts() { asm volatile("cpsie i" ::: "memory"); }
inline bool HalApi::interruptsEnabled() {
  int ret;
  asm volatile("mrs %[output], primask" : [output] "=r"(ret));
  return ret == 0;
}

#else
inline void HalApi::init() {}
inline void HalApi::watchdog_handler() {}

inline Time HalApi::now() { return time_; }
inline void HalApi::delay(Duration d) { time_ = time_ + d; }
inline Voltage HalApi::analogRead(AnalogPin pin) {
  return analog_pin_values_.at(pin);
}
inline void HalApi::test_setAnalogPin(AnalogPin pin, Voltage value) {
  analog_pin_values_[pin] = value;
}
inline void HalApi::setDigitalPinMode(PwmPin pin, PinMode mode) {
  pwm_pin_modes_[pin] = mode;
}
inline void HalApi::setDigitalPinMode(BinaryPin pin, PinMode mode) {
  binary_pin_modes_[pin] = mode;
}
inline void HalApi::digitalWrite(BinaryPin pin, VoltageLevel value) {
  if (binary_pin_modes_[pin] != PinMode::OUTPUT) {
    throw "Can only write to an OUTPUT pin";
  }
  binary_pin_values_[pin] = value;
}
inline void HalApi::analogWrite(PwmPin pin, float duty) {
  if (pwm_pin_modes_[pin] != PinMode::OUTPUT) {
    throw "Can only write to an OUTPUT pin";
  }
  pwm_pin_values_[pin] = duty;
}
[[nodiscard]] inline uint16_t HalApi::serialRead(char *buf, uint16_t len) {
  if (serialIncomingData_.empty()) {
    return 0;
  }
  auto &readBuf = serialIncomingData_.front();
  uint16_t n = std::min(len, static_cast<uint16_t>(readBuf.size()));
  memcpy(buf, readBuf.data(), n);
  readBuf.erase(readBuf.begin(), readBuf.begin() + n);
  if (readBuf.empty()) {
    serialIncomingData_.pop_front();
  }
  return n;
}
inline uint16_t HalApi::serialBytesAvailableForRead() {
  return serialIncomingData_.empty()
             ? 0
             : static_cast<uint16_t>(serialIncomingData_.front().size());
}
[[nodiscard]] inline uint16_t HalApi::serialWrite(const char *buf,
                                                  uint16_t len) {
  uint16_t n = std::min(len, serialBytesAvailableForWrite());
  serialOutgoingData_.insert(serialOutgoingData_.end(), buf, buf + n);
  return n;
}
inline uint16_t HalApi::serialBytesAvailableForWrite() {
  // TODO: Simulate partial writes?  For now, simply return the true size of
  // the Arduino tx buffer.
  return 64;
}
inline uint16_t HalApi::test_serialGetOutgoingData(char *data, uint16_t len) {
  uint16_t n = std::min(len, static_cast<uint16_t>(serialOutgoingData_.size()));
  memcpy(data, serialOutgoingData_.data(), n);
  serialOutgoingData_.erase(serialOutgoingData_.begin(),
                            serialOutgoingData_.begin() + n);
  return n;
}
inline void HalApi::test_serialPutIncomingData(const char *data, uint16_t len) {
  constexpr uint16_t MAX_MSG_SIZE = 64;
  while (len > MAX_MSG_SIZE) {
    serialIncomingData_.push_back(std::vector<char>(data, data + MAX_MSG_SIZE));
    data += MAX_MSG_SIZE;
    len = static_cast<uint16_t>(len - MAX_MSG_SIZE);
  }
  serialIncomingData_.push_back(std::vector<char>(data, data + len));
}

inline void HalApi::disableInterrupts() { interruptsEnabled_ = false; }
inline void HalApi::enableInterrupts() { interruptsEnabled_ = true; }
inline bool HalApi::interruptsEnabled() { return interruptsEnabled_; }

inline uint32_t HalApi::crc32(uint8_t *data, uint32_t length) {
  return soft_crc32(reinterpret_cast<char *>(data), length);
}

// NOTE - these functions are for debugging/testing the controller only.
// They aren't necessary or used when running in test mode
inline uint16_t HalApi::debugWrite(const char *buf, uint16_t len) {
  return len;
}
inline uint16_t HalApi::debugRead(char *buf, uint16_t len) { return 0; }

inline void HalApi::startLoopTimer(const Duration &period,
                                   void (*callback)(void *), void *arg) {}

#endif

#endif // HAL_H
