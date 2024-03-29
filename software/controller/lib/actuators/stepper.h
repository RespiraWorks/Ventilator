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

///////////////////////////////////////////////////////////////////////////////
//
// This module implements a communication interface used to talk to a fixed
// number of stepper driver chips via an SPI synchronous serial port.
//
// The stepper driver chips are setup in a daisy chain configuration (see
// https://en.wikipedia.org/wiki/Serial_Peripheral_Interface#Daisy_chain_configuration)
//
// The driver chips supported are made by ST.  The following chips are supported:
//   L6470
//     https://www.st.com/content/st_com/en/products/motor-drivers/stepper-motor-drivers/l6470.html
//   powerSTEP01
//     https://www.st.com/content/st_com/en/products/motor-drivers/stepper-motor-drivers/powerstep01.html
//
// This interface is designed to allow it to be safely used by code running
// both in the main background loop of the controller as well as from the
// high priority control loop which itself runs from an ISR.
//
// The main background loop can call any method implemented in this module.
// The loop will block while the communication with the stepper chips takes
// place and resume when the communication has finished.
//
// The high priority control loop can only call a subset of the methods here,
// those which send commands to the stepper chips but don't return a value.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <optional>

#include "spi_chain.h"

namespace StepMotor {

// These are the simple op codes for the stepper driver.
// Not included here are set/get parameter which include the parameter ID as part of the code.
enum class OpCode : uint8_t {
  Nop = 0,                   // Used when there's no other command to send
  RunNegative = 0x50,        // Run negative at constant speed.
  RunPositive = 0x51,        // Run positive at constant speed.
  StepClockNegative = 0x58,  // Switch to step clock mode moving in negative direction
  StepClockPositive = 0x59,  // Switch to step clock mode moving in positive direction
  MoveNegative = 0x40,       // Move - steps, steps is given as a parameter
  MovePositive = 0x41,       // Move + steps, steps is given as a parameter
  GoTo = 0x60,               // Go to an absolute position using shortest path
  GoToNegative = 0x68,       // Go to an absolute position in negative direction
  GoToPositive = 0x69,       // Go to an absolute position in positive direction
  Home = 0x70,               // Same as GoTo 0
  ResetPosition = 0xD8,      // Set absolute position register to 0
  ResetDevice = 0xC0,        // Reset the stepper chip
  SoftStop = 0xB0,           // Perform a soft stop
  HardStop = 0xB8,           // Perform a hard stop
  SoftDisable = 0xA0,        // Soft stop, then disable
  HardDisable = 0xA8,        // Hard stop, then disable
  GetStatus = 0xD0,          // Read the 16-bit status word
};

enum class Param : uint8_t {
  AbsolutePosition = 0x01,          // Absolute position
  ElectricalPosition = 0x02,        // Electrical position
  MarkPosition = 0x03,              // Mark position
  Speed = 0x04,                     // Current speed
  Acceleration = 0x05,              // Acceleration
  Deceleration = 0x06,              // Deceleration
  MaxSpeed = 0x07,                  // Maximum allowed speed
  MinSpeed = 0x08,                  // Minimum allowed speed
  KValueHold = 0x09,                //
  KValueRun = 0x0A,                 //
  KValueAccelerate = 0x0B,          //
  KValueDecelerate = 0x0C,          //
  IntersectSpeed = 0x0D,            //
  StartSlope = 0x0E,                //
  FinalSlopeAcceleration = 0x0F,    //
  FinalSlopeDeceleration = 0x10,    //
  ThermalCompensation = 0x11,       //
  AdcOutput = 0x12,                 //
  OverrideCurrentThreshold = 0x13,  //
  StallThreshold = 0x14,            //
  FullStepSpeed = 0x15,             // Speed at which to switch to full step mode
  StepMode = 0x16,                  //
  AlarmEnable = 0x17,               //
  GateConfig1 = 0x18,               //
  GateConfig2 = 0x19,               //
  Config = 0x1A,                    //
  Status = 0x1B,                    //
};

// This array holds the length of each parameter in units of bytes, rounded up to the nearest byte.
// This info is based on table 12 in the powerSTEP chip data sheet.
// The Handler class normally abstracts these away, unless you want to use [Get|Set]Param directly.
static constexpr size_t ParamLength[32] = {
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

// Error codes returned by my functions
enum class ErrorCode {
  Ok,
  BadParam,      // The parameter ID is invalid
  BadValue,      // Illegal value passed
  WouldBlock,    // Call would block, can't be called from within ISR
  QueueFull,     // Can't add command to queue, not enough space
  InvalidState,  // Invalid state for command.
};

// Detailed status about the stepper driver chip.
// Note, latching fields will return true if the event has occurred since the last time
// the status was read.  The act of reading the status clears them so they won't be true
// on the next read unless they happen again.
union Status {
  struct {
    uint16_t disabled : 1;
    uint16_t stopped : 1;  // active low
    uint16_t switch_input : 1;
    uint16_t switch_event : 1;  // latching
    uint16_t motion_direction : 1;
    uint16_t motion_status : 2;
    uint16_t rejected_command : 1;  // latching
    uint16_t invalid_command : 1;   // latching
    uint16_t undervoltage : 1;      // latching, active low
    uint16_t thermal_warning : 1;   // latching, active low
    uint16_t thermal_shutdown : 1;  // latching, active low
    uint16_t overcurrent : 1;       // latching, active low
    uint16_t step_loss : 2;         // latching, active low
    uint16_t step_clock_mode : 1;
  } bitfield;
  uint16_t word;
};

// Possible values for Status::bitfield.motion_status
enum class MoveStatus : uint16_t {
  Stopped = 0,
  Accelerating = 1,
  Decelerating = 2,
  ConstantSpeed = 3,
};

using Chain = SPI::DaisyChain</*MaxSlaves=*/4, /*QueueLength*/ 10>;

// Represents one of the stepper motors in the system
class Handler {
 public:
  Handler(size_t index, Chain *stepper_chain)
      : slave_index_(index), stepper_chain_(stepper_chain){};

  void Initialize();

  // Sets the number of full steps / rev for the motor.  For most stepper motors this is 200.
  // That's the default if this function isn't called.
  // If your stepper is a '1.8 deg' motor, then it has 200 steps/rev (360 deg / 200 steps = 1.8
  // deg). This value is used internally to convert between degrees and steps.
  void SetStepsPerRev(uint32_t spr) { steps_per_rev_ = spr; }

  uint32_t GetStepsPerRev() const { return steps_per_rev_; }

  // Read the current absolute motor velocity and return it in deg/sec units
  // Note that this value is always positive
  ErrorCode GetCurrentSpeed(float *ret);

  // Get and set the motors max speed in deg/sec
  ErrorCode SetMaxSpeed(float dps);
  ErrorCode GetMaxSpeed(float *ret);

  // Get and set the motor's minimum speed setting in deg/sec
  //
  // NOTE - Setting a non-zero minimum speed doesn't mean the motor can't stop, this is the minimum
  // speed when it is actually moving.
  // When you start a move, rather then increase linearly from 0, the stepper will jump to this
  // speed immediately, then ramp up from there.
  // This can help with vibration.
  //
  // NOTE - The motor must be disabled to set this
  ErrorCode SetMinSpeed(float dps);
  ErrorCode GetMinSpeed(float *ret);

  // Set the motors accel and decel rate in deg/sec/sec units
  //
  // NOTE - The motor must be disabled to set this
  ErrorCode SetAccel(float acc);

  // Set the amplitude of the voltage output used to drive the motor.
  // The values set here adjust the amplitude of output that pushes power to the motor.
  // Higher values will push more current into the motor, lower values will push less.
  // The motor will be more powerful with higher values, but will get hotter, consume more
  // power, and could potentially be damaged if these are set too high, so be careful.
  //
  // There are four different values that can be set which control the
  // output to the motor in different phases of its motion:
  //   hold - Value used when the motor is holding position (not moving).
  //   run  - Value used when running at constant velocity.
  //   accel - Value used when accelerating.
  //   decel - Value used when decelerating.
  //
  // In all cases the values are set in a range of 0 to 1, for 0 to 100%.
  ErrorCode SetAmpHold(float amp);
  ErrorCode SetAmpRun(float amp);
  ErrorCode SetAmpAccel(float amp);
  ErrorCode SetAmpDecel(float amp);

  // Sets all four amplitude values to the same value.
  ErrorCode SetAmpAll(float amp);

  // Goto to the position (in deg) via the shortest path.
  // This returns once the move has started, it doesn't wait for the move to finish
  ErrorCode GotoPos(float deg);

  // Start a relative move of the passed number of deg.
  ErrorCode MoveRel(float deg);

  // Start running at a constant velocity.
  // The velocity is specified in deg/sec units
  ErrorCode RunAtVelocity(float vel);

  // Decelerate to zero velocity and hold position.
  // This can also be used to enable the motor without causing any motion
  ErrorCode SoftStop();

  // Stop abruptly and hold position.
  // This can also be used to enable the motor without causing any motion.
  ErrorCode HardStop();

  // Decelerate to zero velocity and disable.
  ErrorCode SoftDisable();

  // Immediately disable the motor.
  ErrorCode HardDisable();

  // Reset the motor position to zero.
  ErrorCode ClearPosition();

  // Reset the stepper chip
  ErrorCode Reset();

  // Read the status of the stepper chip.
  // Like all reads, this can only be done in the background loop.
  ErrorCode GetStatus(Status *stat);

  // Set and get parameters.  These are mostly for internal use.
  // The higher level methods should generally be used instead.
  ErrorCode SetParam(Param param, uint32_t value);
  ErrorCode GetParam(Param param, uint32_t *value);

  // For testing purposes
  bool power_step() const { return power_step_; }

 private:
  size_t slave_index_;

  // Number of full steps/rev
  // Defaults to the standard value for most steppers
  uint32_t steps_per_rev_{200};

  float DegreesPerSecondToRegisterValue(float speed, float conversion_factor) const;
  float RegisterValueToDegreesPerSecond(int32_t value, float conversion_factor) const;
  int32_t DegreesToMicrosteps(float deg) const;
  ErrorCode SetKval(Param param, float amp);

  // Send a command and wait for the response (if any)
  ErrorCode SendCmd(uint8_t *command, size_t length, uint8_t *response = nullptr);

  // True if this is a powerSTEP chip.
  bool power_step_{false};

  // Stepper's daisy chain used to speak with the steppers on the SPI bus
  Chain *stepper_chain_;
};

}  // namespace StepMotor
