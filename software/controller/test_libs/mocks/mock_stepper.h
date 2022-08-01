/* Copyright 2022, RespiraWorks

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

#include "mock_spi.h"
#include "stepper.h"

namespace StepMotor {

enum class MoveDir { Shortest, Negative, Positive };

// Class that allows us to (very approximately) emulate a stepper motor
class TestStepper {
 public:
  TestStepper(float low_stop, float high_stop, bool power_step);

  // Action that the stepper does when receiving a byte
  void ReceiveByte(uint8_t byte);
  uint8_t GetNextReponseByte();

  float Position() { return position_; };

  void ResetDevice();

  void MoveToPosition(int32_t microsteps, MoveDir dir);

  void ExecuteCommand();

  void SetParamIfAccessible(size_t address, uint32_t value);

  void GetParam(size_t address);

  void EnableDevice() { status_.bitfield.disabled = false; }

  void DisableDevice() { status_.bitfield.disabled = true; }

  // buffer to hold current command
  uint8_t received_command_[4] = {0};
  size_t command_index_{0};
  size_t command_length_{0};

  // buffer to hold response to last command
  uint8_t response_[3] = {0};
  size_t response_index_{0};
  size_t response_count_{0};

  // Current position (in degrees).
  float position_{0};
  // Stop positions should allow us to use this class to test the pinch valves as well.
  float low_stop_;
  float high_stop_;

  // we emulate differences in behaviour between PowerSTEP01 and L6470
  bool power_step_;
  size_t status_address_;

  // status of the stepper driver
  Status status_{.word = 0x7C03};  // init value based on tests ran with L6470 chip
  // params array
  uint32_t params_[32] = {0};

  float MicroStepsPerDeg();
};

}  // namespace StepMotor
