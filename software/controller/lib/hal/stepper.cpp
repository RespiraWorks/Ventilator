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

#if defined(BARE_STM32)

#include <cmath>
#include <cstring>

#include "clocks_stm32.h"
#include "interrupts.h"
#include "system_timer.h"

// This array holds the length of each parameter in units of
// bytes, rounded up to the nearest byte.  This info is based
// on table 12 in the powerSTEP chip data sheet
uint8_t StepMotor::param_len_[32] = {
    0,  // 0x00 - No valid parameter with ID 0
    3,  // 0x01 - Absolute position (22 bits)
    2,  // 0x02 - Electrical position (9 bits)
    3,  // 0x03 - Mark position (22 bits)
    3,  // 0x04 - Current speed (20 bits)
    2,  // 0x05 - Acceleration (12 bits)
    2,  // 0x06 - Deceleration (12 bits)
    2,  // 0x07 - Maximum speed (10 bits)
    2,  // 0x08 - Minimum speed (12 bits)
    1,  // 0x09 - KValueHold Holding K VAL (8 bits)
    1,  // 0x0A - KValueRun Constant speed K VAL (8 bits)
    1,  // 0x0B - KVAL_ACC Acceleration starting K VAL (8 bits)
    1,  // 0x0C - KVAL_DEC Deceleration starting K VAL (8 bits)
    2,  // 0x0D - IntersectSpeed Intersect speed (14 bits)
    1,  // 0x0E - ST_SLP Start slope (8 bits)
    1,  // 0x0F - FN_SLP_ACC Acceleration final slope (8 bits)
    1,  // 0x10 - FN_SLP_DEC Deceleration final slope (8 bits)
    1,  // 0x11 - K_THERM Thermal compensation factor (4 bits)
    1,  // 0x12 - ADC output (5 bits)
    1,  // 0x13 - OCD threshold (5 bits)
    1,  // 0x14 - STALL_TH STALL threshold (5 bits)
    2,  // 0x15 - Full-step speed (11 bits)
    1,  // 0x16 - Step mode (8 bits)
    1,  // 0x17 - Alarm enables (8 bits)
    2,  // 0x18 - Gate driver configuration (11 bits)
    1,  // 0x19 - Gate driver configuration (8 bits)
    2,  // 0x1A - IC configuration (16 bits)
    2,  // 0x1B - Status (16 bits)
    0,  // 0x1C - No such parameter
    0,  // 0x1D - No such parameter
    0,  // 0x1E - No such parameter
    0,  // 0x1F - No such parameter
};

// These constants are used to convert speeds in steps/sec
// to the weird values used by the stepper driver chip.
// Note that different registers use different conversion
// factors.  See the chip data sheet for details.
static constexpr float TickTime = 250e-9f;
static constexpr float VelCurrentSpeedReg = TickTime * (1 << 28);
static constexpr float VelMaxSpeedReg = TickTime * (1 << 18);
static constexpr float VelMinSpeedReg = TickTime * (1 << 24);
static constexpr float VelFSSpeedReg = TickTime * (1 << 18);
static constexpr float VelIntSpeedReg = TickTime * (1 << 26);

// The number of microsteps / full step.
// For now this is a constant, we're just using the
// default value of the chip.
static constexpr uint32_t MicrostepPerStep = 128;

StepMtrErr StepMotor::SetParam(StepMtrParam param, uint32_t value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(param_len_)) {
    return StepMtrErr::BadParam;
  }

  size_t length = param_len_[p];
  if (!length || (length > 3)) {
    return StepMtrErr::BadParam;
  }

  uint8_t command_buff[4];
  command_buff[0] = p;  // The op-code for a set is just the parameter number

  // Split the parameter value into bytes, MSB first
  value <<= 8 * (3 - length);
  command_buff[1] = static_cast<uint8_t>(value >> 16);
  command_buff[2] = static_cast<uint8_t>(value >> 8);
  command_buff[3] = static_cast<uint8_t>(value);

  // The op-code for a set is equal to the parameter number
  return SendCmd(command_buff, length + 1);
}

StepMtrErr StepMotor::GetParam(StepMtrParam param, uint32_t *value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(param_len_)) {
    return StepMtrErr::BadParam;
  }

  size_t length = param_len_[p];
  if (!length || (length > 3)) {
    return StepMtrErr::BadParam;
  }

  // It's not legal to call this from the control loop because it
  // has to block.  Return an error if we're in an interrupt handler
  if (Interrupts::singleton().InInterruptHandler()) {
    return StepMtrErr::WouldBlock;
  }

  uint8_t command_buff[4];
  uint8_t response[sizeof(command_buff) - 1] = {0};

  // For a get, the op-code is the parameter | 0x20
  command_buff[0] = static_cast<uint8_t>(p | 0x20);
  command_buff[1] = 0;
  command_buff[2] = 0;
  command_buff[3] = 0;

  StepMtrErr err = SendCmd(command_buff, length + 1, response);
  if (err != StepMtrErr::Ok) {
    return err;
  }

  // Returned data is stored MSB first.
  *value = 0;
  for (size_t i = 0; i < length; i++) {
    *value <<= 8;
    *value |= response[i];
  }

  return err;
}

void StepMotor::Initialize() {
  // Do some basic init of the stepper motor chips so we can make them spin the motors
  Reset();

  // We need to delay briefly after reset before sending a new command.
  // For the power-step chip this delay time is specified as 500 microseconds in the data sheet.
  // For the L6470 it's only 45 max
  SystemTimer::singleton().delay(microseconds(500));

  // Get the first gate config register of the powerSTEP01.
  // This is actually the config register on the L6470
  uint32_t val{0};
  GetParam(StepMtrParam::GateConfig1, &val);

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
  SetParam(StepMtrParam::GateConfig1, 0x0FFD);

  // GateConfig2 xxxxxxxx
  //           ...\\\\\---------- Dead time 1000ns (7)
  //           \\\--------------- Blanking time 1000ns (7)
  SetParam(StepMtrParam::GateConfig2, 0xF7);
}

// Convert a velocity from Deg/sec units to the value to program
// into one of the stepper controller registers
float StepMotor::DpsToVelReg(float vel, float cnv) const {
  // Convert to steps / sec
  float step_per_sec = vel * static_cast<float>(steps_per_rev_) / 360.0f;

  // Multiply that by the register specific conversion factor
  return cnv * step_per_sec;
}

// Convert a velocity from a register value to deg/sec
float StepMotor::RegVelToDps(int32_t val, float cnv) const {
  return static_cast<float>(val) * 360.0f / (cnv * static_cast<float>(steps_per_rev_));
}

// Read the current absolute motor velocity and return it
// in deg/sec units
// Note that this value is always positive
StepMtrErr StepMotor::GetCurrentSpeed(float *ret) {
  uint32_t val;
  StepMtrErr err = GetParam(StepMtrParam::Speed, &val);
  *ret = RegVelToDps(val, VelCurrentSpeedReg);
  return err;
}

// Set the motor's max speed setting in deg/sec
//
// NOTE - The motor must be disabled to set this
StepMtrErr StepMotor::SetMaxSpeed(float dps) {
  if (dps < 0) {
    return StepMtrErr::BadValue;
  }

  uint32_t speed = static_cast<uint32_t>(DpsToVelReg(dps, VelMaxSpeedReg));
  if (speed > 0x3ff) {
    speed = 0x3ff;
  }

  return SetParam(StepMtrParam::MaxSpeed, speed);
}

// Get the motor's max speed setting in deg/sec
StepMtrErr StepMotor::GetMaxSpeed(float *ret) {
  uint32_t val;
  StepMtrErr err = GetParam(StepMtrParam::MaxSpeed, &val);
  *ret = RegVelToDps(val, VelMaxSpeedReg);
  return err;
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
StepMtrErr StepMotor::SetMinSpeed(float dps) {
  if (dps < 0) {
    return StepMtrErr::BadValue;
  }

  uint32_t speed = static_cast<uint32_t>(DpsToVelReg(dps, VelMinSpeedReg));
  if (speed > 0xfff) {
    speed = 0xfff;
  }

  return SetParam(StepMtrParam::MinSpeed, speed);
}

// Get the motor's min speed setting in deg/sec
StepMtrErr StepMotor::GetMinSpeed(float *ret) {
  uint32_t val;
  StepMtrErr err = GetParam(StepMtrParam::MinSpeed, &val);
  *ret = RegVelToDps(val, VelMinSpeedReg);
  return err;
}

// Set the motors accel and decel rate in deg/sec/sec units
//
// NOTE - The motor must be disabled to set this
StepMtrErr StepMotor::SetAccel(float acc) {
  static constexpr float Scaler = (TickTime * TickTime * 1099511627776.f);
  if (acc < 0) {
    return StepMtrErr::BadValue;
  }

  // Convert from deg/sec/sec to steps/sec/sec
  acc *= static_cast<float>(steps_per_rev_) / 360.0f;

  // Convert to the proper units for the driver chip
  uint32_t val = static_cast<uint32_t>(acc * Scaler);
  if (val > 0x0fff) {
    val = 0xfff;
  }

  StepMtrErr err = SetParam(StepMtrParam::Acceleration, val);
  if (err != StepMtrErr::Ok) {
    return err;
  }

  return SetParam(StepMtrParam::Deceleration, val);
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
StepMtrErr StepMotor::SetKval(StepMtrParam param, float amp) {
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
    return StepMtrErr::BadValue;
  }

  // The stepper chip uses a value of 0 to 255
  return SetParam(param, static_cast<uint32_t>(amp * 255));
}

StepMtrErr StepMotor::SetAmpHold(float amp) { return SetKval(StepMtrParam::KValueHold, amp); }
StepMtrErr StepMotor::SetAmpRun(float amp) { return SetKval(StepMtrParam::KValueRun, amp); }
StepMtrErr StepMotor::SetAmpAccel(float amp) {
  return SetKval(StepMtrParam::KValueAccelerate, amp);
}
StepMtrErr StepMotor::SetAmpDecel(float amp) {
  return SetKval(StepMtrParam::KValueDecelerate, amp);
}

StepMtrErr StepMotor::SetAmpAll(float amp) {
  StepMtrErr err = SetAmpHold(amp);
  if (err != StepMtrErr::Ok) {
    return err;
  }

  err = SetAmpRun(amp);
  if (err != StepMtrErr::Ok) {
    return err;
  }

  err = SetAmpAccel(amp);
  if (err != StepMtrErr::Ok) {
    return err;
  }

  return SetAmpDecel(amp);
}

// Start running at a constant velocity.
// The velocity is specified in deg/sec units
StepMtrErr StepMotor::RunAtVelocity(float vel) {
  // Convert the speed from deg/sec to the weird units used by the stepper chip
  float speed = DpsToVelReg(fabsf(vel), VelCurrentSpeedReg);

  // The speed value is a 20 bit unsigned integer passed as part of the command.
  uint32_t s = static_cast<uint32_t>(speed);
  if (s > 0x000fffff) {
    s = 0x000fffff;
  }

  uint8_t command[4];
  if (vel < 0)
    command[0] = static_cast<uint8_t>(StepMtrCmd::RunNegative);
  else
    command[0] = static_cast<uint8_t>(StepMtrCmd::RunPositive);

  command[1] = static_cast<uint8_t>(s >> 16);
  command[2] = static_cast<uint8_t>(s >> 8);
  command[3] = static_cast<uint8_t>(s);
  return SendCmd(command, 4);
}

// Decelerate to zero velocity and hold position
// This can also be used to enable the motor without causing any motion
StepMtrErr StepMotor::SoftStop() {
  uint8_t command = static_cast<uint8_t>(StepMtrCmd::SoftStop);
  return SendCmd(&command, 1);
}

// Stop abruptly and hold position
// This can also be used to enable the motor without causing any motion
StepMtrErr StepMotor::HardStop() {
  uint8_t command = static_cast<uint8_t>(StepMtrCmd::HardStop);
  return SendCmd(&command, 1);
}

// Decelerate to zero velocity and disable
StepMtrErr StepMotor::SoftDisable() {
  uint8_t command = static_cast<uint8_t>(StepMtrCmd::SoftDisable);
  return SendCmd(&command, 1);
}

// Immediately disable the motor
StepMtrErr StepMotor::HardDisable() {
  uint8_t command = static_cast<uint8_t>(StepMtrCmd::HardDisable);
  return SendCmd(&command, 1);
}

// Reset the motor position to zero
StepMtrErr StepMotor::ClearPosition() {
  uint8_t command = static_cast<uint8_t>(StepMtrCmd::ResetPosition);
  return SendCmd(&command, 1);
}

// Reset the stepper chip
StepMtrErr StepMotor::Reset() {
  uint8_t command = static_cast<uint8_t>(StepMtrCmd::ResetDevice);
  return SendCmd(&command, 1);
}

StepMtrErr StepMotor::GetStatus(StepperStatus *stat) {
  // It's not legal to call this from the control loop because it
  // has to block.  Return an error if we're in an interrupt handler
  if (Interrupts::singleton().InInterruptHandler()) {
    return StepMtrErr::WouldBlock;
  }

  // We expect a 2 bytes response so we add two Nops after the op code.
  uint8_t command[] = {static_cast<uint8_t>(StepMtrCmd::GetStatus),
                   static_cast<uint8_t>(StepMtrCmd::Nop), static_cast<uint8_t>(StepMtrCmd::Nop)};
  uint8_t response[sizeof(command) - 1] = {0};

  StepMtrErr err = SendCmd(command, sizeof(command), response);
  if (err != StepMtrErr::Ok) {
    return err;
  }

  stat->enabled = (response[1] & 0x01) == 0;
  stat->command_error = (response[1] & 0x80) || (response[0] & 0x01);
  stat->under_voltage = (response[0] & 0x02) == 0;
  stat->thermal_warning = (response[0] & 0x04) == 0;
  stat->thermal_shutdown = (response[0] & 0x08) == 0;
  stat->over_current = (response[0] & 0x10) == 0;
  stat->step_loss = (response[0] & 0x60) != 0x60;

  switch (response[1] & 0x60) {
    case 0x00:
      stat->move_status = StepMoveStatus::Stopped;
      break;
    case 0x20:
      stat->move_status = StepMoveStatus::Accelerating;
      break;
    case 0x40:
      stat->move_status = StepMoveStatus::Decelerating;
      break;
    case 0x60:
      stat->move_status = StepMoveStatus::ConstantSpeed;
      break;
  }

  return err;
}

int32_t StepMotor::DegToUstep(float deg) const {
  uint32_t ustep_per_rev = MicrostepPerStep * steps_per_rev_;

  float steps = static_cast<float>(ustep_per_rev) * deg / 360.0f;
  return static_cast<int32_t>(steps);
}

// Goto to the position (in deg) via the shortest path
// This returns once the move has started, it doesn't wait for the move to finish
StepMtrErr StepMotor::GotoPos(float deg) {
  int32_t ustep = DegToUstep(deg);

  uint8_t command[4];
  command[0] = static_cast<uint8_t>(StepMtrCmd::GoTo);
  command[1] = static_cast<uint8_t>(ustep >> 16);
  command[2] = static_cast<uint8_t>(ustep >> 8);
  command[3] = static_cast<uint8_t>(ustep);
  return SendCmd(command, 4);
}

// Start a relative move of the passed number of deg.
StepMtrErr StepMotor::MoveRel(float deg) {
  int32_t dist = DegToUstep(deg);

  uint8_t command[4];

  if (dist < 0) {
    command[0] = static_cast<uint8_t>(StepMtrCmd::MoveNegative);
    dist *= -1;
  } else
    command[0] = static_cast<uint8_t>(StepMtrCmd::MovePositive);

  if (dist > 0x003FFFFF) {
    return StepMtrErr::BadValue;
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
// Note that when called outside an interrupt handler, this waits until the command is processed
// before returning.
StepMtrErr StepMotor::SendCmd(uint8_t *command, uint32_t length, uint8_t *response) {
  bool request_finished = false;
  SPI::Request request = {
      .command = command,
      .length = length,
      .response = response,
      .processed = &request_finished,
  };
  if (!daisy_chain_->SendRequest(request, slave_index_)) {
    return StepMtrErr::QueueFull;
  }

  while (!Interrupts::singleton().InInterruptHandler() && !request_finished) {
  }

  return StepMtrErr::Ok;
}

#else

/// \TODO improve this mocking to be helpful in testing

StepMtrErr StepMotor::HardDisable() { return StepMtrErr::Ok; }
StepMtrErr StepMotor::SetAmpAll(float amp) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::SetMaxSpeed(float dps) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::SetAccel(float acc) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::MoveRel(float deg) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::ClearPosition() { return StepMtrErr::Ok; }
StepMtrErr StepMotor::GotoPos(float deg) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::HardStop() { return StepMtrErr::Ok; }

#endif
