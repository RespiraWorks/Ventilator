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

#if defined(BARE_STM32)

#include "stepper.h"
#include "hal.h"
#include "hal_stm32.h"
#include <math.h>
#include <string.h>

// Static data members
StepMotor StepMotor::motor_[StepMotor::kTotalMotors];
uint8_t StepMotor::dma_buff_[StepMotor::kTotalMotors];
StepCommState StepMotor::coms_state_ = StepCommState::IDLE;

// This array holds the length of each parameter in units of
// bytes, rounded up to the nearest byte.  This info is based
// on table 12 in the powerSTEP chip data sheet
uint8_t StepMotor::param_len_[32] = {
    0, // 0x00 - No valid parameter with ID 0
    3, // 0x01 - Absolute position (22 bits)
    2, // 0x02 - Electrical position (9 bits)
    3, // 0x03 - Mark position (22 bits)
    3, // 0x04 - Current speed (20 bits)
    2, // 0x05 - Acceleration (12 bits)
    2, // 0x06 - Deceleration (12 bits)
    2, // 0x07 - Maximum speed (10 bits)
    2, // 0x08 - Minimum speed (12 bits)
    1, // 0x09 - KVAL_HOLD Holding K VAL (8 bits)
    1, // 0x0A - KVAL_RUN Constant speed K VAL (8 bits)
    1, // 0x0B - KVAL_ACC Acceleration starting K VAL (8 bits)
    1, // 0x0C - KVAL_DEC Deceleration starting K VAL (8 bits)
    2, // 0x0D - INT_SPEED Intersect speed (14 bits)
    1, // 0x0E - ST_SLP Start slope (8 bits)
    1, // 0x0F - FN_SLP_ACC Acceleration final slope (8 bits)
    1, // 0x10 - FN_SLP_DEC Deceleration final slope (8 bits)
    1, // 0x11 - K_THERM Thermal compensation factor (4 bits)
    1, // 0x12 - ADC output (5 bits)
    1, // 0x13 - OCD threshold (5 bits)
    1, // 0x14 - STALL_TH STALL threshold (5 bits)
    2, // 0x15 - Full-step speed (11 bits)
    1, // 0x16 - Step mode (8 bits)
    1, // 0x17 - Alarm enables (8 bits)
    2, // 0x18 - Gate driver configuration (11 bits)
    1, // 0x19 - Gate driver configuration (8 bits)
    2, // 0x1A - IC configuration (16 bits)
    2, // 0x1B - Status (16 bits)
    0, // 0x1C - No such parameter
    0, // 0x1D - No such parameter
    0, // 0x1E - No such parameter
    0, // 0x1F - No such parameter
};

// These constants are used to convert speeds in steps/sec
// to the weird values used by the stepper driver chip.
// Note that different registers use different conversion
// factors.  See the chip data sheet for details.
static const float tick_time = 250e-9f;
static const float cvt_vel_crnt_speed_reg = tick_time * (1 << 28);
static const float cvt_vel_max_speed_reg = tick_time * (1 << 18);
static const float cvt_vel_min_speed_reg = tick_time * (1 << 24);
static const float cvt_vel_fs_speed_reg = tick_time * (1 << 18);
static const float cvt_vel_int_speed_reg = tick_time * (1 << 26);

// The number of microsteps / full step.
// For now this is a constant, we're just using the
// default value of the chip.
static const int ustep_per_step_ = 128;

// These functions raise and lower the chip select pin
inline void CS_High() { GPIO_SetPin(GPIO_B_BASE, 6); }
inline void CS_Low() { GPIO_ClrPin(GPIO_B_BASE, 6); }

StepMotor::StepMotor() {}

StepMtrErr StepMotor::SetParam(StepMtrParam param, uint32_t value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(param_len_))
    return StepMtrErr::BAD_PARAM;

  int len = param_len_[p];
  if (!len || (len > 3))
    return StepMtrErr::BAD_PARAM;

  uint8_t cmdBuff[4];
  cmdBuff[0] = p; // The op-code for a set is just the parameter number

  // Split the parameter value into bytes, MSB first
  value <<= 8 * (3 - len);
  cmdBuff[1] = static_cast<uint8_t>(value >> 16);
  cmdBuff[2] = static_cast<uint8_t>(value >> 8);
  cmdBuff[3] = static_cast<uint8_t>(value);

  // The op-code for a set is equal to the parameter number
  return SendCmd(cmdBuff, len + 1);
}

StepMtrErr StepMotor::GetParam(StepMtrParam param, uint32_t *value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(param_len_))
    return StepMtrErr::BAD_PARAM;

  int len = param_len_[p];
  if (!len || (len > 3))
    return StepMtrErr::BAD_PARAM;

  // It's not legal to call this from the control loop because it
  // has to block.  Return an error if we're in an interrupt handler
  if (Hal.InInterruptHandler())
    return StepMtrErr::WOULD_BLOCK;

  uint8_t cmdBuff[4];

  // For a get, the op-code is the parameter | 0x20
  cmdBuff[0] = static_cast<uint8_t>(p | 0x20);
  cmdBuff[1] = 0;
  cmdBuff[2] = 0;
  cmdBuff[3] = 0;

  StepMtrErr err = SendCmd(cmdBuff, len + 1);
  if (err != StepMtrErr::OK)
    return err;

  // At this point, the response from the stepper driver
  // chip is in my buffer.  The first byte is zero, and
  // the returned data is in bytes 1-N stored MSB first.
  *value = 0;
  for (int i = 0; i < len; i++) {
    *value <<= 8;
    *value |= cmdBuff[i + 1];
  }

  return err;
}

/******************************************************************
 * SPI port used to talk to stepper motor drivers.
 *
 * Note that the stepper motor drivers have a somewhat inefficient
 * SPI implementation.  We need to pull the chip select (CS) line
 * high between each byte we send and hold it high for 625ns.
 * That means that we can't stream the data using DMA, we have to
 * handle every byte with an interrupt and busy wait in the interrupt
 * handler.
 *
 * One positive is that you can daisy chain multiple stepper drivers
 * and commuicate with them all at once.  If you have N drivers then
 * you would send N bytes all at once and bring the CS line high
 * between sets of N bytes.  The bits flow through the chain of
 * stepper drivers all connected together and the data is latched
 * on the rising edge of CS.  That's a little better for us since
 * we have 3 steppers.
 *****************************************************************/
void HalApi::StepperMotorInit() {
  EnableClock(SPI1_BASE);
  EnableClock(DMA2_BASE);

  // The following pins are used to talk to the stepper
  // drivers:
  //   PA5 - SCLK
  //   PA6 - MISO
  //   PA7 - MOSI
  //   PB6 - CS
  //
  // Some additional pins I don't really care about, but
  // are connected to the stepper developer's board
  // For the most part I can just ignore these
  //   PA9  - Reset input.
  //   PA10 - flag open drain output
  //   PB4  - busy open drain output
  //   PC10 - STCK input

  // Configure the CS and reset pins as outputs.
  // pulled high.  I don't really use the reset pin,
  // I just want it to be high so I don't reset the
  // part inadvertantly
  CS_High();
  GPIO_SetPin(GPIO_A_BASE, 9);
  GPIO_PinMode(GPIO_B_BASE, 6, GPIO_PinMode::OUT);
  GPIO_PinMode(GPIO_A_BASE, 9, GPIO_PinMode::OUT);

  // Assign the three SPI pins to the SPI peripheral
  GPIO_PinAltFunc(GPIO_A_BASE, 5, 5);
  GPIO_PinAltFunc(GPIO_A_BASE, 6, 5);
  GPIO_PinAltFunc(GPIO_A_BASE, 7, 5);

  // Set the output pins to use the highest speed setting
  GPIO_OutSpeed(GPIO_A_BASE, 5, GPIO_OutSpeed::SMOKIN);
  GPIO_OutSpeed(GPIO_A_BASE, 7, GPIO_OutSpeed::SMOKIN);

  // Configure my SPI port to talk to the stepper
  SPI_Regs *const spi = SPI1_BASE;

  // Configure the SPI to work in 8-bit data mode
  // Enable RXNE interrupts
  spi->ctrl2.rx_dma_en = 1; // Enable DMA on receive
  spi->ctrl2.tx_dma_en = 1; // Enable DMA on transmit
  spi->ctrl2.ds = 7;        // 8-bit data size
  spi->ctrl2.frxth = 1;     // Receive interrupt on every byte

  // Configure for master mode, CPOL and CPHA both 0.
  // The stepper driver chip has a max SPI clock
  // rate of 5MHz which is the rate I'll use.
  spi->ctrl1.cpha = 1; // Data is sampled on the rising edge of the clock
  spi->ctrl1.cpol = 1; // Clock line is high when not active
  spi->ctrl1.mstr = 1; // We're acting as the master on the SPI bus
  spi->ctrl1.br = 3;   // Set bit rate for 80MHz/16 or 5MHz
  spi->ctrl1.ssi = 1;  // Enable software slave select
  spi->ctrl1.ssm = 1;  // Software slave select management
  spi->ctrl1.spe = 1;  // Enable the SPI module

  // DMA2 channels 3 and 4 can be used to handle rx and tx interrupts from
  // SPI1 respectively.
  DMA_SelectChannel(DMA2_BASE, DMA_Chan::C3, 4);
  DMA_SelectChannel(DMA2_BASE, DMA_Chan::C4, 4);

  // The two DMA channels move data to/from the SPI data register.
  DMA_Regs *dma = DMA2_BASE;
  int C3 = static_cast<int>(DMA_Chan::C3);
  int C4 = static_cast<int>(DMA_Chan::C4);
  dma->channel[C3].pAddr = &spi->data;
  dma->channel[C4].pAddr = &spi->data;

  // Configure the DMA channels, but don't enable them yet
  dma->channel[C3].config.enable = 0;
  dma->channel[C3].config.tcie = 1;
  dma->channel[C3].config.htie = 0;
  dma->channel[C3].config.teie = 0;
  dma->channel[C3].config.dir =
      static_cast<REG>(DmaChannelDir::PERIPHERAL_TO_MEM);
  dma->channel[C3].config.circular = 0;
  dma->channel[C3].config.perInc = 0;
  dma->channel[C3].config.memInc = 1;
  dma->channel[C3].config.psize = 0;
  dma->channel[C3].config.msize = 0;
  dma->channel[C3].config.priority = 0;
  dma->channel[C3].config.mem2mem = 0;

  dma->channel[C4].config.enable = 0;
  dma->channel[C4].config.tcie = 0;
  dma->channel[C4].config.htie = 0;
  dma->channel[C4].config.teie = 0;
  dma->channel[C4].config.dir =
      static_cast<REG>(DmaChannelDir::MEM_TO_PERIPHERAL);
  dma->channel[C4].config.circular = 0;
  dma->channel[C4].config.perInc = 0;
  dma->channel[C4].config.memInc = 1;
  dma->channel[C4].config.psize = 0;
  dma->channel[C4].config.msize = 0;
  dma->channel[C4].config.priority = 0;
  dma->channel[C4].config.mem2mem = 0;

  Hal.EnableInterrupt(InterruptVector::DMA2_CH3, IntPriority::STANDARD);

  StepMotor::OneTimeInit();
}

// Do some basic init of the stepper motor chips so we can
// make them spin the motors
void StepMotor::OneTimeInit() {
  uint32_t val;
  for (int i = 0; i < kTotalMotors; i++) {

    StepMotor *mtr = StepMotor::GetStepper(i);

    // Reset the chip to default values
    mtr->Reset();

    // Get the first gate config register of the powerSTEP01.
    // This is actually the config register on the L6470
    mtr->GetParam(StepMtrParam::GATE_CFG1, &val);

    // If this is at the default config register value for
    // the L6470 then I don't need to do any more configuration
    if (val == 0x2E88)
      continue;

    // Configure the two gate config registers to reasonable values
    //
    // GATE_CFG1 xxxxxxxxxxxxxxxx
    //           ...........\\\\\ - time at constant current 3750ns (0x1D)
    //           ........\\\------- Gate current 96mA (7)
    //           .....\\\---------- Turn off boost time 1uS (7)
    //           ....\------------- Watch dog enable (1)
    //           \\\\-------------- reserved
    mtr->SetParam(StepMtrParam::GATE_CFG1, 0x0FFD);

    // GATE_CFG2 xxxxxxxx
    //           ...\\\\\---------- Dead time 1000ns (7)
    //           \\\--------------- Blanking time 1000ns (7)
    mtr->SetParam(StepMtrParam::GATE_CFG2, 0xF7);
  }
}

// Convert a velocity from Deg/sec units to the value to program
// into one of the stepper controller registers
float StepMotor::DpsToVelReg(float vel, float cnv) {

  // Convert to steps / sec
  float step_per_sec = vel * static_cast<float>(steps_per_rev_) / 360.0f;

  // Multiply that by the register specific conversion factor
  return cnv * step_per_sec;
}

// Convert a velocity from a register value to deg/sec
float StepMotor::RegVelToDps(int32_t val, float cnv) {
  return static_cast<float>(val) * 360.0f /
         (cnv * static_cast<float>(steps_per_rev_));
}

// Read the current absolute motor velocity and return it
// in deg/sec units
// Note that this value is always positive
StepMtrErr StepMotor::GetCurrentSpeed(float *ret) {
  uint32_t val;
  StepMtrErr err = GetParam(StepMtrParam::SPEED, &val);
  *ret = RegVelToDps(val, cvt_vel_crnt_speed_reg);
  return err;
}

// Set the motor's max speed setting in deg/sec
//
// NOTE - The motor must be disabled to set this
StepMtrErr StepMotor::SetMaxSpeed(float dps) {
  if (dps < 0)
    return StepMtrErr::BAD_VALUE;

  uint32_t speed =
      static_cast<uint32_t>(DpsToVelReg(dps, cvt_vel_max_speed_reg));
  if (speed > 0x3ff)
    speed = 0x3ff;

  return SetParam(StepMtrParam::MAX_SPEED, speed);
}

// Get the motor's max speed setting in deg/sec
StepMtrErr StepMotor::GetMaxSpeed(float *ret) {
  uint32_t val;
  StepMtrErr err = GetParam(StepMtrParam::MAX_SPEED, &val);
  *ret = RegVelToDps(val, cvt_vel_max_speed_reg);
  return err;
}

// Set the motor's min speed setting in deg/sec
//
// NOTE - Setting a non-zero minimum speed doesn't mean
// the motor can't stop, this is the minimum speed for
// a move.  When you start a move, rathern then increase
// linearly from 0, the stepper will jump to this speed
// immediately, then ramp up from there.
// This can help with vibration.
//
// NOTE - The motor must be disabled to set this
StepMtrErr StepMotor::SetMinSpeed(float dps) {
  if (dps < 0)
    return StepMtrErr::BAD_VALUE;

  uint32_t speed =
      static_cast<uint32_t>(DpsToVelReg(dps, cvt_vel_min_speed_reg));
  if (speed > 0xfff)
    speed = 0xfff;

  return SetParam(StepMtrParam::MIN_SPEED, speed);
}

// Get the motor's min speed setting in deg/sec
StepMtrErr StepMotor::GetMinSpeed(float *ret) {
  uint32_t val;
  StepMtrErr err = GetParam(StepMtrParam::MIN_SPEED, &val);
  *ret = RegVelToDps(val, cvt_vel_min_speed_reg);
  return err;
}

// Set the motors accel and decel rate in deg/sec/sec units
//
// NOTE - The motor must be disabled to set this
StepMtrErr StepMotor::SetAccel(float acc) {

  static const float scaler = (tick_time * tick_time * 1099511627776.f);
  if (acc < 0)
    return StepMtrErr::BAD_VALUE;

  // Convert from deg/sec/sec to steps/sec/sec
  acc *= static_cast<float>(steps_per_rev_) / 360.0f;

  // Convert to the proper units for the driver chip
  uint32_t val = static_cast<uint32_t>(acc * scaler);
  if (val > 0x0fff)
    val = 0xfff;

  StepMtrErr err = SetParam(StepMtrParam::ACCEL, val);
  if (err != StepMtrErr::OK)
    return err;

  return SetParam(StepMtrParam::DECEL, val);
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
// output to the motor in different phases of it's motion:
//   hold - Value used when the motor is holding position (not moving)
//   run  - Value used when running at constant velocity.
//   accel - Value used when accelerating
//   decel - Value used when decelerating
//
// In all cases the values are set in a range of 0 to 1
// for 0 to 100%
StepMtrErr StepMotor::SetKval(StepMtrParam param, float amp) {

  if ((amp < 0) || (amp > 1))
    return StepMtrErr::BAD_VALUE;

  // The stepper chip uses a value of 0 to 255
  return SetParam(param, static_cast<uint32_t>(amp * 255));
}

StepMtrErr StepMotor::SetAmpHold(float amp) {
  return SetKval(StepMtrParam::KVAL_HOLD, amp);
}
StepMtrErr StepMotor::SetAmpRun(float amp) {
  return SetKval(StepMtrParam::KVAL_RUN, amp);
}
StepMtrErr StepMotor::SetAmpAccel(float amp) {
  return SetKval(StepMtrParam::KVAL_ACCEL, amp);
}
StepMtrErr StepMotor::SetAmpDecel(float amp) {
  return SetKval(StepMtrParam::KVAL_DECEL, amp);
}

StepMtrErr StepMotor::SetAmpAll(float amp) {
  StepMtrErr err = SetAmpHold(amp);
  if (err != StepMtrErr::OK)
    return err;

  err = SetAmpRun(amp);
  if (err != StepMtrErr::OK)
    return err;

  err = SetAmpAccel(amp);
  if (err != StepMtrErr::OK)
    return err;

  return SetAmpDecel(amp);
}

// Start running at a constant velocity.
// The velocity is specified in deg/sec units
StepMtrErr StepMotor::RunAtVelocity(float vel) {

  int neg = vel < 0;
  vel = fabsf(vel);

  // Convert the speed from deg/sec to the weird units
  // used by the stepper chip
  float speed = DpsToVelReg(vel, cvt_vel_crnt_speed_reg);

  // The speed value is a 20 bit unsigned integer passed
  // as part of the command.
  int32_t S = static_cast<int32_t>(speed);
  if (S > 0x000fffff)
    S = 0x000fffff;

  uint8_t cmd[4];
  if (neg)
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::RUN_NEG);
  else
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::RUN_POS);

  cmd[1] = static_cast<uint8_t>(S >> 16);
  cmd[2] = static_cast<uint8_t>(S >> 8);
  cmd[3] = static_cast<uint8_t>(S);
  return SendCmd(cmd, 4);
}

// Decelerate to zero velocity and hold position
// This can also be used to enable the motor without
// causing any motion
StepMtrErr StepMotor::SoftStop() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::SOFT_STOP);
  return SendCmd(&cmd, 0);
}

// Stop abruptly and hold position
// This can also be used to enable the motor without
// causing any motion
StepMtrErr StepMotor::HardStop() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::HARD_STOP);
  return SendCmd(&cmd, 0);
}

// Decelerate to zero velocity and disable
StepMtrErr StepMotor::SoftDisable() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::SOFT_DISABLE);
  return SendCmd(&cmd, 0);
}

// Immediately disable the motor
StepMtrErr StepMotor::HardDisable() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::HARD_DISABLE);
  return SendCmd(&cmd, 0);
}

// Reset the motor position to zero
StepMtrErr StepMotor::ClearPosition() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::RESET_POS);
  return SendCmd(&cmd, 0);
}

// Reset the stepper chip
StepMtrErr StepMotor::Reset() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::RESET_DEVICE);
  return SendCmd(&cmd, 0);
}

StepMtrErr StepMotor::GetStatus(StepperStatus *stat) {

  // It's not legal to call this from the control loop because it
  // has to block.  Return an error if we're in an interrupt handler
  if (Hal.InInterruptHandler())
    return StepMtrErr::WOULD_BLOCK;

  uint8_t cmd[] = {static_cast<uint8_t>(StepMtrCmd::GET_STATUS), 0, 0};

  StepMtrErr err = SendCmd(cmd, 3);
  if (err != StepMtrErr::OK)
    return err;

  stat->enabled = (cmd[2] & 0x01) == 0;
  stat->command_error = (cmd[2] & 0x80) || (cmd[1] & 0x01);
  stat->under_voltage = (cmd[1] & 0x02) == 0;
  stat->thermal_warning = (cmd[1] & 0x04) == 0;
  stat->thermal_shutdown = (cmd[1] & 0x08) == 0;
  stat->over_current = (cmd[1] & 0x10) == 0;
  stat->step_loss = (cmd[1] & 0x60) != 0x60;

  switch (cmd[2] & 0x60) {
  case 0x00:
    stat->move_status = StepMoveStatus::STOPPED;
    break;
  case 0x20:
    stat->move_status = StepMoveStatus::ACCELERATING;
    break;
  case 0x40:
    stat->move_status = StepMoveStatus::DECELERATING;
    break;
  case 0x60:
    stat->move_status = StepMoveStatus::CONSTANT_SPEED;
    break;
  }

  return err;
}

int32_t StepMotor::DegToUstep(float deg) {
  uint32_t ustep_per_rev = ustep_per_step_ * steps_per_rev_;

  float steps = static_cast<float>(ustep_per_rev) * deg / 360.0f;
  return static_cast<int32_t>(steps);
}

// Goto to the position (in deg) via the shortest path
// This returns once the move has started, it doesn't
// wait for the move to finish
StepMtrErr StepMotor::GotoPos(float deg) {
  int32_t ustep = DegToUstep(deg);

  uint8_t cmd[4];
  cmd[0] = static_cast<uint8_t>(StepMtrCmd::GOTO);
  cmd[1] = static_cast<uint8_t>(ustep >> 16);
  cmd[2] = static_cast<uint8_t>(ustep >> 8);
  cmd[3] = static_cast<uint8_t>(ustep);
  return SendCmd(cmd, 4);
}

// Start a relative move of the passed number of deg.
StepMtrErr StepMotor::MoveRel(float deg) {
  int32_t dist = DegToUstep(deg);

  uint8_t cmd[4];

  if (dist < 0) {
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::MOVE_NEG);
    dist *= -1;
  } else
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::MOVE_POS);

  if (dist > 0x003FFFFF)
    return StepMtrErr::BAD_VALUE;

  cmd[1] = static_cast<uint8_t>(dist >> 16);
  cmd[2] = static_cast<uint8_t>(dist >> 8);
  cmd[3] = static_cast<uint8_t>(dist);
  return SendCmd(cmd, 4);
}

// Send a command to the motor and wait for the response.
// The passed buffer should be at least len bytes long.
// On entry it holds the commands (or commands) that will be
// sent to the stepper.  On return it will hold the response
// we received from the stepper.
//
// Each command to the stepper consists of a one byte command
// code and 0 to 3 data bytes.  Multiple commands can be placed
// one after another and all sent as a unit with this function.
//
// NOTE - this is a blocking call and should not be called from
// the high priority controller thread.  If it is, it will return
// an error.
//
StepMtrErr StepMotor::SendCmd(uint8_t *cmd, uint32_t len) {

  // See if we're running from an interrupt handler
  // (i.e. the controller thread).  If so we just
  // add this command to our queue and return
  if (Hal.InInterruptHandler())
    return EnqueueCmd(cmd, len);

  // Copy the command to my buffer with interrupts disabled.
  // I want to make sure the whole command gets sent as one continuous
  // stream and it's possible that commands are currently being sent
  // to other motors.
  StepCommState state;
  {
    BlockInterrupts block;
    cmd_ptr_ = cmd;
    cmd_remain_ = len;
    state = coms_state_;
  }

  // If the communications interface is idle, start it.
  if (state == StepCommState::IDLE)
    UpdateComState();

  // Wait for the ISR to finish sending the command.
  // When it does, it will set the pointer to NULL
  while (cmd_ptr_) {
  }

  return StepMtrErr::OK;
}

// Enqueue the command and return immediately.  This is called
// from the controller loop to send commands to the drivers.
StepMtrErr StepMotor::EnqueueCmd(uint8_t *cmd, uint32_t len) {

  // Block interrupts during this call to prevent race conditions
  // with the ISR that handles the communication.
  BlockInterrupts block;

  // It's illegal to call this if we're currently pulling data
  // from the queues.
  if (coms_state_ == StepCommState::SEND_QUEUED)
    return StepMtrErr::INVALID_STATE;

  if (queue_count_ + len > sizeof(queue_))
    return StepMtrErr::QUEUE_FULL;

  memcpy(&queue_[queue_count_], cmd, len);
  queue_count_ += len;

  return StepMtrErr::OK;
}

// Update the communications state machine.
//
// This is called from the ISR when the next byte of the
// command needs to be sent.
//
// It's also called when a new command needs to be sent,
// but only if the state machine is idle.  This shouldn't
// be called outside the DMA ISR when the state machine
// isn't idle or some nasty race conditions may result.
//
// Since this is a private method that's not really an
// issue.  This is mostly just a warning to future
// developers who may be tinkering with this code.
void StepMotor::UpdateComState() {

  // This will get set to true if I find any data to send
  bool data_to_send = false;
  switch (coms_state_) {

  //////////////////////////////////////////////
  // If we're idle it means we're starting
  // a new command.  Bump the state and fall
  // through to the next case.  I always look
  // for queued data first.
  //////////////////////////////////////////////
  case StepCommState::IDLE:
    coms_state_ = StepCommState::SEND_QUEUED;
    // fall through

  //////////////////////////////////////////////
  // We're sending data from the local queues.
  // See if the queues still have more data to send.
  // Note that I just ignore any response from
  // the motor drivers when sending data from the queues
  //////////////////////////////////////////////
  case StepCommState::SEND_QUEUED:

    // For each motor, grab the next byte from the queue
    // and add it to my DMA buffer.  For any empty queue
    // I just add a NOP command
    for (int i = 0; i < kTotalMotors; i++) {

      // This really should already be false
      motor_[i].save_response_ = false;

      if (motor_[i].queue_ndx_ < motor_[i].queue_count_) {
        data_to_send = true;
        dma_buff_[i] = motor_[i].queue_[motor_[i].queue_ndx_++];
      }

      else {
        motor_[i].queue_ndx_ = 0;
        motor_[i].queue_count_ = 0;
        dma_buff_[i] = static_cast<uint8_t>(StepMtrCmd::NOP);
      }
    }

    // If any data was found in the queues, then I'm done.
    // Otherwise, move on to the next state.
    if (data_to_send)
      break;

    coms_state_ = StepCommState::SEND_SYNC;
    // fall through

  //////////////////////////////////////////////
  // Sending data sent by the background task.
  // In this state I also save the response from the
  // motor driver chips.
  //////////////////////////////////////////////
  case StepCommState::SEND_SYNC:

    for (int i = 0; i < kTotalMotors; i++) {

      // If we sent this motor driver chip a command from
      // this state last time, save the response in the same
      // buffer that the command came from.
      if (motor_[i].save_response_) {
        *motor_[i].cmd_ptr_++ = dma_buff_[i];
        if (--motor_[i].cmd_remain_ <= 0)
          motor_[i].cmd_ptr_ = 0;
      }

      // If this motor has an active command to send,
      // grab the next byte, otherwise send a NOP
      if (motor_[i].cmd_ptr_) {
        data_to_send = true;
        motor_[i].save_response_ = true;
        dma_buff_[i] = *motor_[i].cmd_ptr_;
      } else {
        motor_[i].save_response_ = false;
        dma_buff_[i] = static_cast<uint8_t>(StepMtrCmd::NOP);
      }
    }

    // If I didn't find anything to send, then set our
    // state to idle and return, I'm done.
    if (!data_to_send) {
      coms_state_ = StepCommState::IDLE;
      return;
    }
  }

  //////////////////////////////////////////////
  // I've got a message to send out to the chain
  // of motor driver chips.  Set up my DMA to
  // send it out.
  //////////////////////////////////////////////
  int C3 = static_cast<int>(DMA_Chan::C3);
  int C4 = static_cast<int>(DMA_Chan::C4);

  DMA_Regs *dma = DMA2_BASE;
  dma->channel[C3].config.enable = 0;
  dma->channel[C4].config.enable = 0;

  dma->channel[C3].count = kTotalMotors;
  dma->channel[C4].count = kTotalMotors;
  dma->channel[C3].mAddr = dma_buff_;
  dma->channel[C4].mAddr = dma_buff_;

  // NOTE - CS has to be high for at least 650ns between bytes.
  // I don't bother timing this because I've found that in
  // practice it takes longer then that to handle the interrupt
  CS_Low();

  dma->channel[C3].config.enable = 1;
  dma->channel[C4].config.enable = 1;
}

void StepMotor::DMA_ISR() {
  CS_High();

  // Clear the DMA interrupt
  DMA_ClearInt(DMA2_BASE, DMA_Chan::C3, DmaInterrupt::GLOBAL);

  UpdateComState();
}

// This is called from the HAL at the end of the high
// priority loop timer ISR.  If the comm state machine
// is idle it starts a new transmission
void StepMotor::StartQueuedCommands() {
  if (coms_state_ == StepCommState::IDLE)
    UpdateComState();
}

#endif
