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

#include "fake_stepper.h"

#include <cfloat>  // for FLT_EPSILON
#include <cmath>   // for fmod and fabsf

namespace StepMotor {

enum class ParamAccess { ReadOnly, WriteWhenDisabled, WriteWhenStopped, WriteWhenever };

// Access based on POWERSTEP01 datasheet table 12 (p50).
static constexpr ParamAccess ParamsAccess[32] = {
    ParamAccess::ReadOnly,           // No such param
    ParamAccess::WriteWhenStopped,   // Position
    ParamAccess::WriteWhenDisabled,  // Elecrical Position
    ParamAccess::WriteWhenever,      // Mark position
    ParamAccess::ReadOnly,           // Current speed
    ParamAccess::WriteWhenStopped,   // Acceleration
    ParamAccess::WriteWhenStopped,   // Deceleration
    ParamAccess::WriteWhenever,      // Max speed
    ParamAccess::WriteWhenStopped,   // Min speed
    ParamAccess::WriteWhenever,      // KVal for Hold
    ParamAccess::WriteWhenever,      // KVal for Run
    ParamAccess::WriteWhenever,      // KVal for Acceleration
    ParamAccess::WriteWhenever,      // KVal for Deceleration
    ParamAccess::WriteWhenDisabled,  // Intersect speed
    ParamAccess::WriteWhenDisabled,  // Start slope
    ParamAccess::WriteWhenDisabled,  // Acceleration final slope
    ParamAccess::WriteWhenDisabled,  // Deceleration final slope
    ParamAccess::WriteWhenever,      // Thermal compensation
    ParamAccess::ReadOnly,           // ADC output
    ParamAccess::WriteWhenever,      // OCD Threshold
    ParamAccess::WriteWhenever,      // Stall Threshold
    ParamAccess::WriteWhenever,      // Full step speed
    ParamAccess::WriteWhenDisabled,  // Step mode
    ParamAccess::WriteWhenStopped,   // Alarm enable
    ParamAccess::WriteWhenDisabled,  // Gate config 1 (IC config for L6470)
    ParamAccess::WriteWhenDisabled,  // Gate config 2 (Status for L6470, ReadOnly)
    ParamAccess::WriteWhenDisabled,  // IC config (POWERSTEP01 only)
    ParamAccess::ReadOnly,           // Status (POWERSTEP01 only)
};

TestStepper::TestStepper(float low_stop, float high_stop, bool power_step)
    : low_stop_(low_stop),
      high_stop_(high_stop),
      power_step_(power_step),
      status_address_(power_step ? 27 : 25) {
  ResetDevice();
};

void TestStepper::ResetDevice() {
  status_.word = 0x7C03;
  // init values based on respective datasheet:
  // - table 12 (p50) for powerSTEP01
  // - table 9 (p40) for L6470
  size_t i = 0;
  while (i < 5) params_[i++] = 0;                 // 0 through 4
  while (i < 7) params_[i++] = 0x8A;              // 5 and 6
  params_[i++] = 0x41;                            // 7
  params_[i++] = 0;                               // 8
  while (i < 13) params_[i++] = 0x29;             // 9 through 12
  params_[i++] = 0x408;                           // 13
  params_[i++] = 0x19;                            // 14
  while (i < 17) params_[i++] = 0x29;             // 15 and 16
  params_[i++] = 0;                               // 17
  params_[i++] = 0;                               // 18
  params_[i++] = 0x8;                             // 19
  params_[i++] = power_step_ ? 0x10 : 0x40;       // 20
  params_[i++] = 0x27;                            // 21
  params_[i++] = 0x7;                             // 22
  params_[i++] = 0xFF;                            // 23
  params_[i++] = power_step_ ? 0 : 0x2E88;        // 24
  params_[i++] = power_step_ ? 0 : status_.word;  // 25
  params_[i++] = power_step_ ? 0x2C88 : 0;        // 26
  params_[i++] = power_step_ ? status_.word : 0;  // 27
}

void TestStepper::ReceiveByte(uint8_t byte) {
  if (command_index_ >= command_length_) {
    // This is a new command
    received_command_[0] = byte;
    command_index_ = 1;
    // Set default command length, may be modified in following switch
    command_length_ = 1;

    // Op Code for set param is 3-bits code "000" followed by the 5-bits param address.
    // Meaning that if the first 3 bits are 000, we expect ParamLength[address] more bytes.
    if ((byte & 0xE0) == 0) {
      command_length_ = 1 + ParamLength[byte & 0x1F];
    }
    // Op Code for get param is 3-bits code "001" followed by the 5-bits param address.
    // Requires a response, but we should not receive more (non-Nop) bytes
    if ((byte & 0xE0) == 0x20) {
      response_count_ = 0;
      response_index_ = 0;
    }

    // Treat other op codes
    switch (byte) {
      // following commands take a 3 bytes argument:
      case static_cast<uint8_t>(OpCode::RunNegative):
      case static_cast<uint8_t>(OpCode::RunPositive):
      case static_cast<uint8_t>(OpCode::MoveNegative):
      case static_cast<uint8_t>(OpCode::MovePositive):
      case static_cast<uint8_t>(OpCode::GoTo):
      case static_cast<uint8_t>(OpCode::GoToNegative):
      case static_cast<uint8_t>(OpCode::GoToPositive):
        command_length_ = 4;  // expect 3 more bytes
        break;
      // commands that require a response
      case static_cast<uint8_t>(OpCode::GetStatus):
        response_count_ = 0;
        response_index_ = 0;
        break;
    }
  } else {
    received_command_[command_index_++] = byte;
  }

  if (command_index_ >= command_length_) {
    // We have the full command, execute it.
    ExecuteCommand();
  }
};

uint8_t TestStepper::GetNextReponseByte() {
  if (response_index_ < response_count_) {
    return response_[response_index_++];
  } else {
    return received_command_[0];
  }
}

void TestStepper::ExecuteCommand() {
  uint8_t op_code = received_command_[0];
  // argument was received MSB first.
  uint32_t argument{0};
  if (op_code == static_cast<uint8_t>(OpCode::GoTo) ||
      op_code == static_cast<uint8_t>(OpCode::GoToPositive) ||
      op_code == static_cast<uint8_t>(OpCode::GoToNegative) ||
      op_code ==
          static_cast<uint8_t>(Param::AbsolutePosition) ||  // OpCode to set the Absolute position
      op_code == static_cast<uint8_t>(Param::MarkPosition)  // OpCode to set the Mark position
  ) {
    // Those commands use two's complement, check the sign bit
    if (received_command_[1] & 0x80) {
      // Make sure the high bits are completed with FF to keep the argument negative
      // when put into an int32_t
      argument = 0xFFFFFFFF;
    }
  }
  for (size_t byte = 1; byte < command_length_; byte++) {
    argument = (argument << 8) + received_command_[byte];
  }
  switch (op_code) {
    case static_cast<uint8_t>(OpCode::MoveNegative):
      MoveToPosition(static_cast<int32_t>(position_ * MicroStepsPerDeg()) - argument,
                     MoveDirection::Negative);
      break;
    case static_cast<uint8_t>(OpCode::MovePositive):
      MoveToPosition(static_cast<int32_t>(position_ * MicroStepsPerDeg()) + argument,
                     MoveDirection::Positive);
      break;
    case static_cast<uint8_t>(OpCode::GoTo):
      MoveToPosition(argument, MoveDirection::Shortest);
      break;
    case static_cast<uint8_t>(OpCode::GoToNegative):
      MoveToPosition(argument, MoveDirection::Negative);
      break;
    case static_cast<uint8_t>(OpCode::GoToPositive):
      MoveToPosition(argument, MoveDirection::Positive);
      break;
    case static_cast<uint8_t>(OpCode::ResetPosition):
      high_stop_ -= position_;
      low_stop_ -= position_;
      position_ = 0;
      break;
    case static_cast<uint8_t>(OpCode::Home):
      MoveToPosition(0, MoveDirection::Shortest);
      break;
    case static_cast<uint8_t>(OpCode::ResetDevice):
      ResetDevice();
      break;
    case static_cast<uint8_t>(OpCode::SoftStop):
    case static_cast<uint8_t>(OpCode::HardStop):
      status_.bitfield.stopped = 1;
      EnableDevice();
      break;
    case static_cast<uint8_t>(OpCode::SoftDisable):
    case static_cast<uint8_t>(OpCode::HardDisable):
      status_.bitfield.stopped = 1;
      DisableDevice();
      break;
    case static_cast<uint8_t>(OpCode::GetStatus):
      GetParam(status_address_);
      break;
    // Other valid OpCodes we don't treat in this test class
    case static_cast<uint8_t>(OpCode::Nop):
    case static_cast<uint8_t>(OpCode::RunNegative):
    case static_cast<uint8_t>(OpCode::RunPositive):
    case static_cast<uint8_t>(OpCode::StepClockNegative):
    case static_cast<uint8_t>(OpCode::StepClockPositive):
    default:
      // Op Code for set param is 3-bits code "000" followed by the 5-bits param address.
      // Meaning that if the first 3 bits are 000, we set the desired param value.
      if ((op_code & 0xE0) == 0) {
        SetParamIfAccessible(op_code & 0x1F, argument);
      } else
          // Op Code for get param is 3-bits code "001" followed by the 5-bits param address.
          // Meaning that if the first 3 bits are 001, or if the command is GetStatus, we need to
          // prepare a response.
          if ((op_code & 0xE0) == 0x20) {
        GetParam(op_code & 0x1F);
      } else {
        // Unknown OpCode. Log a command error and return
        status_.bitfield.invalid_command = 1;
        return;
      }
  }
}

void TestStepper::SetParamIfAccessible(size_t address, uint32_t value) {
  if (address >= 28) {
    // no such field
    status_.bitfield.invalid_command = 1;
    return;
  }
  if ((!power_step_ && address >= 25) ||  // fields only settable on PowerSTEP01
      (ParamsAccess[address] == ParamAccess::ReadOnly) ||
      (ParamsAccess[address] == ParamAccess::WriteWhenStopped && !status_.bitfield.stopped) ||
      (ParamsAccess[address] == ParamAccess::WriteWhenDisabled && !status_.bitfield.disabled)) {
    // reject the command
    status_.bitfield.rejected_command = 1;
    return;
  }
  params_[address] = value;
}

void TestStepper::GetParam(size_t address) {
  if (address < 0 || address >= (power_step_ ? 28 : 26)) {
    // No such field
    status_.bitfield.invalid_command = 1;
    return;
  }
  response_count_ = ParamLength[address];
  uint32_t value = params_[address];

  // Treat status separately, as the status is not stored in params_
  // also, reading status resets latching statuses to their init value
  if (address == (status_address_)) {
    value = status_.word;
    status_.bitfield.switch_event = 0;
    status_.bitfield.rejected_command = 0;
    status_.bitfield.invalid_command = 0;
    status_.bitfield.undervoltage = 1;
    status_.bitfield.thermal_warning = 1;
    status_.bitfield.thermal_shutdown = 1;
    status_.bitfield.overcurrent = 1;
    status_.bitfield.step_loss = 3;
  }

  // Queue each byte in response, MSB first
  value <<= 8 * (sizeof(response_) - response_count_);
  for (size_t i = 0; i < response_count_; i++) {
    response_[i] = (value >> 16) & 0xFF;
    value <<= 8;
  }
}

void TestStepper::MoveToPosition(int32_t microsteps, MoveDirection dir) {
  float new_position = static_cast<float>(microsteps) / MicroStepsPerDeg();
  if (dir == MoveDirection::Shortest) {
    // Determine the smallest angle between position_ and new_position
    float diff = std::fmod(new_position - position_ + 180.0f, 360.0f) - 180.0f;
    // Move by that amount, in the desired direction.
    MoveToPosition(static_cast<int32_t>((position_ + diff) * MicroStepsPerDeg()),
                   diff > 0 ? MoveDirection::Positive : MoveDirection::Negative);
    return;
  }

  // stepper can only move when stopped.
  if (!status_.bitfield.stopped) {
    status_.bitfield.rejected_command = 1;
    return;
  }
  // We don't simulate actual movement between positions, but we still assume the motor is not
  // stopped when it has just received a move command (with a different position than current one).
  if (fabsf(new_position - position_) > FLT_EPSILON) {
    status_.bitfield.stopped = 0;
  }
  switch (dir) {
    case MoveDirection::Positive:
      status_.bitfield.motion_direction = 0;
      if (new_position >= high_stop_ || new_position < position_) {
        position_ = high_stop_;
      } else {
        position_ = new_position;
      }
      break;
    case MoveDirection::Negative:
      status_.bitfield.motion_direction = 1;
      if (new_position <= low_stop_ || new_position > position_) {
        position_ = low_stop_;
      } else {
        position_ = new_position;
      }
      break;
    default:
      __builtin_unreachable();
  }
  params_[static_cast<size_t>(Param::AbsolutePosition)] =
      static_cast<int32_t>(position_ * MicroStepsPerDeg());
}

float TestStepper::MicroStepsPerDeg() {
  // Steppers have 200 steps per revolution.  Last three bytes of StepMode parameter determine
  // the number of microsteps per step (in powers of 2).
  return 200.f / 360.f *
         static_cast<float>(1 << (params_[static_cast<size_t>(Param::StepMode)] & 0x7));
}

}  // namespace StepMotor
