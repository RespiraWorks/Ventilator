/* Copyright 2020-2022, RespiraWorks

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

#include "stepper.h"

#include <cmath>
#include <cstring>

#include "clocks_stm32.h"
#include "interrupts.h"
#include "system_timer.h"

// See header file for details on the implementation philosophy.
// This module supports L6470 and powerSTEP01 stepper drivers from ST Microelectronics.
// Datasheets for these chips are:
//   L6470
//     https://www.st.com/content/st_com/en/products/motor-drivers/stepper-motor-drivers/l6470.html
//   powerSTEP01
//     https://www.st.com/content/st_com/en/products/motor-drivers/stepper-motor-drivers/powerstep01.html

namespace StepMotor {

// These constants are used to convert speeds in steps/sec to the weird values
// used by the stepper driver chip.
// Note that different registers use different conversion factors.
// See the chip data sheet for details.
static constexpr float TickTime = 250e-9f;
static constexpr float VelCurrentSpeedReg = TickTime * (1 << 28);
static constexpr float VelMaxSpeedReg = TickTime * (1 << 18);
static constexpr float VelMinSpeedReg = TickTime * (1 << 24);
static constexpr float VelFSSpeedReg = TickTime * (1 << 18);
static constexpr float VelIntSpeedReg = TickTime * (1 << 26);

// The number of microsteps / full step.
// For now this is a constant, we're just using the default value of the chip.
static constexpr uint32_t MicrostepPerStep = 128;

ErrorCode Handler::SetParam(Param param, uint32_t value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(ParamLength)) {
    return ErrorCode::BadParam;
  }

  size_t length = ParamLength[p];
  if (!length || (length > 3)) {
    return ErrorCode::BadParam;
  }

  uint8_t command_buffer[4];
  command_buffer[0] = p;  // The op-code for a set is just the parameter number

  // Split the parameter value into bytes, MSB first
  value <<= 8 * (3 - length);
  command_buffer[1] = static_cast<uint8_t>(value >> 16);
  command_buffer[2] = static_cast<uint8_t>(value >> 8);
  command_buffer[3] = static_cast<uint8_t>(value);

  // The op-code for a set is equal to the parameter number
  return SendCmd(command_buffer, length + 1);
}

ErrorCode Handler::GetParam(Param param, uint32_t *value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(ParamLength)) {
    return ErrorCode::BadParam;
  }

  size_t length = ParamLength[p];
  if (!length || (length > 3)) {
    return ErrorCode::BadParam;
  }

  // It's not legal to call this from the high priority task because it has to block.
  // Return an error if we're in an interrupt handler
  if (Interrupts::singleton().InInterruptHandler()) {
    return ErrorCode::WouldBlock;
  }

  uint8_t command_buffer[4];
  uint8_t response[sizeof(command_buffer) - 1] = {0};

  // For a get, the op-code is the parameter | 0x20
  command_buffer[0] = static_cast<uint8_t>(p | 0x20);
  command_buffer[1] = 0;
  command_buffer[2] = 0;
  command_buffer[3] = 0;

  ErrorCode error = SendCmd(command_buffer, length + 1, response);
  if (error != ErrorCode::Ok) {
    return error;
  }

  // Returned data is stored MSB first.
  *value = 0;
  for (size_t i = 0; i < length; i++) {
    *value <<= 8;
    *value |= response[i];
  }

  return error;
}

void Handler::Initialize() {
  // Do some basic init of the stepper motor chips so we can make them spin the motors
  Reset();

  // We need to delay briefly after reset before sending a new command.
  // For the power-step chip this delay time is specified as 500 microseconds in the data sheet.
  // For the L6470 it's only 45 max
  SystemTimer::singleton().delay(microseconds(500));

  // Get the first gate config register of the powerSTEP01.
  // This is actually the config register on the L6470
  uint32_t val{0};
  GetParam(Param::GateConfig1, &val);

  // If this is at the default config register value for the L6470 then I don't need to do any
  // more configuration
  if (val == 0x2E88) {
    return;
  }

  power_step_ = true;

  // Configure the two gate config registers to reasonable values
  //
  // GateConfig1 xxxxxxxxxxxxxxxx
  //           ...........\\\\\ - time at constant current 3750ns (0x1D)
  //           ........\\\------- Gate current 96mA (7)
  //           .....\\\---------- Turn off boost time 1uS (7)
  //           ....\------------- Watch dog enable (1)
  //           \\\\-------------- reserved
  SetParam(Param::GateConfig1, 0x0FFD);

  // GateConfig2 xxxxxxxx
  //           ...\\\\\---------- Dead time 1000ns (7)
  //           \\\--------------- Blanking time 1000ns (7)
  SetParam(Param::GateConfig2, 0xF7);
}

// Convert a velocity from Deg/sec units to the value to program
// into one of the stepper controller registers
float Handler::DegreesPerSecondToRegisterValue(float speed, float conversion_factor) const {
  // Convert to steps / sec
  float step_per_sec = speed * static_cast<float>(steps_per_rev_) / 360.0f;

  // Multiply that by the register specific conversion factor
  return conversion_factor * step_per_sec;
}

// Convert a velocity from a register value to deg/sec
float Handler::RegisterValueToDegreesPerSecond(int32_t value, float conversion_factor) const {
  return static_cast<float>(value) * 360.0f /
         (conversion_factor * static_cast<float>(steps_per_rev_));
}

// Read the current absolute motor velocity and return it in deg/sec units
// Note that this value is always positive
ErrorCode Handler::GetCurrentSpeed(float *return_value) {
  uint32_t register_value;
  ErrorCode error = GetParam(Param::Speed, &register_value);
  *return_value = RegisterValueToDegreesPerSecond(register_value, VelCurrentSpeedReg);
  return error;
}

// Set the motor's max speed setting in deg/sec
//
// NOTE - The motor must be disabled to set this
ErrorCode Handler::SetMaxSpeed(float dps) {
  if (dps < 0) {
    return ErrorCode::BadValue;
  }

  uint32_t speed = static_cast<uint32_t>(DegreesPerSecondToRegisterValue(dps, VelMaxSpeedReg));
  if (speed > 0x3ff) {
    speed = 0x3ff;
  }

  return SetParam(Param::MaxSpeed, speed);
}

// Get the motor's max speed setting in deg/sec
ErrorCode Handler::GetMaxSpeed(float *return_value) {
  uint32_t register_value;
  ErrorCode error = GetParam(Param::MaxSpeed, &register_value);
  *return_value = RegisterValueToDegreesPerSecond(register_value, VelMaxSpeedReg);
  return error;
}

// Set the motor's min speed setting in deg/sec
//
// NOTE - Setting a non-zero minimum speed doesn't mean
// the motor can't stop, this is the minimum speed for
// a move.  When you start a move, rather than increase
// linearly from 0, the stepper will jump to this speed
// immediately, then ramp up from there.
// This can help with vibration.
//
// NOTE - The motor must be disabled to set this
ErrorCode Handler::SetMinSpeed(float dps) {
  if (dps < 0) {
    return ErrorCode::BadValue;
  }

  uint32_t speed = static_cast<uint32_t>(DegreesPerSecondToRegisterValue(dps, VelMinSpeedReg));
  if (speed > 0xfff) {
    speed = 0xfff;
  }

  return SetParam(Param::MinSpeed, speed);
}

// Get the motor's min speed setting in deg/sec
ErrorCode Handler::GetMinSpeed(float *return_value) {
  uint32_t register_value;
  ErrorCode error = GetParam(Param::MinSpeed, &register_value);
  *return_value = RegisterValueToDegreesPerSecond(register_value, VelMinSpeedReg);
  return error;
}

// Set the motors accel and decel rate in deg/sec/sec units
//
// NOTE - The motor must be disabled to set this
ErrorCode Handler::SetAccel(float acc) {
  static constexpr float Scaler = (TickTime * TickTime * 1099511627776.f);
  if (acc < 0) {
    return ErrorCode::BadValue;
  }

  // Convert from deg/sec/sec to steps/sec/sec
  acc *= static_cast<float>(steps_per_rev_) / 360.0f;

  // Convert to the proper units for the driver chip
  uint32_t val = static_cast<uint32_t>(acc * Scaler);
  if (val > 0x0fff) {
    val = 0xfff;
  }

  ErrorCode error = SetParam(Param::Acceleration, val);
  if (error != ErrorCode::Ok) {
    return error;
  }

  return SetParam(Param::Deceleration, val);
}

// Set the amplitude of the voltage output used to drive the motor.
// The values set here allow the amplitude of output that pushes power
// to the motor to be adjusted.  Higher values will push more current
// into the motor, lower values will push less.  The motor will be
// more powerful with higher values, but will get hotter, consume more
// power, and could potentially be damaged if these are set too high,
// so be careful.
//
// There are four different values that can be set which control the
// output to the motor in different phases of its motion:
//   hold - Value used when the motor is holding position (not moving)
//   run  - Value used when running at constant velocity.
//   accel - Value used when accelerating
//   decel - Value used when decelerating
//
// In all cases the values are set in a range of 0 to 1
// for 0 to 100%
ErrorCode Handler::SetKval(Param param, float amp) {
  // The powerSTEP chip seems a bit flaky with this setting.
  // It doesn't seem to enable at all with a 10% setting, and is
  // really wimpy with 20%.  For the moment I'm just boosting this
  // by an extra 15% if working with the powerSTEP chip.
  // I don't think we're using that part in the real design, so
  // this can get retired at some point
  if (power_step_ && amp < 0.9) {
    amp += 0.15f;
  }

  if ((amp < 0) || (amp > 1)) {
    return ErrorCode::BadValue;
  }

  // The stepper chip uses a value of 0 to 255
  return SetParam(param, static_cast<uint32_t>(amp * 255));
}

ErrorCode Handler::SetAmpHold(float amp) { return SetKval(Param::KValueHold, amp); }
ErrorCode Handler::SetAmpRun(float amp) { return SetKval(Param::KValueRun, amp); }
ErrorCode Handler::SetAmpAccel(float amp) { return SetKval(Param::KValueAccelerate, amp); }
ErrorCode Handler::SetAmpDecel(float amp) { return SetKval(Param::KValueDecelerate, amp); }

ErrorCode Handler::SetAmpAll(float amp) {
  ErrorCode error = SetAmpHold(amp);
  if (error != ErrorCode::Ok) {
    return error;
  }

  error = SetAmpRun(amp);
  if (error != ErrorCode::Ok) {
    return error;
  }

  error = SetAmpAccel(amp);
  if (error != ErrorCode::Ok) {
    return error;
  }

  return SetAmpDecel(amp);
}

// Start running at a constant velocity.
// The velocity is specified in deg/sec units
ErrorCode Handler::RunAtVelocity(float vel) {
  // Stepper uses two separate commands for RunPositive and RunNegative, both of which take an
  // unsigned speed argument: absolute value of the desired velocity, changed to register value.
  float speed = DegreesPerSecondToRegisterValue(fabsf(vel), VelCurrentSpeedReg);

  // The speed value is a 20 bit unsigned integer passed as part of the command.
  uint32_t s = static_cast<uint32_t>(speed);
  if (s > 0x000fffff) {
    s = 0x000fffff;
  }

  uint8_t command[4];
  if (vel < 0)
    command[0] = static_cast<uint8_t>(OpCode::RunNegative);
  else
    command[0] = static_cast<uint8_t>(OpCode::RunPositive);

  command[1] = static_cast<uint8_t>(s >> 16);
  command[2] = static_cast<uint8_t>(s >> 8);
  command[3] = static_cast<uint8_t>(s);
  return SendCmd(command, 4);
}

// Decelerate to zero velocity and hold position
// This can also be used to enable the motor without causing any motion
ErrorCode Handler::SoftStop() {
  uint8_t command = static_cast<uint8_t>(OpCode::SoftStop);
  return SendCmd(&command, 1);
}

// Stop abruptly and hold position
// This can also be used to enable the motor without causing any motion
ErrorCode Handler::HardStop() {
  uint8_t command = static_cast<uint8_t>(OpCode::HardStop);
  return SendCmd(&command, 1);
}

// Decelerate to zero velocity and disable
ErrorCode Handler::SoftDisable() {
  uint8_t command = static_cast<uint8_t>(OpCode::SoftDisable);
  return SendCmd(&command, 1);
}

// Immediately disable the motor
ErrorCode Handler::HardDisable() {
  uint8_t command = static_cast<uint8_t>(OpCode::HardDisable);
  return SendCmd(&command, 1);
}

// Reset the motor position to zero
ErrorCode Handler::ClearPosition() {
  uint8_t command = static_cast<uint8_t>(OpCode::ResetPosition);
  return SendCmd(&command, 1);
}

// Reset the stepper chip
ErrorCode Handler::Reset() {
  uint8_t command = static_cast<uint8_t>(OpCode::ResetDevice);
  return SendCmd(&command, 1);
}

ErrorCode Handler::GetStatus(Status *stat) {
  // It's not legal to call this from the control loop because it has to block.
  // Return an error if we're in an interrupt handler
  if (Interrupts::singleton().InInterruptHandler()) {
    return ErrorCode::WouldBlock;
  }

  // We expect a 2 bytes response so we add two Nops after the op code.
  uint8_t command[] = {static_cast<uint8_t>(OpCode::GetStatus), static_cast<uint8_t>(OpCode::Nop),
                       static_cast<uint8_t>(OpCode::Nop)};
  uint8_t response[sizeof(command) - 1] = {0};

  ErrorCode error = SendCmd(command, sizeof(command), response);
  if (error != ErrorCode::Ok) {
    return error;
  }

  // received MSB first
  stat->word = static_cast<uint16_t>((response[0] << 8) + response[1]);

  return error;
}

int32_t Handler::DegreesToMicrosteps(float deg) const {
  uint32_t ustep_per_rev = MicrostepPerStep * steps_per_rev_;

  float steps = static_cast<float>(ustep_per_rev) * deg / 360.0f;
  return static_cast<int32_t>(steps);
}

// Goto to the position (in deg) via the shortest path
// This returns once the move has started, it doesn't wait for the move to finish
ErrorCode Handler::GotoPos(float deg) {
  int32_t ustep = DegreesToMicrosteps(deg);

  uint8_t command[4];
  command[0] = static_cast<uint8_t>(OpCode::GoTo);
  command[1] = static_cast<uint8_t>(ustep >> 16);
  command[2] = static_cast<uint8_t>(ustep >> 8);
  command[3] = static_cast<uint8_t>(ustep);
  return SendCmd(command, 4);
}

// Start a relative move of the passed number of deg.
ErrorCode Handler::MoveRel(float deg) {
  int32_t dist = DegreesToMicrosteps(deg);

  uint8_t command[4];

  if (dist < 0) {
    command[0] = static_cast<uint8_t>(OpCode::MoveNegative);
    dist *= -1;
  } else
    command[0] = static_cast<uint8_t>(OpCode::MovePositive);

  if (dist > 0x003FFFFF) {
    return ErrorCode::BadValue;
  }

  command[1] = static_cast<uint8_t>(dist >> 16);
  command[2] = static_cast<uint8_t>(dist >> 8);
  command[3] = static_cast<uint8_t>(dist);
  return SendCmd(command, 4);
}

// Send a command to the motor and, unless called from an interrupt handler, wait for it to be
// processed.
// The command should be at least length bytes long and response (if any) should be at least
// (length-1) bytes long.
//
// Each command to the stepper consists of a one byte command code and 0 to 3 data bytes.
// Multiple commands can be placed one after another and all sent as a unit with this function.
//
// Note that when called outside an interrupt handler, a busy wait is included.
ErrorCode Handler::SendCmd(uint8_t *command, size_t length, uint8_t *response) {
  if (!stepper_chain_) {
    // This is coming from an error in setting up the motor. TODO: fault somehow?
    return ErrorCode::InvalidState;
  }
  bool request_finished = false;
  SPI::Request request = {
      .command = command,
      .length = length,
      .response = response,
      .processed = &request_finished,
  };
  if (!stepper_chain_->SendRequest(request, slave_index_)) {
    return ErrorCode::QueueFull;
  }

  while (!Interrupts::singleton().InInterruptHandler() && !request_finished) {
  }

  return ErrorCode::Ok;
}

}  // namespace StepMotor
