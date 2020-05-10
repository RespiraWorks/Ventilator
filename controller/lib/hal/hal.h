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
// You shouldn't include <Arduino.h> outside of this file; everything that
// interacts with the hardware should go through here.
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

#ifdef TEST_MODE

#if defined(ARDUINO_AVR_UNO) || defined(BARE_STM32)
#error                                                                         \
    "TEST_MODE intended to be run only on native, but ARDUINO_AVR_UNO or BARE_STM32 is defined"
#endif

#include <cstring>
#include <deque>
#include <map>
#include <vector>

#define HAL_CONSTANT(name) HAL_##name

#else // !TEST_MODE

#if !defined(ARDUINO_AVR_UNO) && !defined(BARE_STM32)
#error                                                                         \
    "When running without TEST_MODE, expecting ARDUINO_AVR_UNO or BARE_STM32 to be defined"
#endif

#if defined(BARE_STM32)
#include <stdint.h>

// Some constants that would normally be in the Arduino header
// which we're not using on the bare system

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

#else
#include <Arduino.h>
#endif // BARE_STM32

#ifdef ARDUINO_AVR_UNO
#include <avr/wdt.h>
#endif

// "HAL" has to be there because the respective Arduino symbols are macros,
// e.g. A0 expands to 14, so we can't have a constant named A0.
#define HAL_CONSTANT(name) HAL_##name = name
#endif // TEST_MODE

// ---------------------------------------------------------------
// Strongly typed analogues of some Arduino types.
// "Strongly typed" means that it will be a compile error, e.g.,
// to pass a PWM pin id to a function expecting an analog pin id.
// ---------------------------------------------------------------

// Mode of a digital pin.
// Usage: PinMode::HAL_INPUT etc.
enum class PinMode {
  // Test code relies on INPUT being the first enumeration (to get the behavior
  // that INPUT pins are the default).
  HAL_CONSTANT(INPUT),
  HAL_CONSTANT(OUTPUT),
  HAL_CONSTANT(INPUT_PULLUP)
};

// Voltage level of a digital pin.
// Usage: VoltageLevel::HAL_HIGH, HAL_LOW
enum class VoltageLevel { HAL_CONSTANT(HIGH), HAL_CONSTANT(LOW) };

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

// Singleton class which implements a hardware abstraction layer.
//
// Access this via the `Hal` global variable, e.g. `Hal.millis()`.
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

  // Reads from analog sensor.
  //
  // On Arduino, the analog-to-digital converter has 10 bits of precision, so
  // you get a number in the range [0, 1024).
  //
  // In test mode, will return the last value set via test_setAnalogPin.
  //
  // TODO: Implementation is currently incorrect on STM32,
  // https://github.com/RespiraWorks/VentilatorSoftware/pull/186#discussion_r415436954
  int analogRead(AnalogPin pin);

#ifdef TEST_MODE
  void test_setAnalogPin(AnalogPin pin, int value);
#endif

  // Causes `pin` to output a square wave with duty cycle determined by value
  // (range [0, 255]).
  //
  // Perhaps a better name would be "pwmWrite", but we also want to be somewhat
  // consistent with the Arduino API that people are familiar with.
  void analogWrite(PwmPin pin, int value);

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

#ifndef TEST_MODE
  // Translates to a numeric pin that can be passed to the Arduino API.
  uint8_t rawPin(PwmPin pin);
  uint8_t rawPin(AnalogPin pin);
  uint8_t rawPin(BinaryPin pin);

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

  // Pets the watchdog, this makes the watchdog not reset the
  // system for configured amount of time
  void watchdog_handler();

  // Interrupt enable/disable functions.
  //
  // NOTE:
  // Interrupts should only be disabled for short periods of time and only for
  // very good reasons.  Leaving interrupts disabled for long can cause loss of
  // serial data and other bad effects.

  // Disable interrupts
  void IntDisable();

  // Enable interrupts
  void IntEnable();

  // Disable interrutps and return true if they were enabled when the function
  // was called.  Return false if interrupts were already disabled.
  bool IntSuspend();

  // Restore interrupts to the state they were in when IntSuspend was last
  // called.  The return value from IntSuspend is passed in.
  // If the input value is false, this function has no effect.
  void IntRestore(bool yes) {
    if (yes)
      IntEnable();
  }

private:
  // Initializes watchdog, sets appropriate pins to OUTPUT, etc.  Called by
  // HalApi::init
  void watchdog_init();

  void setDigitalPinMode(PwmPin pin, PinMode mode);
  void setDigitalPinMode(BinaryPin pin, PinMode mode);

#ifdef TEST_MODE
  Time time_ = millisSinceStartup(0);

  // The default pin mode on Arduino is INPUT, which happens to be the first
  // enumerator in PinMode and so the default in these maps!
  //
  // Source: https://www.arduino.cc/en/Tutorial/DigitalPins
  // "Arduino (Atmega) pins default to input"
  std::map<PwmPin, PinMode> pwm_pin_modes_;
  std::map<BinaryPin, PinMode> binary_pin_modes_;

  std::map<AnalogPin, int> analog_pin_values_;
  std::map<BinaryPin, VoltageLevel> binary_pin_values_;
  std::map<PwmPin, int> pwm_pin_values_;

  std::deque<std::vector<char>> serialIncomingData_;
  std::vector<char> serialOutgoingData_;
#endif

#if defined(ARDUINO_AVR_UNO)
  // Value of ::millis() at last call to watchdog_handler().  Used to detect
  // clock overflows.
  uint32_t last_millis_ = 0;

  // Number of times millis() has gone backwards.   Arduino millis is a uint32,
  // so this happens once every 1.6 months of uptime.
  uint32_t num_clock_overflows_ = 0;
#endif
};

extern HalApi Hal;

#if defined(ARDUINO_AVR_UNO)

inline void HalApi::init() {

  // Hal.init() should be the first thing called during initialization.
  // Among other things, it initializes the watchdog, and that needs to happen
  // very early in startup because:
  //
  //  - The purpose of the watchdog is to catch hangs, and if initializing the
  //    watchdog weren't the first thing we did, we wouldn't catch hangs in the
  //    work that came before.
  //
  //  - After the device is soft-reset via reset_device(), the watchdog timer
  //    has a very short value.  We need to Hal.init() immediately so that
  //    we don't time out while initializing.
  watchdog_init();

  constexpr int32_t BAUD_RATE_BPS = 115200;
  Serial.begin(BAUD_RATE_BPS, SERIAL_8N1);

  setDigitalPinMode(PwmPin::BLOWER, PinMode::HAL_OUTPUT);
  setDigitalPinMode(BinaryPin::SOLENOID, PinMode::HAL_OUTPUT);
}
inline Time HalApi::now() {
  return millisSinceStartup((uint64_t{1} << 32) * num_clock_overflows_ +
                            ::millis());
}
inline void HalApi::delay(Duration d) {
  ::delay(static_cast<unsigned long>(d.milliseconds()));
}

inline uint8_t HalApi::rawPin(AnalogPin pin) {
  // See pinout at https://bit.ly/3aERr69.
  // TODO: Update with STM32 pinout.
  switch (pin) {
  case AnalogPin::PATIENT_PRESSURE:
    return A5;
  case AnalogPin::INFLOW_PRESSURE_DIFF:
    return A6;
  case AnalogPin::OUTFLOW_PRESSURE_DIFF:
    return A7;
  }
  // Switch above covers all cases (and gcc enforces this).
  __builtin_unreachable();
}

inline uint8_t HalApi::rawPin(PwmPin pin) {
  // See pinout at https://bit.ly/3aERr69.
  // TODO: Update with STM32 pinout.
  switch (pin) {
  case PwmPin::BLOWER:
    return 6;
  }
  // Switch above covers all cases (and gcc enforces this).
  __builtin_unreachable();
}

inline uint8_t HalApi::rawPin(BinaryPin pin) {
  // See pinout at https://bit.ly/3aERr69.
  // TODO: Update with STM32 pinout.
  switch (pin) {
  case BinaryPin::SOLENOID:
    return 7;
  }
  // Switch above covers all cases (and gcc enforces this).
  __builtin_unreachable();
}

inline int HalApi::analogRead(AnalogPin pin) {
  return ::analogRead(rawPin(pin));
}
inline void HalApi::setDigitalPinMode(PwmPin pin, PinMode mode) {
  ::pinMode(rawPin(pin), static_cast<uint8_t>(mode));
}
inline void HalApi::setDigitalPinMode(BinaryPin pin, PinMode mode) {
  ::pinMode(rawPin(pin), static_cast<uint8_t>(mode));
}
inline void HalApi::digitalWrite(BinaryPin pin, VoltageLevel value) {
  ::digitalWrite(rawPin(pin), static_cast<uint8_t>(value));
}
inline void HalApi::analogWrite(PwmPin pin, int value) {
  ::analogWrite(rawPin(pin), value);
}
[[nodiscard]] inline uint16_t HalApi::serialRead(char *buf, uint16_t len) {
  return Serial.readBytes(buf, stl::min(len, serialBytesAvailableForRead()));
}
inline uint16_t HalApi::serialBytesAvailableForRead() {
  return Serial.available();
}
[[nodiscard]] inline uint16_t HalApi::serialWrite(const char *buf,
                                                  uint16_t len) {
  return Serial.write(buf, stl::min(len, serialBytesAvailableForWrite()));
}
inline uint16_t HalApi::serialBytesAvailableForWrite() {
  return Serial.availableForWrite();
}

inline void HalApi::watchdog_init() {
#ifdef ARDUINO_AVR_UNO
  // FIXME Does this pose potential issues for arduino code updates?

  // The device will be reset if watchdog_handle is not called within roughly
  // this time period.
  //
  // Options are: 15ms, 30ms, 60ms, 120ms, 250ms, 500ms, 1s, 2s, 4s, 8s.
  //
  // TODO: This value was not chosen carefully.
  wdt_enable(WDTO_120MS);
#endif
}
[[noreturn]] inline void HalApi::reset_device() {
#ifdef ARDUINO_AVR_UNO
  // Reset the device by setting a short watchdog timeout and then entering an
  // infinite loop.
  wdt_enable(WDTO_15MS);
  while (true) {
  }
#endif
}
inline void HalApi::watchdog_handler() {
  // Check for clock overflow.  We need to run this at least once every
  // 2^32ms = 1.6mo.  The watchdog handler has to be called at least as
  // frequently as the watchdog timeout, and the largest timeout Arduino
  // supports is 8s.  That gives us a little wiggle room.  :)
  uint32_t millis = ::millis();
  if (millis < last_millis_) {
    num_clock_overflows_++;
  }
  last_millis_ = millis;

#ifdef ARDUINO_AVR_UNO
  wdt_reset();
#endif
}

// Interrupt disable/enable not supported for Arduino HALs yet.
// I'm assuming these are going away soon so don't require new
// functionality
inline void HalApi::IntDisable() {}
inline void HalApi::IntEnable() {}
inline bool HalApi::IntSuspend() { return false; }

// Support for bare STM32.  Only a few inline functions are defined here.
// Most support for this HAL is in a separate cpp file.
#elif defined(BARE_STM32)
// Disable interrupts.
// Returns true if interrupts were enabled when this
// was called or false if they were already disabled.

// Disable interrupts
inline void HalApi::IntDisable() { asm volatile("cpsid i" ::: "memory"); }

// Enable interrupts
inline void HalApi::IntEnable() { asm volatile("cpsie i" ::: "memory"); }

// Disable interrutps and return true if they were enabled when the function
// was called.  Return false if interrupts were already disabled.
inline bool HalApi::IntSuspend() {
  int ret;
  asm volatile("mrs   %[output], primask\n\t"
               "cpsid i"
               : [output] "=r"(ret)::"memory");
  return (ret == 0);
}

#else
inline void HalApi::init() {}
inline void HalApi::watchdog_handler() {}

inline Time HalApi::now() { return time_; }
inline void HalApi::delay(Duration d) { time_ = time_ + d; }
inline int HalApi::analogRead(AnalogPin pin) {
  return analog_pin_values_.at(pin);
}
inline void HalApi::test_setAnalogPin(AnalogPin pin, int value) {
  analog_pin_values_[pin] = value;
}
inline void HalApi::setDigitalPinMode(PwmPin pin, PinMode mode) {
  pwm_pin_modes_[pin] = mode;
}
inline void HalApi::setDigitalPinMode(BinaryPin pin, PinMode mode) {
  binary_pin_modes_[pin] = mode;
}
inline void HalApi::digitalWrite(BinaryPin pin, VoltageLevel value) {
  if (binary_pin_modes_[pin] != PinMode::HAL_OUTPUT) {
    throw "Can only write to an OUTPUT pin";
  }
  binary_pin_values_[pin] = value;
}
inline void HalApi::analogWrite(PwmPin pin, int value) {
  if (pwm_pin_modes_[pin] != PinMode::HAL_OUTPUT) {
    throw "Can only write to an OUTPUT pin";
  }
  pwm_pin_values_[pin] = value;
}
[[nodiscard]] inline uint16_t HalApi::serialRead(char *buf, uint16_t len) {
  if (serialIncomingData_.empty()) {
    return 0;
  }
  auto &readBuf = serialIncomingData_.front();
  uint16_t n = stl::min(len, static_cast<uint16_t>(readBuf.size()));
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
  uint16_t n = stl::min(len, serialBytesAvailableForWrite());
  serialOutgoingData_.insert(serialOutgoingData_.end(), buf, buf + n);
  return n;
}
inline uint16_t HalApi::serialBytesAvailableForWrite() {
  // TODO: Simulate partial writes?  For now, simply return the true size of
  // the Arduino tx buffer.
  return 64;
}
inline uint16_t HalApi::test_serialGetOutgoingData(char *data, uint16_t len) {
  uint16_t n = stl::min(len, static_cast<uint16_t>(serialOutgoingData_.size()));
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

inline void HalApi::IntDisable() {}
inline void HalApi::IntEnable() {}
inline bool HalApi::IntSuspend() { return false; }

#endif

#endif // HAL_H
