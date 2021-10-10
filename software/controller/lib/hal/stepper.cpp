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

#include "stepper.h"

StepMotor StepMotor::motor_[StepMotor::MaxMotors];
int StepMotor::total_motors_;

#if defined(BARE_STM32)

#include <cmath>
#include <cstring>

#include "clocks.h"
#include "dma.h"
#include "gpio.h"
#include "interrupts.h"
#include "spi.h"
#include "system_timer.h"

// Static data members
uint8_t StepMotor::dma_buff_[StepMotor::MaxMotors];
StepCommState StepMotor::coms_state_ = StepCommState::Idle;

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
static constexpr int MicrostepPerStep = 128;

// These functions raise and lower the chip select pin
void chip_select_high() { GPIO::set_pin(GPIO::Port::B, 6); }
void chip_select_low() { GPIO::clear_pin(GPIO::Port::B, 6); }

StepMtrErr StepMotor::SetParam(StepMtrParam param, uint32_t value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(param_len_)) return StepMtrErr::BadParam;

  int len = param_len_[p];
  if (!len || (len > 3)) return StepMtrErr::BadParam;

  uint8_t cmd_buff[4];
  cmd_buff[0] = p;  // The op-code for a set is just the parameter number

  // Split the parameter value into bytes, MSB first
  value <<= 8 * (3 - len);
  cmd_buff[1] = static_cast<uint8_t>(value >> 16);
  cmd_buff[2] = static_cast<uint8_t>(value >> 8);
  cmd_buff[3] = static_cast<uint8_t>(value);

  // The op-code for a set is equal to the parameter number
  return SendCmd(cmd_buff, len + 1);
}

StepMtrErr StepMotor::GetParam(StepMtrParam param, uint32_t *value) {
  uint8_t p = static_cast<uint8_t>(param);
  if (p > sizeof(param_len_)) return StepMtrErr::BadParam;

  int len = param_len_[p];
  if (!len || (len > 3)) return StepMtrErr::BadParam;

  // It's not legal to call this from the control loop because it
  // has to block.  Return an error if we're in an interrupt handler
  if (Interrupts::singleton().InInterruptHandler()) return StepMtrErr::WouldBlock;

  uint8_t cmd_buff[4];

  // For a get, the op-code is the parameter | 0x20
  cmd_buff[0] = static_cast<uint8_t>(p | 0x20);
  cmd_buff[1] = 0;
  cmd_buff[2] = 0;
  cmd_buff[3] = 0;

  StepMtrErr err = SendCmd(cmd_buff, len + 1);
  if (err != StepMtrErr::Ok) return err;

  // At this point, the response from the stepper driver
  // chip is in my buffer.  The first byte is zero, and
  // the returned data is in bytes 1-N stored MSB first.
  *value = 0;
  for (int i = 0; i < len; i++) {
    *value <<= 8;
    *value |= cmd_buff[i + 1];
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
 * and communicate with them all at once.  If you have N drivers then
 * you would send N bytes all at once and bring the CS line high
 * between sets of N bytes.  The bits flow through the chain of
 * stepper drivers all connected together and the data is latched
 * on the rising edge of CS.  That's a little better for us since
 * we have 3 steppers.
 *****************************************************************/
void StepMotor::OneTimeInit() {
  enable_peripheral_clock(PeripheralID::SPI1);
  enable_peripheral_clock(PeripheralID::DMA2);

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
  // part inadvertently
  chip_select_high();
  GPIO::set_pin(GPIO::Port::A, 9);
  GPIO::pin_mode(GPIO::Port::B, 6, GPIO::PinMode::Output);
  GPIO::pin_mode(GPIO::Port::A, 9, GPIO::PinMode::Output);

  // Assign the three SPI pins to the SPI peripheral, [DS] Table 17 (pg 76)
  GPIO::alternate_function(GPIO::Port::A, /*pin =*/5,
                           GPIO::AlternativeFunction::AF5);  // SPI1_SCK
  GPIO::alternate_function(GPIO::Port::A, /*pin =*/6,
                           GPIO::AlternativeFunction::AF5);  // SPI1_MISO
  GPIO::alternate_function(GPIO::Port::A, /*pin =*/7,
                           GPIO::AlternativeFunction::AF5);  // SPI1_MOSI

  // Set the output pins to use the highest speed setting
  GPIO::output_speed(GPIO::Port::A, 5, GPIO::OutSpeed::Smoking);
  GPIO::output_speed(GPIO::Port::A, 7, GPIO::OutSpeed::Smoking);

  // Configure my SPI port to talk to the stepper
  SpiReg *const spi = Spi1Base;

  // Configure the SPI to work in 8-bit data mode
  // Enable RXNE interrupts
  spi->control2.rx_dma = 1;             // Enable DMA on receive
  spi->control2.tx_dma = 1;             // Enable DMA on transmit
  spi->control2.data_size = 7;          // 8-bit data size
  spi->control2.fifo_rx_threshold = 1;  // Receive interrupt on every byte

  // Configure for master mode, CPOL and CPHA both 0.
  // The stepper driver chip has a max SPI clock
  // rate of 5MHz which is the rate I'll use.
  spi->control_reg1.clock_phase = 1;            // Data is sampled on the rising edge of the clock
  spi->control_reg1.clock_polarity = 1;         // Clock line is high when not active
  spi->control_reg1.master = 1;                 // We're acting as the master on the SPI bus
  spi->control_reg1.bitrate = 3;                // Set bit rate for 80MHz/16 or 5MHz
  spi->control_reg1.internal_slave_select = 1;  // Enable software slave select
  spi->control_reg1.sw_slave_management = 1;    // Software slave select management
  spi->control_reg1.enable = 1;                 // Enable the SPI module

  // DMA2 channels 3 and 4 can be used to handle rx and tx interrupts from
  // SPI1 respectively.
  DMA::SelectChannel(DMA::Base::DMA2, DMA::Channel::Chan3, 4);
  DMA::SelectChannel(DMA::Base::DMA2, DMA::Channel::Chan4, 4);

  // The two DMA channels move data to/from the SPI data register.
  DmaReg *dma = DMA::get_register(DMA::Base::DMA2);

  // Configure the DMA channels, but don't enable them yet
  /// \TODO: (subtly) repetitive blocks for each channel - factor this out
  int c3 = static_cast<int>(DMA::Channel::Chan3);
  dma->channel[c3].peripheral_address = &spi->data;
  dma->channel[c3].config.enable = 0;
  dma->channel[c3].config.tx_complete_interrupt = 1;
  dma->channel[c3].config.half_tx_interrupt = 0;
  dma->channel[c3].config.tx_error_interrupt = 0;
  dma->channel[c3].config.direction = static_cast<uint32_t>(DMA::ChannelDir::PeripheralToMemory);
  dma->channel[c3].config.circular = 0;
  dma->channel[c3].config.peripheral_increment = 0;
  dma->channel[c3].config.memory_increment = 1;
  dma->channel[c3].config.peripheral_size = 0;
  dma->channel[c3].config.memory_size = 0;
  dma->channel[c3].config.priority = 0;
  dma->channel[c3].config.mem2mem = 0;

  int c4 = static_cast<int>(DMA::Channel::Chan4);
  dma->channel[c4].peripheral_address = &spi->data;
  dma->channel[c4].config.enable = 0;
  dma->channel[c4].config.tx_complete_interrupt = 0;
  dma->channel[c4].config.half_tx_interrupt = 0;
  dma->channel[c4].config.tx_error_interrupt = 0;
  dma->channel[c4].config.direction = static_cast<uint32_t>(DMA::ChannelDir::MemoryToPeripheral);
  dma->channel[c4].config.circular = 0;
  dma->channel[c4].config.peripheral_increment = 0;
  dma->channel[c4].config.memory_increment = 1;
  dma->channel[c4].config.peripheral_size = 0;
  dma->channel[c4].config.memory_size = 0;
  dma->channel[c4].config.priority = 0;
  dma->channel[c4].config.mem2mem = 0;

  Interrupts::singleton().EnableInterrupt(InterruptVector::Dma2Channel3, IntPriority::Standard);

  // Do some basic init of the stepper motor chips so we can
  // make them spin the motors

  uint32_t val;

  ProbeChips();

  for (int i = 0; i < total_motors_; i++) {
    StepMotor *mtr = StepMotor::GetStepper(i);

    mtr->Reset();

    // We need to delay briefly after reset before sending
    // a new command.  For the power-step chip this delay
    // time is specified as 500 microseconds in the data sheet.
    // For the L6470 its only 45 max
    SystemTimer::singleton().delay(microseconds(500));

    // Get the first gate config register of the powerSTEP01.
    // This is actually the config register on the L6470
    mtr->GetParam(StepMtrParam::GateConfig1, &val);

    // If this is at the default config register value for
    // the L6470 then I don't need to do any more configuration
    if (val == 0x2E88) continue;

    mtr->power_step_ = true;

    // Configure the two gate config registers to reasonable values
    //
    // GateConfig1 xxxxxxxxxxxxxxxx
    //           ...........\\\\\ - time at constant current 3750ns (0x1D)
    //           ........\\\------- Gate current 96mA (7)
    //           .....\\\---------- Turn off boost time 1uS (7)
    //           ....\------------- Watch dog enable (1)
    //           \\\\-------------- reserved
    mtr->SetParam(StepMtrParam::GateConfig1, 0x0FFD);

    // GateConfig2 xxxxxxxx
    //           ...\\\\\---------- Dead time 1000ns (7)
    //           \\\--------------- Blanking time 1000ns (7)
    mtr->SetParam(StepMtrParam::GateConfig2, 0xF7);
  }
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
  if (dps < 0) return StepMtrErr::BadValue;

  uint32_t speed = static_cast<uint32_t>(DpsToVelReg(dps, VelMaxSpeedReg));
  if (speed > 0x3ff) speed = 0x3ff;

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
  if (dps < 0) return StepMtrErr::BadValue;

  uint32_t speed = static_cast<uint32_t>(DpsToVelReg(dps, VelMinSpeedReg));
  if (speed > 0xfff) speed = 0xfff;

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
  if (acc < 0) return StepMtrErr::BadValue;

  // Convert from deg/sec/sec to steps/sec/sec
  acc *= static_cast<float>(steps_per_rev_) / 360.0f;

  // Convert to the proper units for the driver chip
  uint32_t val = static_cast<uint32_t>(acc * Scaler);
  if (val > 0x0fff) val = 0xfff;

  StepMtrErr err = SetParam(StepMtrParam::Acceleration, val);
  if (err != StepMtrErr::Ok) return err;

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
  if (power_step_ && amp < 0.9) amp += 0.15f;

  if ((amp < 0) || (amp > 1)) return StepMtrErr::BadValue;

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
  if (err != StepMtrErr::Ok) return err;

  err = SetAmpRun(amp);
  if (err != StepMtrErr::Ok) return err;

  err = SetAmpAccel(amp);
  if (err != StepMtrErr::Ok) return err;

  return SetAmpDecel(amp);
}

// Start running at a constant velocity.
// The velocity is specified in deg/sec units
StepMtrErr StepMotor::RunAtVelocity(float vel) {
  int neg = vel < 0;
  vel = fabsf(vel);

  // Convert the speed from deg/sec to the weird units
  // used by the stepper chip
  float speed = DpsToVelReg(vel, VelCurrentSpeedReg);

  // The speed value is a 20 bit unsigned integer passed
  // as part of the command.
  int32_t s = static_cast<int32_t>(speed);
  if (s > 0x000fffff) s = 0x000fffff;

  uint8_t cmd[4];
  if (neg)
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::RunNegative);
  else
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::RunPositive);

  cmd[1] = static_cast<uint8_t>(s >> 16);
  cmd[2] = static_cast<uint8_t>(s >> 8);
  cmd[3] = static_cast<uint8_t>(s);
  return SendCmd(cmd, 4);
}

// Decelerate to zero velocity and hold position
// This can also be used to enable the motor without
// causing any motion
StepMtrErr StepMotor::SoftStop() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::SoftStop);
  return SendCmd(&cmd, 1);
}

// Stop abruptly and hold position
// This can also be used to enable the motor without
// causing any motion
StepMtrErr StepMotor::HardStop() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::HardStop);
  return SendCmd(&cmd, 1);
}

// Decelerate to zero velocity and disable
StepMtrErr StepMotor::SoftDisable() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::SoftDisable);
  return SendCmd(&cmd, 1);
}

// Immediately disable the motor
StepMtrErr StepMotor::HardDisable() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::HardDisable);
  return SendCmd(&cmd, 1);
}

// Reset the motor position to zero
StepMtrErr StepMotor::ClearPosition() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::ResetPosition);
  return SendCmd(&cmd, 1);
}

// Reset the stepper chip
StepMtrErr StepMotor::Reset() {
  uint8_t cmd = static_cast<uint8_t>(StepMtrCmd::ResetDevice);
  return SendCmd(&cmd, 1);
}

StepMtrErr StepMotor::GetStatus(StepperStatus *stat) {
  // It's not legal to call this from the control loop because it
  // has to block.  Return an error if we're in an interrupt handler
  if (Interrupts::singleton().InInterruptHandler()) return StepMtrErr::WouldBlock;

  uint8_t cmd[] = {static_cast<uint8_t>(StepMtrCmd::GetStatus), 0, 0};

  StepMtrErr err = SendCmd(cmd, 3);
  if (err != StepMtrErr::Ok) return err;

  stat->enabled = (cmd[2] & 0x01) == 0;
  stat->command_error = (cmd[2] & 0x80) || (cmd[1] & 0x01);
  stat->under_voltage = (cmd[1] & 0x02) == 0;
  stat->thermal_warning = (cmd[1] & 0x04) == 0;
  stat->thermal_shutdown = (cmd[1] & 0x08) == 0;
  stat->over_current = (cmd[1] & 0x10) == 0;
  stat->step_loss = (cmd[1] & 0x60) != 0x60;

  switch (cmd[2] & 0x60) {
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
// This returns once the move has started, it doesn't
// wait for the move to finish
StepMtrErr StepMotor::GotoPos(float deg) {
  int32_t ustep = DegToUstep(deg);

  uint8_t cmd[4];
  cmd[0] = static_cast<uint8_t>(StepMtrCmd::GoTo);
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
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::MoveNegative);
    dist *= -1;
  } else
    cmd[0] = static_cast<uint8_t>(StepMtrCmd::MovePositive);

  if (dist > 0x003FFFFF) return StepMtrErr::BadValue;

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
  if (Interrupts::singleton().InInterruptHandler()) return EnqueueCmd(cmd, len);

  // Copy the command to my buffer with interrupts disabled.
  // I want to make sure the whole command gets sent as one continuous
  // stream and it's possible that commands are currently being sent
  // to other motors.
  StepCommState state;
  {
    BlockInterrupts block;
    memcpy(&last_cmd_[0], cmd, len);
    cmd_ptr_ = &last_cmd_[0];
    cmd_remain_ = len;
    state = coms_state_;
  }

  // If the communications interface is idle, start it.
  if (state == StepCommState::Idle) UpdateComState();

  // Wait for the ISR to finish sending the command.
  // When it does, it will set the pointer to NULL
  while (cmd_ptr_) {
  }

  // The ISR replaces the command with the response, I need to copy the
  // response so the caller can use it.
  memcpy(cmd, &last_cmd_[0], len);
  // TODO: have a dedicated place to place the stepper response so the ISR
  // doesn't reuse the location of the command to put the response.

  return StepMtrErr::Ok;
}

// Enqueue the command and return immediately.  This is called
// from the controller loop to send commands to the drivers.
StepMtrErr StepMotor::EnqueueCmd(uint8_t *cmd, uint32_t len) {
  // Block interrupts during this call to prevent race conditions
  // with the ISR that handles the communication.
  BlockInterrupts block;

  // It's illegal to call this if we're currently pulling data
  // from the queues.
  if (coms_state_ == StepCommState::SendQueued) return StepMtrErr::InvalidState;

  if (queue_count_ + len > sizeof(queue_)) return StepMtrErr::QueueFull;

  memcpy(&queue_[queue_count_], cmd, len);
  queue_count_ += len;

  return StepMtrErr::Ok;
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
    case StepCommState::Idle:
      coms_state_ = StepCommState::SendQueued;
      // fall through

    //////////////////////////////////////////////
    // We're sending data from the local queues.
    // See if the queues still have more data to send.
    // Note that I just ignore any response from
    // the motor drivers when sending data from the queues
    //////////////////////////////////////////////
    case StepCommState::SendQueued:

      // For each motor, grab the next byte from the queue
      // and add it to my DMA buffer.  For any empty queue
      // I just add a Nop command
      for (int i = 0; i < total_motors_; i++) {
        // This really should already be false
        motor_[i].save_response_ = false;

        if (motor_[i].queue_ndx_ < motor_[i].queue_count_) {
          data_to_send = true;
          dma_buff_[i] = motor_[i].queue_[motor_[i].queue_ndx_++];
        }

        else {
          motor_[i].queue_ndx_ = 0;
          motor_[i].queue_count_ = 0;
          dma_buff_[i] = static_cast<uint8_t>(StepMtrCmd::Nop);
        }
      }

      // If any data was found in the queues, then I'm done.
      // Otherwise, move on to the next state.
      if (data_to_send) break;

      coms_state_ = StepCommState::SendSync;
      // fall through

    //////////////////////////////////////////////
    // Sending data sent by the background task.
    // In this state I also save the response from the
    // motor driver chips.
    //////////////////////////////////////////////
    case StepCommState::SendSync:

      for (int i = 0; i < total_motors_; i++) {
        // If we sent this motor driver chip a command from
        // this state last time, save the response in the same
        // buffer that the command came from.
        if (motor_[i].save_response_) {
          *motor_[i].cmd_ptr_++ = dma_buff_[i];
          if (--motor_[i].cmd_remain_ <= 0) motor_[i].cmd_ptr_ = nullptr;
        }

        // If this motor has an active command to send,
        // grab the next byte, otherwise send a Nop
        if (motor_[i].cmd_ptr_) {
          data_to_send = true;
          motor_[i].save_response_ = true;
          dma_buff_[i] = *motor_[i].cmd_ptr_;
        } else {
          motor_[i].save_response_ = false;
          dma_buff_[i] = static_cast<uint8_t>(StepMtrCmd::Nop);
        }
      }

      // If I didn't find anything to send, then set our
      // state to idle and return, I'm done.
      if (!data_to_send) {
        coms_state_ = StepCommState::Idle;
        return;
      }
  }

  //////////////////////////////////////////////
  // I've got a message to send out to the chain
  // of motor driver chips.  Set up my DMA to
  // send it out.
  //////////////////////////////////////////////
  int c3 = static_cast<int>(DMA::Channel::Chan3);
  int c4 = static_cast<int>(DMA::Channel::Chan4);

  DmaReg *dma = DMA::get_register(DMA::Base::DMA2);
  dma->channel[c3].config.enable = 0;
  dma->channel[c4].config.enable = 0;

  dma->channel[c3].count = total_motors_;
  dma->channel[c4].count = total_motors_;
  dma->channel[c3].memory_address = dma_buff_;
  dma->channel[c4].memory_address = dma_buff_;

  // NOTE - CS has to be high for at least 650ns between bytes.
  // I don't bother timing this because I've found that in
  // practice it takes longer than that to handle the interrupt
  chip_select_low();

  dma->channel[c3].config.enable = 1;
  dma->channel[c4].config.enable = 1;
}

void StepMotor::DmaISR() {
  chip_select_high();

  // Clear the DMA interrupt
  DMA::ClearInt(DMA::Base::DMA2, DMA::Channel::Chan3, DMA::Interrupt::Global);

  UpdateComState();
}

// This is called from the HAL at the end of the high
// priority loop timer ISR.  If the comm state machine
// is idle it starts a new transmission
void StepMotor::StartQueuedCommands() {
  if (coms_state_ == StepCommState::Idle) UpdateComState();
}

// This is used to send a command to the stepper chips during
// startup.
void StepMotor::SendInitCmd(uint8_t *buff, int len) {
  int c3 = static_cast<int>(DMA::Channel::Chan3);
  int c4 = static_cast<int>(DMA::Channel::Chan4);

  DmaReg *dma = DMA::get_register(DMA::Base::DMA2);
  dma->channel[c3].config.enable = 0;
  dma->channel[c4].config.enable = 0;

  dma->channel[c3].count = len;
  dma->channel[c4].count = len;
  dma->channel[c3].memory_address = buff;
  dma->channel[c4].memory_address = buff;

  chip_select_low();

  // I prevent interrupts during this because I don't
  // want the normal interrupt handler to run.
  // The normal interrupt handler is designed to be used
  // with normal commands after startup and I don't want
  // to add the additional logic to it to handle these
  // unusual startup commands.
  BlockInterrupts block;
  dma->channel[c3].config.enable = 1;
  dma->channel[c4].config.enable = 1;
  while (!dma->interrupt_status.tcif3) {
  }

  // Clear the interrupt flag so I won't get an interrupt
  // as soon as I re-enable them
  DMA::ClearInt(DMA::Base::DMA2, DMA::Channel::Chan3, DMA::Interrupt::Global);

  // Raise the chip select line and wait 1 microsecond.
  // The minimum time the CS needs to be high is just under
  // 1 microsecond.
  chip_select_high();
  SystemTimer::singleton().delay(microseconds(1));
}

// This is run at startup before the DMA interrupts are enabled.
// It checks to determine how many stepper driver chips are
// present in the system.  Once the hardware stabilizes enough
// for everyone to have the same number of chips we can remove
// this, but it's convenient for right now.
void StepMotor::ProbeChips() {
  // I use a buffer a bit larger than the max number of motors
  // to try to continue to work even if there are a few more
  // driver chips than I support on the bus.
  uint8_t probe_buff[MaxMotors + 4];

  // First, send NO OP commands to all the chips a few times.
  // This will flush any data being sent by chips that were in
  // the middle of a command when the controller was restarted.
  for (int i = 0; i < 5; i++) {
    memset(probe_buff, static_cast<uint8_t>(StepMtrCmd::Nop), sizeof(probe_buff));
    SendInitCmd(probe_buff, sizeof(probe_buff));
  }

  // Now send a reset to all the chips on the bus.
  memset(probe_buff, static_cast<uint8_t>(StepMtrCmd::ResetDevice), sizeof(probe_buff));
  SendInitCmd(probe_buff, sizeof(probe_buff));

  // The first N bytes of the returned array should be 0 and the rest should
  // be the reset command.
  total_motors_ = 0;
  for (unsigned char i : probe_buff) {
    if (i == 0x00)
      total_motors_++;
    else
      break;
  }

  // If all the bytes in the buffer were zero, then most likely there is
  // nothing connected at all.
  if (total_motors_ == sizeof(probe_buff)) total_motors_ = 0;
}

#else

/// \TODO: improve this mocking to be helpful in testing

StepMtrErr StepMotor::HardDisable() { return StepMtrErr::Ok; }
StepMtrErr StepMotor::SetAmpAll(float amp) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::SetMaxSpeed(float dps) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::SetAccel(float acc) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::MoveRel(float deg) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::ClearPosition() { return StepMtrErr::Ok; }
StepMtrErr StepMotor::GotoPos(float deg) { return StepMtrErr::Ok; }
StepMtrErr StepMotor::HardStop() { return StepMtrErr::Ok; }

#endif
