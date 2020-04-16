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
#include "gmock/gmock.h"
#define HAL_MOCK_METHOD(returntype, name, args)                                \
  MOCK_METHOD(returntype, name, args)
#else
#define HAL_MOCK_METHOD(returntype, name, args) returntype name args
#endif

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

  // TODO: Need at least one HAL_MOCK_METHOD.

private:
#ifdef TEST_MODE
  // Instance variables used when mocking HAL.

  uint32_t millis_ = 0;
  int pin_values_[15] = {0};
#endif
};

#undef HAL_MOCK_METHOD

extern HalApi Hal;

#ifdef AVR

#include <Arduino.h>

inline uint32_t HalApi::millis() { return ::millis(); }
inline void HalApi::delay(uint32_t ms) { ::delay(ms); }
inline int HalApi::analogRead(int pin) { return ::analogRead(pin); }

#else

inline uint32_t HalApi::millis() { return millis_; }
inline void HalApi::delay(uint32_t ms) { millis_ += ms; }
inline int HalApi::analogRead(int pin) { return pin_values_[pin]; }
inline void HalApi::test_setAnalogPin(int pin, int value) { pin_values_[pin] = value; }

#endif
