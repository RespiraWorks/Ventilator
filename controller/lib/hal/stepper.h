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

This file implements the HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.  Details of the processor's
peripherals can be found in the reference manual for that processor:
   https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

Details specific to the ARM processor used in this chip can be found in
the programmer's manual for the processor available here:
   https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf

*/

#ifndef STEPPER_H_
#define STEPPER_H_

#include "circular_buffer.h"

// These are the simple opcodes for the stepper driver.
// Not included here are set/get parameter which include
// the parameter ID value as part of the code.
enum class StepMtrCmd {
  NOP = 0, // Used when there's no other command to send

  RUN_NEG =
      0x50, // Start running at a constant speed, 2 byte speed is given as data
  RUN_POS =
      0x51, // Start running at a constant speed, 2 byte speed is given as data
  STEP_CLK_NEG = 0x58, // Switch to step clock mode moving in negative direction
  STEP_CLK_POS = 0x59, // Switch to step clock mode moving in positive direction
  MOVE_NEG = 0x40,     // Move - steps, steps is given as a parameter
  MOVE_POS = 0x41,     // Move + steps, steps is given as a parameter
  GOTO = 0x60,         // Go to an absolute position using shortest path
  GOTO_NEG = 0x68,     // Go to an absolute position in negative direction
  GOTO_POS = 0x69,     // Go to an absolute position in positive direction
  HOME = 0x70,         // Same as GOTO 0
  RESET_POS = 0xD8,    // Set absolute position register to 0
  RESET_DEVICE = 0xC0, // Reset the stepper chip
  SOFT_STOP = 0xB0,    // Perform a soft stop
  HARD_STOP = 0xB8,    // Perform a hard stop
  SOFT_DISABLE = 0xA0, // Soft stop, then disable
  HARD_DISABLE = 0xA8, // Hard stop, then disable
  GET_STATUS = 0xD0,   // Read the 16-bit status word
};

enum class StepMtrParam {
  ABS_POS = 0x01,           // Absolute position
  ELE_POS = 0x02,           // Electrical position
  MARK_POS = 0x03,          // Mark position
  SPEED = 0x04,             // Current speed
  ACCEL = 0x05,             // Acceleration
  DECEL = 0x06,             // Deceleration
  MAX_SPEED = 0x07,         // Maximum allowed speed
  MIN_SPEED = 0x08,         // Minimum allowed speed
  ADC_OUT = 0x12,           //
  OVER_CRNT_THRESH = 0x13,  //
  FULL_STEP_SPEED = 0x15,   // Speed at which to switch to full step mode
  STEP_MODE = 0x16,         //
  ALARM_ENA = 0x17,         //
  GATE_CFG1 = 0x18,         //
  GATE_CFG2 = 0x18,         //
  STATUS = 0x1B,            //
  CONFIG = 0x1A,            //
  KVAL_HOLD = 0x09,         //
  KVAL_RUN = 0x0A,          //
  KVAL_ACCEL = 0x0B,        //
  KVAL_DECEL = 0x0C,        //
  INT_SPEED = 0x0D,         //
  START_SLOPE = 0x0E,       //
  FINAL_SLOPE_ACCEL = 0x0F, //
  FINAL_SLOPE_DECEL = 0x10, //
  THERMAL_COMP = 0x11,      //
  STALL_THRESH = 0x14,      //
};

// Error codes returned by my functions
enum class StepMtrErr {
  OK,
  CMD_TOO_LONG, // Command is too long to fit in my command buffer
  BAD_PARAM,    // The parameter ID is invalid
};

// Represents one of the stepper motors in the system
class StepMotor {
  static const int totalMotors = 1;

public:
  StepMotor();

  // Return a pointer to the Nth stepper motor
  // in the system.  Returns NULL for an invalid input
  static StepMotor *GetStepper(int n) {
    if ((n < 0) || (n >= totalMotors))
      return 0;
    return &motor[n];
  }

  // Set and get parameters.
  // These commands block until the command finishes
  StepMtrErr SetParam(StepMtrParam param, uint32_t value);
  StepMtrErr GetParam(StepMtrParam param, uint32_t *value);

private:
  static StepMotor motor[totalMotors];
  static uint8_t dmaBuff[totalMotors];
  static uint8_t paramLen[32];
  static uint32_t activeMask;

  // These two buffers hold the command and response bytes
  // to be sent to the motor.
  CircBuff<uint8_t, 20> cmd;
  CircBuff<uint8_t, 20> rsp;

  // Send a command and wait for the response
  StepMtrErr SendCmd(StepMtrCmd op, uint8_t *data, int dlen, wait = false);

  static void StartCmd(bool fromISR = false);

public:
  // Interrupt service routine.
  // This has to be public, but don't call it
  static void DMA_ISR();
};

#endif
