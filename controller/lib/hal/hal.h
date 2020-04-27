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

// A hardware abstraction layer that supports mocking/faking for tests.
//
// Once this is completed, you shouldn't include <Arduino.h> outside of this
// file; everything that interacts with the hardware should go through here.
//
// When running in test mode, HalApi is pedantically a combination of a mock
// and a fake.  All that means is,
//
//   - Some methods have "fake" implementations.  For example, instead of
//     telling the actual amount of time since boot, millis() stays constant,
//     and doesn't advance unless you call delay().
//
//   - Other methods are "mocked".  gMock provides a default do-nothing
//     implementation, and:
//      - If you want to check that a mocked method is called (e.g. you want
//        to check that data was sent along a channel, use EXPECT_CALL.
//      - If you want to provide a temporary ("mock") implementation of a
//        method, e.g. to make a "read" method return data, use ON_CALL.
//
// For more info, see
// https://github.com/google/googletest/blob/master/googlemock/docs/for_dummies.md
//
// In order to support mocking, the HAL has to be a class with virtual methods.
// But we don't want virtual methods when compiling for the controller, because
// they come at a cost.  Thus we do some macro magic to get virtual methods
// when compiling for test, but not when compiling for production.

#include "alg.h"

#ifdef TEST_MODE

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_NUCLEO_L452RE)
#error                                                                         \
    "TEST_MODE intended to be run only on native, but ARDUINO_AVR_UNO or ARDUINO_NUCLEO_L452RE is defined"
#endif

#include <deque>
#include <map>
#include <vector>

#include "gmock/gmock.h"

#define HAL_MOCK_METHOD(returntype, name, args)                                \
  MOCK_METHOD(returntype, name, args)
#define HAL_CONSTANT(name) HAL_##name

#else // !TEST_MODE

#if !defined(ARDUINO_AVR_UNO) && !defined(ARDUINO_NUCLEO_L452RE)
#error                                                                         \
    "When running without TEST_MODE, expecting ARDUINO_AVR_UNO or ARDUINO_NUCLEO_L452RE to be defined"
#endif
#include <Arduino.h>

#ifdef ARDUINO_AVR_UNO
#include <avr/wdt.h>
#endif

#define HAL_MOCK_METHOD(returntype, name, args) returntype name args

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
enum class PinMode : uint8_t {
  HAL_CONSTANT(INPUT),
  HAL_CONSTANT(OUTPUT),
  HAL_CONSTANT(INPUT_PULLUP)
};

// Voltage level of a digital pin.
// Usage: VoltageLevel::HAL_HIGH, HAL_LOW
enum class VoltageLevel : uint8_t { HAL_CONSTANT(HIGH), HAL_CONSTANT(LOW) };

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

// IDs of the digital pins that can be used for pulse-width modulation.
// https://github.com/arduino/ArduinoCore-avr/blob/257ee3f/variants/standard/pins_arduino.h#L35
enum class PwmPinId {
  PWM_3 = 3,
  PWM_5 = 5,
  PWM_6 = 6,
  PWM_9 = 9,
  PWM_10 = 10,
  PWM_11 = 11,
};

// Singleton class which implements a hardware abstraction layer.
//
// Access this via the `Hal` global variable, e.g. `Hal.millis()`.
class HalApi {
public:
  void init();

  // Number of milliseconds that have passed since the board started running the
  // program.
  //
  // Faked when mocking.  Time doesn't advance unless you call delay().
  uint32_t millis();

  // Sleeps for some number of milliseconds.
  //
  // Faked when mocking.  Does not sleep, but does advance the time returned by
  // millis().
  void delay(uint32_t ms);

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

  // TODO(jlebar): Make digital pin number strongly typed?  It's slightly
  // tricky because the digital pin numbers are a superset of PWM pin numbers,
  // and C++ doesn't support enum inheritance.
  void setDigitalPinMode(int pin, PinMode mode);
  void setDigitalPinMode(PwmPinId pin, PinMode mode) {
    setDigitalPinMode(static_cast<int>(pin), mode);
  }
  void analogWrite(PwmPinId pin, int value);
  void digitalWrite(int pin, VoltageLevel value);

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

#ifdef TEST_MODE
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

private:
  // Initializes watchdog, is called by HalApi::init
  void watchdog_init();
#ifdef TEST_MODE
  // Instance variables used when mocking HAL.

  uint32_t millis_ = 0;

  std::map<AnalogPin, int> analog_pin_values_;

  VoltageLevel digital_pin_values_[14] = {VoltageLevel::HAL_LOW};
  // The default pin mode on Arduino is INPUT.
  // Source: https://www.arduino.cc/en/Tutorial/DigitalPins
  // "Arduino (Atmega) pins default to input"
  PinMode digital_pin_modes_[14] = {PinMode::HAL_INPUT};

  // TODO: Really, PWM pins are digital pins - i.e., "writing to a PWM pin"
  // means "asking the device to set the digital pin to HIGH this% of the time".
  int pwm_pin_values_[14] = {0};

  std::deque<std::vector<char>> serialIncomingData_;
  std::vector<char> serialOutgoingData_;
#endif
};

#undef HAL_MOCK_METHOD

extern HalApi Hal;

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_NUCLEO_L452RE)

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
}
inline uint32_t HalApi::millis() { return ::millis(); }
inline void HalApi::delay(uint32_t ms) { ::delay(ms); }
inline int HalApi::analogRead(AnalogPin pin) {
  int raw_pin = [&] {
    switch (pin) {
    case AnalogPin::PATIENT_PRESSURE:
      return A0;
    case AnalogPin::INFLOW_PRESSURE_DIFF:
      return A1;
    case AnalogPin::OUTFLOW_PRESSURE_DIFF:
      return A2;
    }
    // Switch above covers all cases (and gcc enforces this).
    __builtin_unreachable();
  }();
  return ::analogRead(raw_pin);
}
inline void HalApi::setDigitalPinMode(int pin, PinMode mode) {
  ::pinMode(pin, static_cast<uint8_t>(mode));
}
inline void HalApi::digitalWrite(int pin, VoltageLevel value) {
  ::digitalWrite(pin, static_cast<uint8_t>(value));
}
inline void HalApi::analogWrite(PwmPinId pin, int value) {
  ::analogWrite(static_cast<int>(pin), value);
}
[[nodiscard]] inline uint16_t HalApi::serialRead(char *buf, uint16_t len) {
  return Serial.readBytes(buf, alg::min(len, serialBytesAvailableForRead()));
}
inline uint16_t HalApi::serialBytesAvailableForRead() {
  return Serial.available();
}
[[nodiscard]] inline uint16_t HalApi::serialWrite(const char *buf,
                                                  uint16_t len) {
  return Serial.write(buf, alg::min(len, serialBytesAvailableForWrite()));
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
#ifdef ARDUINO_AVR_UNO
  wdt_reset();
#endif
}

#else

inline uint32_t HalApi::millis() { return millis_; }
inline void HalApi::delay(uint32_t ms) { millis_ += ms; }
inline int HalApi::analogRead(AnalogPin pin) {
  return analog_pin_values_.at(pin);
}
inline void HalApi::test_setAnalogPin(AnalogPin pin, int value) {
  analog_pin_values_[pin] = value;
}
inline void HalApi::setDigitalPinMode(int pin, PinMode mode) {
  digital_pin_modes_[pin] = mode;
}
inline void HalApi::digitalWrite(int pin, VoltageLevel value) {
  if (digital_pin_modes_[pin] != PinMode::HAL_OUTPUT) {
    throw "Can only write to an OUTPUT pin";
  }
  digital_pin_values_[pin] = value;
}
inline void HalApi::analogWrite(PwmPinId pin, int value) {
  pwm_pin_values_[static_cast<int>(pin)] = value;
}
[[nodiscard]] inline uint16_t HalApi::serialRead(char *buf, uint16_t len) {
  if (serialIncomingData_.empty()) {
    return 0;
  }
  auto &readBuf = serialIncomingData_.front();
  uint16_t n = alg::min<uint16_t>(len, readBuf.size());
  memcpy(buf, readBuf.data(), n);
  readBuf.erase(readBuf.begin(), readBuf.begin() + n);
  if (readBuf.empty()) {
    serialIncomingData_.pop_front();
  }
  return n;
}
inline uint16_t HalApi::serialBytesAvailableForRead() {
  return serialIncomingData_.empty() ? 0 : serialIncomingData_.front().size();
}
[[nodiscard]] inline uint16_t HalApi::serialWrite(const char *buf,
                                                  uint16_t len) {
  uint16_t n = alg::min(len, serialBytesAvailableForWrite());
  serialOutgoingData_.insert(serialOutgoingData_.end(), buf, buf + n);
  return n;
}
inline uint16_t HalApi::serialBytesAvailableForWrite() {
  // TODO: Simulate partial writes?  For now, simply return the true size of
  // the Arduino tx buffer.
  return 64;
}
inline uint16_t HalApi::test_serialGetOutgoingData(char *data, uint16_t len) {
  uint16_t n = alg::min<uint16_t>(len, serialOutgoingData_.size());
  memcpy(data, serialOutgoingData_.data(), n);
  serialOutgoingData_.erase(serialOutgoingData_.begin(),
                            serialOutgoingData_.begin() + n);
  return n;
}
inline void HalApi::test_serialPutIncomingData(const char *data, uint16_t len) {
  constexpr int MAX_MSG_SIZE = 64;
  while (len > MAX_MSG_SIZE) {
    serialIncomingData_.push_back(std::vector<char>(data, data + MAX_MSG_SIZE));
    data += MAX_MSG_SIZE;
    len -= MAX_MSG_SIZE;
  }
  serialIncomingData_.push_back(std::vector<char>(data, data + len));
}

#endif

#endif // HAL_H
