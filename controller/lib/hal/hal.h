#include <stdint.h>

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

#ifdef TEST_MODE

#ifdef AVR
#error "TEST_MODE intended to be run only on native, but AVR is defined"
#endif

#include "gmock/gmock.h"
#define HAL_MOCK_METHOD(returntype, name, args)                                \
  MOCK_METHOD(returntype, name, args)
#define HAL_CONSTANT(name) HAL_##name

#else // !TEST_MODE

#ifndef AVR
#error "When running without TEST_MODE, expecting AVR to be defined"
#endif
#include <Arduino.h>

#define HAL_MOCK_METHOD(returntype, name, args) returntype name args
#define HAL_CONSTANT(name) HAL_##name = name
#endif // TEST_MODE

enum class PinMode : uint8_t {
  HAL_CONSTANT(INPUT),
  HAL_CONSTANT(OUTPUT),
  HAL_CONSTANT(INPUT_PULLUP)
};

enum class VoltageLevel : uint8_t { HAL_CONSTANT(HIGH), HAL_CONSTANT(LOW) };

// Singleton class which implements a hardware abstraction layer.
//
// Access this via the `Hal` global variable, e.g. `Hal.millis()`.
class HalApi {
public:
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

  // In test mode, will return the last value set via test_setAnalogPin.
  int analogRead(int pin);
#ifdef TEST_MODE
  void test_setAnalogPin(int pin, int value);
#endif

  void setDigitalPinMode(int pin, PinMode mode);
  void digitalWrite(int pin, VoltageLevel value);

  // TODO: Need at least one HAL_MOCK_METHOD.

private:
#ifdef TEST_MODE
  // Instance variables used when mocking HAL.

  uint32_t millis_ = 0;
  // Arduino Uno has 6 analog input pins and 14 digital input/output pins.
  // Source: https://store.arduino.cc/usa/arduino-uno-rev3
  int analog_pin_values_[6] = {0};

  VoltageLevel digital_pin_values_[14] = {VoltageLevel::HAL_LOW};
  // The default pin mode on Arduino is INPUT.
  // Source: https://www.arduino.cc/en/Tutorial/DigitalPins
  // "Arduino (Atmega) pins default to input"
  PinMode digital_pin_modes_[14] = {PinMode::HAL_INPUT};
#endif
};

#undef HAL_MOCK_METHOD

extern HalApi Hal;

#ifdef AVR

inline uint32_t HalApi::millis() { return ::millis(); }
inline void HalApi::delay(uint32_t ms) { ::delay(ms); }
inline int HalApi::analogRead(int pin) { return ::analogRead(pin); }
inline void HalApi::setDigitalPinMode(int pin, PinMode mode) {
  ::pinMode(pin, static_cast<uint8_t>(mode));
}
inline void HalApi::digitalWrite(int pin, VoltageLevel value) {
  ::digitalWrite(pin, static_cast<uint8_t>(value));
}

#else

inline uint32_t HalApi::millis() { return millis_; }
inline void HalApi::delay(uint32_t ms) { millis_ += ms; }
inline int HalApi::analogRead(int pin) { return analog_pin_values_[pin]; }
inline void HalApi::test_setAnalogPin(int pin, int value) {
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

#endif
