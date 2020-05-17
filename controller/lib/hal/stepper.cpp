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

// Static data members
StepMotor StepMotor::motor[StepMotor::totalMotors];
uint8_t StepMotor::dmaBuff[StepMotor::totalMotors];
bool StepMotor::comsActive;

// This array holds the length of each parameter in units of
// bytes, rounded up to the nearest byte.  This info is based
// on table 12 in the powerSTEP chip data sheet
uint8_t StepMotor::paramLen[32] = {
    0, // 0x00 - No valid parameter with ID 0
    3, // 0x01 - Absolute position (22 bits)
    2, // 0x02 - Electrical position 9
    3, // 0x03 - Mark position 22
    3, // 0x04 - Current speed 20
    2, // 0x05 - Acceleration 12
    2, // 0x06 - Deceleration 12
    2, // 0x07 - Maximum speed 10
    2, // 0x08 - Minimum speed 12
    1, // 0x09 - KVAL_HOLD Holding K VAL 8
    1, // 0x0A - KVAL_RUN Constant speed K VAL 8
    1, // 0x0B - KVAL_ACC Acceleration starting K VAL 8
    1, // 0x0C - KVAL_DEC Deceleration starting K VAL 8
    2, // 0x0D - INT_SPEED Intersect speed 14
    1, // 0x0E - ST_SLP Start slope 8
    1, // 0x0F - FN_SLP_ACC Acceleration final slope 8
    1, // 0x10 - FN_SLP_DEC Deceleration final slope 8
    1, // 0x11 - K_THERM Thermal compensation factor 4
    1, // 0x12 - ADC output 5
    1, // 0x13 - OCD threshold 5
    1, // 0x14 - STALL_TH STALL threshold 5
    2, // 0x15 - Full-step speed 11
    1, // 0x16 - Step mode 8
    1, // 0x17 - Alarm enables 8
    2, // 0x18 - Gate driver configuration 11
    1, // 0x19 - Gate driver configuration 8
    2, // 0x1A - IC configuration 16
    2, // 0x1B - Status 16
    0, // 0x1C - No such parameter
    0, // 0x1D - No such parameter
    0, // 0x1E - No such parameter
    0, // 0x1F - No such parameter
};

#include "vars.h"
int32_t testVar;
DebugVar varTest("test", &testVar, "For testing stepper", "0x%08x");

// These functions raise and lower the chip select pin
static inline void CS_High() { GPIO_SetPin(GPIO_B_BASE, 6); }
static inline void CS_Low() { GPIO_ClrPin(GPIO_B_BASE, 6); }

StepMotor::StepMotor() {}

StepMtrErr StepMotor::SetParam(StepMtrParam param, uint32_t value) {
  uint8_t P = static_cast<uint8_t>(param);
  if (P > sizeof(paramLen))
    return StepMtrErr::BAD_PARAM;

  int len = paramLen[P];
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
  if (P > sizeof(paramLen))
    return StepMtrErr::BAD_PARAM;

  int len = paramLen[P];
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
  EnableClock(DMA1_BASE);

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

  // DMA1 channels 2 and 3 can be used to handle rx and tx interrupts from
  // SPI1 respectively.
  DMA_SelectChannel(DMA1_BASE, DMA_Chan::C2, 1);
  DMA_SelectChannel(DMA1_BASE, DMA_Chan::C3, 1);

  // The two DMA channels move data to/from the SPI data register.
  DMA_Reg *dma = DMA1_BASE;
  int C2 = static_cast<int>(DMA_Chan::C2);
  int C3 = static_cast<int>(DMA_Chan::C3);
  dma->channel[C2].pAddr = &spi->data;
  dma->channel[C3].pAddr = &spi->data;

  Hal.EnableInterrupt(INT_VEC_DMA1_CH2, IntPriority::STANDARD);
}

void test() {
  if (testVar) {

    StepMotor *mtr = StepMotor::GetStepper(0);

    uint32_t x;
    mtr->GetParam(static_cast<StepMtrParam>(testVar), &x);
    testVar = 0;
  }
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
    cmdPtr = cmd;
    cmdRemain = len;
    active = comsActive;
  }

  // If there wasn't already a command being sent,
  // start a new one.
  if (!active)
    StartCmd();

  // Wait for the ISR to finish sending the command.
  // When it does, it will set the pointer to NULL
  while (cmdPtr) {
  }

  return StepMtrErr::OK;
}

// Start transmitting a command.
// This is called when a new command is being sent and also
// from the ISR to send the next byte.
// If sent from the ISR, then the isr parameter will be true
void StepMotor::StartCmd() {

  // Run through all the motors and if the motor was active on
  // that last byte, add the byte we received from them
  // into their receive buffer.
  //
  // Also, keep track of which ones have more data to send.
  // For those that don't, I'll just send a NOP command.
  comsActive = false;
  for (int i = 0; i < totalMotors; i++) {

    // If motor[i].sentByte is true, it means we
    // sent a byte to this motor driver last cycle.
    // In that case, store the response to the
    // motor's buffer.

    if (motor[i].sentByte && motor[i].cmdPtr) {
      *motor[i].cmdPtr++ = dmaBuff[i];
      if (--motor[i].cmdRemain <= 0)
        motor[i].cmdPtr = 0;
    }

    // If this motor has an active command to send,
    // grab the next byte and keep track of that in
    // my bitmask
    if (motor[i].cmdPtr) {
      motor[i].sentByte = true;
      comsActive = true;
      dmaBuff[i] = *motor[i].cmdPtr;
    } else {
      motor[i].sentByte = false;
      dmaBuff[i] = static_cast<uint8_t>(StepMtrCmd::NOP);
    }
  }

  // If there's nothing else to send, I'm done.
  if (!comsActive)
    return;

  int C2 = static_cast<int>(DMA_Chan::C2);
  int C3 = static_cast<int>(DMA_Chan::C3);

  DMA_Reg *dma = DMA1_BASE;
  dma->channel[C2].config = 0;
  dma->channel[C3].config = 0;

  dma->channel[C2].count = totalMotors;
  dma->channel[C3].count = totalMotors;
  dma->channel[C2].mAddr = dmaBuff;
  dma->channel[C3].mAddr = dmaBuff;

  // NOTE - CS has to be high for at least 650ns between bytes.
  // I don't bother timing this because I've found that in
  // practice it takes longer then that to handle the interrupt
  CS_Low();

  int memoryInc = 1 << 7;   // Increment memory address
  int dir_P2M = 0 << 4;     // Transfer from peripherial to memory
  int xferDoneInt = 1 << 1; // Transfer complete interrupt enable
  int chanEnable = 1 << 0;  // Transfer complete interrupt enable

  dma->channel[C2].config = memoryInc | dir_P2M | xferDoneInt | chanEnable;

  int dir_M2P = 1 << 4;
  dma->channel[C3].config = memoryInc | dir_M2P | chanEnable;
}

void StepMotor::DMA_ISR() {
  CS_High();

  // Clear the DMA interrupt
  DMA1_BASE->intClr = 1 << (4 * static_cast<int>(DMA_Chan::C2));

  StartCmd();
}

#endif
