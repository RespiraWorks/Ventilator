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

#include <stdint.h>

#include <algorithm>

#include "units.h"

#ifdef TEST_MODE

#if defined(BARE_STM32)
#error "TEST_MODE intended to be run only on native, but BARE_STM32 is defined"
#endif

#include <assert.h>

#include <cstring>
#include <deque>
#include <map>
#include <vector>

#include "checksum.h"

#else  // !TEST_MODE

#if !defined(BARE_STM32)
#error "When running without TEST_MODE, expecting BARE_STM32 to be defined"
#endif

#endif  // TEST_MODE

// ---------------------------------------------------------------
// Strongly typed analogues of some Arduino types.
// "Strongly typed" means that it will be a compile error, e.g.,
// to pass a PWM pin id to a function expecting an analog pin id.
// ---------------------------------------------------------------

// Mode of a digital pin.
// Usage: PinMode::Input etc.
enum class PinMode {
  // Test code relies on Input being the first enumeration (to get the
  // behavior that Input pins are the default).
  Input,
  Output,
  InputPullup
};

// Voltage level of a digital pin.
// Usage: VoltageLevel::High, Low
enum class VoltageLevel { High, Low };

// Location of analog sensors as labeled on the PCB(s). Note that this does not necessarily define
// their function and further mapping should be done in the higher layers of the software.
enum class AnalogPin {
  InterimBoardAnalogPressure,
  U3PatientPressure,
  U4InhaleFlow,
  U5ExhaleFlow,
  InterimBoardOxygenSensor,
};

// Pulse-width modulated outputs from the controller.  These can be set to
// values in [0-255].
//
// Pins default to Input, so if you add a new pin here, be sure to update
// HalApi::Init() and set it to Output!
enum class PwmPin {
  // Controls the fan speed.
  Blower,
};

// Binary pins set by the controller -- these are booleans, High or Low.
//
// PWM pins can of course be HIGH or LOW too, but we separate out purely on/off
// pins from PWM pins for reasons of "strong typing".
//
// Pins default to Input, so if you add a new pin here, be sure to update
// HalApi::Init() and set it to Output!
enum class BinaryPin {
  RedLED,
  YellowLED,
  GreenLED,
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
  Critical = 2,  // Very important interrupt
  Standard = 5,  // Normal hardware interrupts
  Low = 8,       // Less important.  Hardware interrupts can interrupt this
};

enum class InterruptVector;

#ifdef TEST_MODE
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
#endif  // TEST_MODE

// Singleton class which implements a hardware abstraction layer.
//
// Access this via the `hal` global variable, e.g. `hal.millis()`.
//
// TODO: Make Hal a namespace rather than a class.  Then this header won't need
// any ifdefs for different platforms, and all of the "global variables" can
// move into the hal_foo.cpp files.
class HalApi {
 public:
  void Init();

  // Amount of time that has passed since the board started running the
  // program.
  //
  // Faked when testing.  Time doesn't advance unless you call Delay().
  Time Now();

  // Sleeps for some number of milliseconds.
  //
  // Faked when testing.  Does not sleep, but does advance the time returned
  // by millis().
  void Delay(Duration d);

  // Caveat for people new to Arduino: AnalogRead and AnalogWrite are
  // completely separate from each other and do not even refer to the same
  // pins. AnalogRead() reads the value of an analog input pin. AnalogWrite()
  // writes to a PWM pin - some of the digital pins are PWM pins.

  // Reads from analog sensor using an analog-to-digital converter.
  //
  // Returns a voltage.  On STM32 this can range from 0 to 3.3V.
  //
  // In test mode, will return the last value set via TESTSetAnalogPin.
  Voltage AnalogRead(AnalogPin pin) const;

#ifdef TEST_MODE
  void TESTSetAnalogPin(AnalogPin pin, Voltage value);
#endif

  // Causes `pin` to output a square wave with the given duty cycle (range
  // [0, 1]).
  //
  // Perhaps a better name would be "pwmWrite", but we also want to be
  // somewhat consistent with the Arduino API that people are familiar with.
  void AnalogWrite(PwmPin pin, float duty);

  // Sets `pin` to high or low.
  void DigitalWrite(BinaryPin pin, VoltageLevel value);

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

  // Buzzer used for alarms.  These functions turn the buzzer on/off.
  void BuzzerOn(float volume = 1.0f);
  void BuzzerOff();

  // PSOL (Proportional Solenoid) support
  void InitPSOL();
  void PSolValue(float val);

  // Erase a page of flash given the starting address of that page.
  bool FlashErasePage(uint32_t address);

  // Write data to flash memory at the specified address
  // Note that the STM32 only allows writing in multiples of 8 bytes
  // to addresses that are a multiple of 8.
  //
  // @param addr - Address in flash memory at which to write.
  //               NOTE - must be a multiple of 8
  // @param data - Pointer to data to write
  // @param ct   - Number of bytes to write
  //               NOTE - must be a multiple of 8
  bool FlashWrite(uint32_t addr, void *data, size_t ct);

#ifndef TEST_MODE
  // Translates to a numeric pin that can be passed to the Arduino API.
  uint8_t RawPin(PwmPin pin);
  uint8_t RawPin(AnalogPin pin);
  uint8_t RawPin(BinaryPin pin);

  // Perform some early chip initialization before static constructors are run
  void EarlyInit();

#else
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
#endif

  // Performs the device soft-reset
  [[noreturn]] void ResetDevice();

  // Start the loop timer
  void StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg);

  // Pets the watchdog, this makes the watchdog not reset the
  // system for configured amount of time
  void WatchdogHandler();

  // Interrupt enable/disable functions.
  //
  // Where possible, prefer using the BlockInterrupts RAII class.
  //
  // NOTE: Interrupts should only be disabled for short periods of time and
  // only for very good reasons.  Leaving interrupts disabled for long can
  // cause loss of serial data and other bad effects.
  void DisableInterrupts();
  void EnableInterrupts();

  // Returns true if interrupts are currently enabled.
  //
  // Where possible, prefer using the BlockInterrupts RAII class.
  bool InterruptsEnabled() const;

  // Return true if we are currently executing in an interrupt handler
  bool InInterruptHandler();

 private:
  // Initializes watchdog, sets appropriate pins to Output, etc.  Called by
  // HalApi::Init
  void WatchdogInit();

#ifdef BARE_STM32
  void InitGpio();
  void InitADC();
  void InitI2C();
  void InitSysTimer();
  void InitPwmOut();
  void InitUARTs();
  void EnableClock(volatile void *ptr);
  void EnableInterrupt(InterruptVector vec, IntPriority pri);
  void StepperMotorInit();
  void InitBuzzer();

#endif

  void SetDigitalPinMode(PwmPin pin, PinMode mode);
  void SetDigitalPinMode(BinaryPin pin, PinMode mode);

#ifdef TEST_MODE
  Time time_ = microsSinceStartup(0);
  bool interrupts_enabled_ = true;

  // The default pin mode on Arduino is Input, which happens to be the first
  // enumerator in PinMode and so the default in these maps!
  //
  // Source: https://www.arduino.cc/en/Tutorial/DigitalPins
  // "Arduino (Atmega) pins default to input"
  std::map<PwmPin, PinMode> pwm_pin_modes_;
  std::map<BinaryPin, PinMode> binary_pin_modes_;

  std::map<AnalogPin, Voltage> analog_pin_values_;
  std::map<BinaryPin, VoltageLevel> binary_pin_values_;
  std::map<PwmPin, float> pwm_pin_values_;

  TestSerialPort serial_port_;
  TestSerialPort debug_serial_port_;
#endif
};

extern HalApi hal;

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
  BlockInterrupts() : active_(hal.InterruptsEnabled()) {
    if (active_) {
      hal.DisableInterrupts();
    }
  }

  // Not copyable or movable.  (Technically we could make this class
  // movable if necessary, but it probably isn't!)
  BlockInterrupts(const BlockInterrupts &) = delete;
  BlockInterrupts(BlockInterrupts &&) = delete;
  BlockInterrupts &operator=(const BlockInterrupts &) = delete;
  BlockInterrupts &operator=(BlockInterrupts &&) = delete;

  ~BlockInterrupts() {
    if (active_) {
      hal.EnableInterrupts();
    }
  }

 private:
  bool active_;
};

#if defined(BARE_STM32)

inline void HalApi::DisableInterrupts() { asm volatile("cpsid i" ::: "memory"); }
inline void HalApi::EnableInterrupts() { asm volatile("cpsie i" ::: "memory"); }
inline bool HalApi::InterruptsEnabled() const {
  int ret;
  asm volatile("mrs %[output], primask" : [output] "=r"(ret));
  return ret == 0;
}

inline bool HalApi::InInterruptHandler() {
  int ret;
  asm volatile("mrs %[output], ipsr" : [output] "=r"(ret));
  return ret > 0;
}

#else
inline void HalApi::Init() {}
inline void HalApi::WatchdogHandler() {}

inline Time HalApi::Now() { return time_; }
inline void HalApi::Delay(Duration d) { time_ = time_ + d; }
inline Voltage HalApi::AnalogRead(AnalogPin pin) const { return analog_pin_values_.at(pin); }
inline void HalApi::TESTSetAnalogPin(AnalogPin pin, Voltage value) {
  analog_pin_values_[pin] = value;
}
inline void HalApi::SetDigitalPinMode(PwmPin pin, PinMode mode) { pwm_pin_modes_[pin] = mode; }
inline void HalApi::SetDigitalPinMode(BinaryPin pin, PinMode mode) {
  binary_pin_modes_[pin] = mode;
}
inline void HalApi::DigitalWrite(BinaryPin pin, VoltageLevel value) {
  if (binary_pin_modes_[pin] != PinMode::Output) {
    assert(false && "Can only write to an OUTPUT pin");
  }
  binary_pin_values_[pin] = value;
}
inline void HalApi::AnalogWrite(PwmPin pin, float duty) {
  if (pwm_pin_modes_[pin] != PinMode::Output) {
    assert(false && "Can only write to an OUTPUT pin");
  }
  pwm_pin_values_[pin] = duty;
}

inline uint16_t HalApi::SerialRead(char *buf, uint16_t len) { return serial_port_.Read(buf, len); }
inline uint16_t HalApi::SerialBytesAvailableForRead() {
  return serial_port_.BytesAvailableForRead();
}
inline uint16_t HalApi::SerialWrite(const char *buf, uint16_t len) {
  return serial_port_.Write(buf, len);
}
inline uint16_t HalApi::SerialBytesAvailableForWrite() {
  return serial_port_.BytesAvailableForWrite();
}
inline uint16_t HalApi::TESTSerialGetOutgoingData(char *data, uint16_t len) {
  return serial_port_.GetOutgoingData(data, len);
}
inline void HalApi::TESTSerialPutIncomingData(const char *data, uint16_t len) {
  serial_port_.PutIncomingData(data, len);
}

inline uint16_t HalApi::DebugRead(char *buf, uint16_t len) {
  return debug_serial_port_.Read(buf, len);
}
inline uint16_t HalApi::DebugBytesAvailableForRead() {
  return debug_serial_port_.BytesAvailableForRead();
}
inline uint16_t HalApi::DebugWrite(const char *buf, uint16_t len) {
  return debug_serial_port_.Write(buf, len);
}
inline uint16_t HalApi::DebugBytesAvailableForWrite() {
  return debug_serial_port_.BytesAvailableForWrite();
}
inline uint16_t HalApi::TESTDebugGetOutgoingData(char *data, uint16_t len) {
  return debug_serial_port_.GetOutgoingData(data, len);
}
inline void HalApi::TESTDebugPutIncomingData(const char *data, uint16_t len) {
  debug_serial_port_.PutIncomingData(data, len);
}

inline void HalApi::DisableInterrupts() { interrupts_enabled_ = false; }
inline void HalApi::EnableInterrupts() { interrupts_enabled_ = true; }
inline bool HalApi::InterruptsEnabled() const { return interrupts_enabled_; }
inline bool HalApi::InInterruptHandler() { return false; }

inline uint16_t TestSerialPort::Read(char *buf, uint16_t len) {
  if (incoming_data_.empty()) {
    return 0;
  }
  auto &read_buffer = incoming_data_.front();
  uint16_t n = std::min(len, static_cast<uint16_t>(read_buffer.size()));
  memcpy(buf, read_buffer.data(), n);
  read_buffer.erase(read_buffer.begin(), read_buffer.begin() + n);
  if (read_buffer.empty()) {
    incoming_data_.pop_front();
  }
  return n;
}
inline uint16_t TestSerialPort::BytesAvailableForRead() {
  return incoming_data_.empty() ? 0 : static_cast<uint16_t>(incoming_data_.front().size());
}
inline uint16_t TestSerialPort::Write(const char *buf, uint16_t len) {
  uint16_t n = std::min(len, BytesAvailableForWrite());
  outgoing_data_.insert(outgoing_data_.end(), buf, buf + n);
  return n;
}
inline uint16_t TestSerialPort::BytesAvailableForWrite() {
  // TODO: Simulate partial writes?  For now, simply return the true size of
  // the Arduino tx buffer.
  return 64;
}
inline uint16_t TestSerialPort::GetOutgoingData(char *data, uint16_t len) {
  uint16_t n = std::min(len, static_cast<uint16_t>(outgoing_data_.size()));
  memcpy(data, outgoing_data_.data(), n);
  outgoing_data_.erase(outgoing_data_.begin(), outgoing_data_.begin() + n);
  return n;
}
inline void TestSerialPort::PutIncomingData(const char *data, uint16_t len) {
  constexpr uint16_t max_message_size = 64;
  while (len > max_message_size) {
    incoming_data_.push_back(std::vector<char>(data, data + max_message_size));
    data += max_message_size;
    len = static_cast<uint16_t>(len - max_message_size);
  }
  incoming_data_.push_back(std::vector<char>(data, data + len));
}

inline void HalApi::StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg) {}

inline void BuzzerOn(float volume) {}
inline void BuzzerOff() {}
inline void InitPSOL() {}
inline void PSolValue(float val) {}
inline bool HalApi::FlashErasePage(uint32_t address) { return true; }
inline bool HalApi::FlashWrite(uint32_t addr, void *data, size_t ct) { return true; }

#endif
