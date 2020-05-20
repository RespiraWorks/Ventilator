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

// Static data members
StepMotor StepMotor::motor_[StepMotor::total_motors_];
uint8_t StepMotor::dma_buff_[StepMotor::total_motors_];
bool StepMotor::coms_active_;

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

// Notes to get the stepper working
// Gate current 96mA
// VCC Value 15V
// UVLO threshold 7V
// Turn OFF boost time 1000ns
// Controlled current time 3750ns
// Blanking time 1000ns
// Dead time 1000ns

int32_t setMtr, getMtr, cmd, mtrNdx;
uint32_t getVal, tmp2;
float mtrVel, mtrAcc, tmp1;
#define DEBUG_ENABLED
#ifdef DEBUG_ENABLED
#include "vars.h"
DebugVar v1("set_param", &setMtr, "For testing stepper", "0x%08x");
DebugVar v2("get_param", &getMtr, "For testing stepper", "0x%08x");
DebugVar v3("mtr_value", &getVal, "For testing stepper", "0x%08x");
DebugVar v4("mtr_cmd", &cmd, "For testing stepper", "0x%08x");
DebugVar v5("tmp2", &tmp2, "For testing stepper", "0x%08x");
DebugVar v6("mtr_ndx", &mtrNdx, "For testing stepper", "%d");
DebugVar v7("mtr_vel", &mtrVel, "For testing stepper", "%.3f");
DebugVar v8("mtr_tmp", &tmp1, "For testing stepper", "%.5e");
DebugVar v9("mtr_acc", &mtrAcc, "For testing stepper", "%.3f");
#endif

void test() {
  StepMotor *mtr = StepMotor::GetStepper(mtrNdx);
  if (!mtr)
    return;

  if (setMtr) {
    mtr->SetParam(static_cast<StepMtrParam>(setMtr), getVal);
    setMtr = 0;
  }

  if (getMtr) {
    mtr->GetParam(static_cast<StepMtrParam>(getMtr), &getVal);
    getMtr = 0;

    tmp1 = mtr->GetCurrentSpeed();
  }

  if (cmd) {
    mtr->SendCmd(reinterpret_cast<uint8_t *>(&cmd), 4);
    cmd = 0;
  }

  if (mtrVel) {
    mtr->RunAtVelocity(mtrVel);
    mtrVel = 0;
  }

  if (mtrAcc) {
    mtr->SetAccel(mtrAcc);
    mtrAcc = 0;
  }
}

// These functions raise and lower the chip select pin
static inline void CS_High() { GPIO_SetPin(GPIO_B_BASE, 6); }
static inline void CS_Low() { GPIO_ClrPin(GPIO_B_BASE, 6); }

StepMotor::StepMotor() {
  cmd_ptr_ = 0;
  cmd_remain_ = 0;
  sent_byte_ = false;

  steps_per_rev_ = 200;
}

StepMtrErr StepMotor::SetParam(StepMtrParam param, uint32_t value) {
  uint8_t P = static_cast<uint8_t>(param);
  if (P > sizeof(param_len_))
    return StepMtrErr::BAD_PARAM;

  int len = param_len_[P];
  if (!len || (len > 3))
    return StepMtrErr::BAD_PARAM;

  uint8_t cmdBuff[4];
  cmdBuff[0] = P; // The op-code for a set is just the parameter number

  // Split the parameter value into bytes, MSB first
  value <<= 8 * (3 - len);
  cmdBuff[1] = static_cast<uint8_t>(value >> 16);
  cmdBuff[2] = static_cast<uint8_t>(value >> 8);
  cmdBuff[3] = static_cast<uint8_t>(value);

  // The op-code for a set is equal to the parameter number
  return SendCmd(cmdBuff, len + 1);
}

StepMtrErr StepMotor::GetParam(StepMtrParam param, uint32_t *value) {
  uint8_t P = static_cast<uint8_t>(param);
  if (P > sizeof(param_len_))
    return StepMtrErr::BAD_PARAM;

  int len = param_len_[P];
  if (!len || (len > 3))
    return StepMtrErr::BAD_PARAM;

  uint8_t cmdBuff[4];

  // For a get, the op-code is the parameter | 0x20
  cmdBuff[0] = static_cast<uint8_t>(P | 0x20);
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
  const int rxFifo8Bit = 1 << 12; // Rx int on every byte
  const int dataSize = 7 << 8;    // 8-bit data words
  const int txDmaEna = 1 << 1;
  const int rxDmaEna = 1 << 0;
  spi->ctrl[1] = rxFifo8Bit | dataSize | txDmaEna | rxDmaEna;

  // Configure for master mode, CPOL and CPHA both 0.
  // The stepper driver chip has a max SPI clock
  // rate of 5MHz which is the rate I'll use.
  const int cpha = 1 << 0; // Data is sampled on the rising edge of the clock
  const int cpol = 1 << 1; // Clock line is high when not active
  const int masterMode = 1 << 2; // We're acting as the master on the SPI bus
  const int baudRate = 3 << 3;   // Set bit rate for 80MHz/16 or 5MHz
  const int spiEnable = 1 << 6;  // Enable the SPI module
  const int internalSS =
      1 << 8; // These two settings allow us to manually control the SS pin
  const int softwareSS = 1 << 9;
  spi->ctrl[0] =
      cpha | cpol | masterMode | baudRate | spiEnable | internalSS | softwareSS;

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
  dma->channel[C3].config.dir = DmaChannelDir::PERIPHERAL_TO_MEM;
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
  dma->channel[C4].config.dir = DmaChannelDir::MEM_TO_PERIPHERAL;
  dma->channel[C4].config.circular = 0;
  dma->channel[C4].config.perInc = 0;
  dma->channel[C4].config.memInc = 1;
  dma->channel[C4].config.psize = 0;
  dma->channel[C4].config.msize = 0;
  dma->channel[C4].config.priority = 0;
  dma->channel[C4].config.mem2mem = 0;

  Hal.EnableInterrupt(INT_VEC_DMA2_CH3, IntPriority::STANDARD);
}

// Read the motor status register (blocking) and return it
StepMtrErr StepMotor::GetStatus(uint16_t *stat) {
  uint8_t cmd[] = {static_cast<uint8_t>(StepMtrCmd::GET_STATUS), 0, 0};

  StepMtrErr err = SendCmd(cmd, 3);
  uint16_t h = cmd[1];
  uint16_t l = cmd[2];
  *stat = static_cast<uint16_t>((h << 8) | l);
  return err;
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
StepMtrErr StepMotor::SendCmd(uint8_t *cmd, int len) {

  // Copy the command to my buffer with interrupts disabled.
  // I want to make sure the whole command gets sent as one continuous
  // stream and it's possible that commands are currently being sent
  // to other motors.
  bool active;
  {
    BlockInterrupts block;
    cmd_ptr_ = cmd;
    cmd_remain_ = len;
    active = coms_active_;
  }

  // If there wasn't already a command being sent,
  // start a new one.
  if (!active)
    StartCmd();

  // Wait for the ISR to finish sending the command.
  // When it does, it will set the pointer to NULL
  while (cmd_ptr_) {
  }

  return StepMtrErr::OK;
}

// Start transmitting a command.
// This is called when a new command is being sent and also
// from the ISR to send the next byte.
void StepMotor::StartCmd() {

  // Run through all the motors and if the motor was active on
  // that last byte, add the byte we received from them
  // into their receive buffer.
  //
  // Also, keep track of which ones have more data to send.
  // For those that don't, I'll just send a NOP command.
  coms_active_ = false;
  for (int i = 0; i < total_motors_; i++) {

    // If motor[i].sent_byte is true, it means we
    // sent a byte to this motor driver last cycle.
    // In that case, store the response to the
    // motor's buffer.

    if (motor_[i].sent_byte_ && motor_[i].cmd_ptr_) {
      *motor_[i].cmd_ptr_++ = dma_buff_[i];
      if (--motor_[i].cmd_remain_ <= 0)
        motor_[i].cmd_ptr_ = 0;
    }

    // If this motor has an active command to send,
    // grab the next byte and keep track of that in
    // my bitmask
    if (motor_[i].cmd_ptr_) {
      motor_[i].sent_byte_ = true;
      coms_active_ = true;
      dma_buff_[i] = *motor_[i].cmd_ptr_;
    } else {
      motor_[i].sent_byte_ = false;
      dma_buff_[i] = static_cast<uint8_t>(StepMtrCmd::NOP);
    }
  }

  // If there's nothing else to send, I'm done.
  if (!coms_active_)
    return;

  int C3 = static_cast<int>(DMA_Chan::C3);
  int C4 = static_cast<int>(DMA_Chan::C4);

  DMA_Regs *dma = DMA2_BASE;
  dma->channel[C3].config.enable = 0;
  dma->channel[C4].config.enable = 0;

  dma->channel[C3].count = total_motors_;
  dma->channel[C4].count = total_motors_;
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

  StartCmd();
}

// Convert a velocity from Deg/sec units to the value to program
// into one of the stepper controller registers
float StepMotor::DpsToVelReg(float vel, float cnv) {

  // Convert to steps / sec
  float step_per_sec = vel * static_cast<float>(steps_per_rev_) / 360.0f;

  // Multiply that by the register specific conversion factor
  return cnv * step_per_sec;
}

// Like above, but reversed
float StepMotor::RegVelToDps(int32_t val, float cnv) {
  return static_cast<float>(val) * 360.0f /
         (cnv * static_cast<float>(steps_per_rev_));
}

// Start running at a constant velocity.
// The velocity is specified in deg/sec units
void StepMotor::RunAtVelocity(float vel) {

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
  SendCmd(cmd, 4);
}

// Read the current absolute motor velocity and return it
// in deg/sec units
// Note that this value is always positive
float StepMotor::GetCurrentSpeed() {
  uint32_t val;
  StepMotor::GetParam(StepMtrParam::SPEED, &val);
  return RegVelToDps(val, cvt_vel_crnt_speed_reg);
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
float StepMotor::GetMaxSpeed() {
  uint32_t val;
  StepMotor::GetParam(StepMtrParam::MAX_SPEED, &val);
  return RegVelToDps(val, cvt_vel_max_speed_reg);
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
float StepMotor::GetMinSpeed() {
  uint32_t val;
  StepMotor::GetParam(StepMtrParam::MIN_SPEED, &val);
  return RegVelToDps(val, cvt_vel_min_speed_reg);
}

// Set the motors accel and decel rate in deg/sec/sec units
//
// NOTE - The motor must be disabled to set this
StepMtrErr StepMotor::SetAccel(float acc) {

  static const float scaler = (tick_time * tick_time * 1099511627776.f);
  if (acc < 0)
    return StepMtrErr::BAD_VALUE;
  tmp1 = scaler;

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

/*
  KVAL_HOLD = 0x09,         //
  KVAL_RUN = 0x0A,          //
  KVAL_ACCEL = 0x0B,        //
  KVAL_DECEL = 0x0C,        //
  INT_SPEED = 0x0D,         //
  START_SLOPE = 0x0E,       //
  FINAL_SLOPE_ACCEL = 0x0F, //
  FINAL_SLOPE_DECEL = 0x10, //
  THERMAL_COMP = 0x11,      //
  OVER_CRNT_THRESH = 0x13,  //
  STALL_THRESH = 0x14,      //
  FULL_STEP_SPEED = 0x15,   // Speed at which to switch to full step mode
  */
#endif
