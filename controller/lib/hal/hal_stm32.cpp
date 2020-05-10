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

#if defined(BARE_STM32)

#include "hal_stm32.h"
#include "circular_buffer.h"
#include "hal.h"

#define SYSTEM_STACK_SIZE 2500

// This is the main stack used in our system.
__attribute__((aligned(8))) uint32_t systemStack[SYSTEM_STACK_SIZE];

// local data
static volatile int64_t msCount;

// local static functions.  I don't want to add any private
// functions to the Hal class to avoid complexity with other
// builds
static void InitGPIO();
static void InitADC();
static void InitSysTimer();
static void InitPwmOut();
static void InitUARTs();
static void EnableClock(void *ptr);
static void EnableInterrupt(int addr, int pri);
static void Timer6ISR();
static void UART3_ISR();

// For now, the main function in main.cpp is called setup
// rather then main.  If we adopt this HAL then we can
// just rename it main and get rid of the following function.
extern void setup();
int main() { setup(); }

// This function is called from the libc initialization code
// before any static constructors are called.  We do some basic
// chip initialization here.
//
// The main things done here are to enable the FPU because if
// we don't do that then we'll get a fatal exception if any
// constructor uses any floating point math, and to enable
// the PLL so we can run at full speed (80MHz) rather then the
// default speed of 4MHz.
extern "C" void _init() {

  // Enable the FPU.  This allows floating point to be used without
  // generating a hard fault.
  // The system control registers are documented in the programmers
  // manual (not the reference manual) chapter 4.
  // Details on enabling the FPU are in section 4.6.6.
  SysCtrl_Reg *sysCtl = SYSCTL_BASE;
  sysCtl->cpac = 0x00F00000;

  // Reset caches and set latency for 80MHz opperation
  // See chapter 3 of the reference manual for details
  // on the embedded flash module
  EnableClock(FLASH_BASE);
  FlashReg *flash = FLASH_BASE;
  flash->access = 0x00000004;
  flash->access = 0x00001804;
  flash->access = 0x00001804;
  flash->access = 0x00000604;

  // Enable the PLL.
  // We use the MSI clock as the source for the PLL
  // The MSI clock is running at its default frequency of
  // 4MHz.
  //
  // The PLL can generate several clocks with somewhat
  // less then descriptive names in the reference manual.
  // These clocks are:
  //   P clock - Used for the SAI peripherial.  Not used here
  //   Q clock - 48MHz output clock used for USB.  Not used here.
  //   R clock - This is the main system clock.  We care about this one.
  //
  // When configuring the PLL there are several constants programmed
  // into the PLL register to set the frequency of the internal VCO
  // These constants are called N and M in the reference manual:
  //
  // Fin = 4MHz
  // Fvco = Fin * (N/M)
  //
  // Legal range for Fvco is 96MHz to 344MHz according to the
  // data sheet.  I'll use 160MHz for Fvco and divide by 2
  // to get an 80MHz output clock
  //
  // See chapter 6 of the reference manual
  int N = 40;
  int M = 1;
  RCC_Regs *rcc = RCC_BASE;
  rcc->pllCfg = 0x01000001 | (N << 8) | ((M - 1) << 4);

  // Turn on the PLL
  rcc->clkCtrl |= 0x01000000;

  // Wait for the PLL ready indication
  while (!(rcc->clkCtrl & 0x02000000)) {
  }

  // Set PLL as system clock
  rcc->clkCfg = 0x00000003;

  // Use system clock as the A/D clock
  rcc->indClkCfg = 0x30000000;
}

/*
 * One time init of HAL.
 */
void HalApi::init() {

  // Init various components needed by the system.
  InitGPIO();
  InitSysTimer();
  InitADC();
  InitPwmOut();
  InitUARTs();
  watchdog_init();

  // Enable interrupts
  Hal.IntEnable();
}

// Reset the processor
[[noreturn]] void HalApi::reset_device() {

  // Note that the system control registers are a standard ARM peripherial
  // they aren't documented in the normal STM32 reference manual, rather
  // they're in the processor programming manual.
  // The register we use to reset the system is called the
  // "Application interrupt and reset control register (AIRCR)"
  SysCtrl_Reg *sysCtl = SYSCTL_BASE;
  sysCtl->apInt = 0x05FA0004;

  // We promised we wouldn't return, so...
  while (true) {
  }
}

/******************************************************************
 * General Purpose I/O support.
 *
 * The following pins are used as GPIO on the rev-1 PCB
 *
 * Please refer to the PCB schematic as the ultimate source of which
 * pin is used for which function.  A less definitive, but perhaps
 * easier to read spreadsheet is availabe here:
 * https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit#gid=0
 *
 * ID inputs.  These can be used to identify the PCB revision
 * we're running on.
 *  PB1  - ID0
 *  PA12 - ID1
 *
 * LED outputs.
 *  PC13 - red
 *  PC14 - yellow
 *  PC15 - green
 *
 * Solenoid
 *  PA11 - Note, this is also a timer pin so we may want to
 *         PWM it to reduce the solenoid voltage.
 *         For no I'm treating it as a digital output.
 *****************************************************************/
static void InitGPIO() {
  // See chapter 8 of the reference manual for details on GPIO

  // Enable all the GPIO clocks
  EnableClock(GPIO_A_BASE);
  EnableClock(GPIO_B_BASE);
  EnableClock(GPIO_C_BASE);
  EnableClock(GPIO_D_BASE);
  EnableClock(GPIO_E_BASE);
  EnableClock(GPIO_H_BASE);

  // Configure PCB ID pins as inputs.
  GPIO_PinMode(GPIO_B_BASE, 1, GPIO_PinMode::IN);
  GPIO_PinMode(GPIO_A_BASE, 12, GPIO_PinMode::IN);

  // Configure LED pins as outputs
  GPIO_PinMode(GPIO_C_BASE, 13, GPIO_PinMode::OUT);
  GPIO_PinMode(GPIO_C_BASE, 14, GPIO_PinMode::OUT);
  GPIO_PinMode(GPIO_C_BASE, 15, GPIO_PinMode::OUT);

  // Turn all three LEDs off initially
  GPIO_ClrPin(GPIO_C_BASE, 13);
  GPIO_ClrPin(GPIO_C_BASE, 14);
  GPIO_ClrPin(GPIO_C_BASE, 15);

  // Configure the solenoid and turn it off
  GPIO_PinMode(GPIO_A_BASE, 11, GPIO_PinMode::OUT);
  GPIO_ClrPin(GPIO_A_BASE, 11);
}

// Set or clear the specified digital output
void HalApi::digitalWrite(BinaryPin pin, VoltageLevel value) {
  GPIO_Regs *base;
  int bit;

  switch (pin) {
  case BinaryPin::SOLENOID:
    base = GPIO_A_BASE;
    bit = 11;
    break;
  }

  switch (value) {
  case VoltageLevel::HAL_HIGH:
    GPIO_SetPin(base, bit);
    break;

  case VoltageLevel::HAL_LOW:
    GPIO_ClrPin(base, bit);
    break;
  }
}

/******************************************************************
 * System timer
 *
 * I use one of the basic timers (timer 6) for general system timing.
 * I configure it to count every microsecond and generate an interrupt
 * every millisecond
 *
 * The basic timers (like timer 6) are documented in chapter 29 of
 * the reference manual
 *****************************************************************/
static void InitSysTimer() {

  // Enable the clock to the timer
  EnableClock(TIMER6_BASE);

  // Just set the timer up to count every microsecond.
  TimerRegs *tmr = TIMER6_BASE;
  tmr->reload = 999;
  tmr->prescale = (CPU_FREQ_MHZ - 1);
  tmr->event = 1;
  tmr->ctrl[0] = 1;
  tmr->intEna = 1;

  EnableInterrupt(INT_VEC_TIMER6, 3);
}

// Just spin for a specified number of microseconds
static void BusyWaitUsec(uint16_t usec) {
  constexpr uint16_t one_sec = 1000;
  while (usec > one_sec) {
    BusyWaitUsec(one_sec);
    usec = static_cast<uint16_t>(usec - one_sec);
  }

  TimerRegs *tmr = TIMER6_BASE;
  uint16_t start = static_cast<uint16_t>(tmr->counter);
  while (1) {
    uint16_t dt = static_cast<uint16_t>(tmr->counter - start);
    if (dt >= usec)
      return;
  }
}

static void Timer6ISR() {
  TIMER6_BASE->status = 0;
  msCount++;
}

void HalApi::delay(Duration d) {
  int64_t start = msCount;
  while (msCount - start < d.milliseconds()) {
  }
}

Time HalApi::now() { return millisSinceStartup(msCount); }

/******************************************************************
 * A/D inputs.
 *
 * The following pins are used as analog inputs on the rev-1 PCB
 *
 * Please refer to the PCB schematic as the ultimate source of which
 * pin is used for which function.  A less definitive, but perhaps
 * easier to read spreadsheet is availabe here:
 * https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit#gid=0
 *
 * PA0 (ADC1_IN5)  - vin
 * PA1 (ADC1_IN6)  - pressure
 * PA4 (ADC1_IN9)  - inhale flow
 * PB0 (ADC1_IN15) - exhale flow
 *
 * See chapter 16 of the reference manual
 *
 *****************************************************************/
static void InitADC() {
  // Enable the clock to the A/D converter
  EnableClock(ADC_BASE);

  // Configure the 4 pins used as analog inputs
  GPIO_PinMode(GPIO_A_BASE, 0, GPIO_PinMode::ANALOG);
  GPIO_PinMode(GPIO_A_BASE, 1, GPIO_PinMode::ANALOG);
  GPIO_PinMode(GPIO_A_BASE, 4, GPIO_PinMode::ANALOG);
  GPIO_PinMode(GPIO_B_BASE, 0, GPIO_PinMode::ANALOG);

  // Perform a power-up and calibration sequence on
  // the A/D converter
  ADC_Regs *adc = ADC_BASE;

  // Exit deep power down mode and turn on the
  // internal voltage regulator.
  adc->adc[0].ctrl = 0x10000000;

  // Wait for the startup time specified in the datasheet
  // for the voltage regulator to become ready.
  // The time in the datasheet is 20 microseconds, but
  // I'll wait for 30 just to be extra conservative
  BusyWaitUsec(30);

  // Calibrate the A/D for single ended channels
  adc->adc[0].ctrl |= 0x80000000;

  // Wait until the CAL bit is cleared meaning
  // calibration is finished
  while (adc->adc[0].ctrl & 0x80000000) {
  }

  // Clear all the status bits
  adc->adc[0].stat = 0x3FF;

  // Enable the A/D
  adc->adc[0].ctrl |= 0x00000001;

  // Wait for the ADRDY status bit to be set
  while (!(adc->adc[0].stat & 0x00000001)) {
  }

  // Configure the A/D as 12-bit resolution
  adc->adc[0].cfg[0] = 0x00000000;

  // Set sample time. I'm using 92.5 A/D clocks (a little over 1us)
  // to sample.  We'll need to do a bit of testing to see what the
  // best value is for this
  adc->adc[0].samp[0] = 5;
}

// Read a single A/D channel.
// This just does the conversion immediately when called.
//
// It would be better to start a sequence of conversions at the beginning of the
// loop but the HAL interface currently isn't set up to run that way.  That's an
// improvement that can be made later
int HalApi::analogRead(AnalogPin pin) {

  int channel = 0;
  switch (pin) {
  case AnalogPin::PATIENT_PRESSURE:
    channel = 6;
    break;

  case AnalogPin::INFLOW_PRESSURE_DIFF:
    channel = 9;
    break;

  case AnalogPin::OUTFLOW_PRESSURE_DIFF:
    channel = 15;
    break;
  }

  ADC_Regs *adc = ADC_BASE;
  adc->adc[0].seq[0] = channel << 6;

  // Clear the EOC flag
  adc->adc[0].stat = 4;

  // Start the conversion
  adc->adc[0].ctrl |= 4;

  // Wait for the end of the conversion
  while (!(adc->adc[0].stat & 4)) {
  }

  // Return the result
  return adc->adc[0].data;
}

/******************************************************************
 * PWM outputs
 *
 * The following four outputs could be driven
 * as PWM outputs:
 *
 * PA8  - Timer 1 Channel 1 - heater control
 * PA11 - Timer 1 Channel 4 - solenoid
 * PB3  - Timer 2 Channel 2 - blower control
 * PB4  - Timer 3 Channel 1 - buzzer
 *
 * For now I'll just set up the blower since that's the only
 * one called out in the HAL
 *
 * These timers are documented in chapters 26 and 27 of the reference
 * manual.
 *****************************************************************/
static void InitPwmOut() {
  // The PWM frequency isn't mentioned anywhere that I can find, so
  // I'm just picking a reasonable number.  This can be refined later
  //
  // The selection of PWM frequency is a trade off between latency and
  // resolution.  Higher frequencies give lower latency and lower resoution.
  //
  // Latency is the time between setting the value and it taking effect,
  // this is essentially the PWM period (1/frequency).  For example, a
  // 20kHz frequency would give a latency of up to 50 usec.
  //
  // Resultion is based on the ratio of the clock frequency (80MHz) to the
  // PWM frequency.  For example, a 20kHz PWM would have a resolution of one
  // part in 4000 (80000000/20000) or about 12 bits.
  const int pwmFreqHz = 20000;

  EnableClock(TIMER2_BASE);

  // Connect PB3 to timer 2
  GPIO_PinAltFunc(GPIO_B_BASE, 3, 1);

  TimerRegs *tmr = TIMER2_BASE;

  // Set the frequency
  tmr->reload = (CPU_FREQ / pwmFreqHz) - 1;

  // Configure channel 2 in PWM output mode 1
  // with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow
  // register and copied to the active register
  // at the start of the next cycle.
  tmr->ccMode[0] = 0x6800;

  tmr->ccEnable = 0x10;

  // Start with 0% duty cycle
  tmr->compare[1] = 0;

  // Load the shadow registers
  tmr->event = 1;

  // Start the counter
  tmr->ctrl[0] = 0x81;
}

// Set the PWM period.  Currently the value is 0 to 255 which is
// a shame.  We should change this to a float, say with a value
// of 0.0 to 1.0 to make better use of our resolution
void HalApi::analogWrite(PwmPin pin, int value) {

  // Convert the value to a float
  float duty = static_cast<float>(value) * (1.0f / 255);

  TimerRegs *tmr;
  int chan;
  switch (pin) {
  case PwmPin::BLOWER:
    tmr = TIMER2_BASE;
    chan = 1;
    break;
  }

  tmr->compare[chan] = static_cast<REG>(static_cast<float>(tmr->reload) * duty);
}

/******************************************************************
 * Serial port to GUI
 * Chapter 38 of the reference manual defines the USART registers.
 *****************************************************************/

class UART {
  CircBuff<128> rxDat;
  CircBuff<128> txDat;
  UART_Regs *const reg;

public:
  UART(UART_Regs *const r) : reg(r) {}

  void Init(int baud) {

    // Set baud rate register
    reg->baud = CPU_FREQ / baud;

    // Enable the UART and receive interrupts
    reg->ctrl[0] = 0x002D;
  }

  // This is the interrupt handler for the UART.
  void ISR() {

    // Check for over run error and framing errors.
    // Clear those errors if they're set to avoid
    // further interrupts from them.
    if (reg->status & 0x000A)
      reg->intClear = 0x000A;

    // See if we received a new byte
    if (reg->status & 0x0020)
      rxDat.Put(static_cast<uint8_t>(reg->rxDat));

    // Check for transmit data register empty
    if (reg->status & reg->ctrl[0] & 0x0080) {
      int ch = txDat.Get();

      // If there's nothing left in the transmit buffer,
      // just disable further transmit interrupts.
      if (ch < 0)
        reg->ctrl[0] &= ~0x0080;

      // Otherwise, Send the next byte
      else
        reg->txDat = ch;
    }
  }

  // Read up to len bytes and store them in the passed buffer.
  // This function does not block, so if less then len bytes
  // are available it will only return the available bytes
  // Returns the number of bytes actually read.
  uint16_t read(char *buf, uint16_t len) {

    for (uint16_t i = 0; i < len; i++) {
      int ch = rxDat.Get();
      if (ch < 0)
        return i;
      *buf++ = static_cast<uint8_t>(ch);
    }

    // Note that we don't need to enable the rx interrupt
    // here.  That one is always enabled.
    return len;
  }

  // Write up to len bytes to the buffer.
  // This function does not block, so if there isn't enough
  // space to write len bytes, then only a partial write
  // will occur.
  // The number of bytes actually written is returned.
  uint16_t write(const char *buf, uint16_t len) {

    uint16_t i;
    for (i = 0; i < len; i++) {
      if (!txDat.Put(*buf++))
        break;
    }

    // Enable the tx interrupt.  If there was already anything
    // in the buffer this will already be enabled, but enabling
    // it again doesn't hurt anything.
    reg->ctrl[0] |= 0x0080;

    return i;
  }

  // Return the number of bytes currently in the
  // receive buffer and ready to be read.
  uint16_t RxFull() { return rxDat.FullCt(); }

  // Returns the number of free locations in the
  // transmit buffer.
  uint16_t TxFree() { return txDat.FreeCt(); }
};

static UART rpUART(UART3_BASE);

// The UART that talks to the rPi uses the following pins:
//    PB10 - TX
//    PB11 - RX
//    PB13 - RTS
//    PB14 - CTS
//
// Please refer to the PCB schematic as the ultimate source of which
// pin is used for which function.  A less definitive, but perhaps
// easier to read spreadsheet is availabe here:
// https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit#gid=0
//
// These pins are connected to UART3
// The UART is described in chapter 38 of the reference manual
static void InitUARTs() {
  // NOTE - The UART functionality hasn't been tested due to lack of hardware!
  //        Need to do that as soon as the boards are available.
  EnableClock(UART3_BASE);

  GPIO_PinAltFunc(GPIO_B_BASE, 10, 7);
  GPIO_PinAltFunc(GPIO_B_BASE, 11, 7);
  GPIO_PinAltFunc(GPIO_B_BASE, 13, 7);
  GPIO_PinAltFunc(GPIO_B_BASE, 14, 7);

  rpUART.Init(115200);

  EnableInterrupt(INT_VEC_UART3, 3);
}

static void UART3_ISR() { rpUART.ISR(); }

uint16_t HalApi::serialRead(char *buf, uint16_t len) {
  return rpUART.read(buf, len);
}

uint16_t HalApi::serialBytesAvailableForRead() { return rpUART.RxFull(); }

uint16_t HalApi::serialWrite(const char *buf, uint16_t len) {
  return rpUART.write(buf, len);
}

uint16_t HalApi::serialBytesAvailableForWrite() { return rpUART.TxFree(); }

/******************************************************************
 * Watchdog timer (see chapter 32 of reference manual).
 *
 * The watchdog timer will reset the system if it hasn't been
 * re-initialized within a specific amount of time.  It's used
 * to catch bugs that would otherwise hang the system.  When
 * the watchdog is enabled such a bug will reset the system
 * rather then let it hang indefinitely.
 *****************************************************************/
void HalApi::watchdog_init() {
  Watchdog_Regs *wdog = WATCHDOG_BASE;

  // Enable the watchdog timer by writing the appropriate value to its key
  // register
  wdog->key = 0xCCCC;

  // Enable register access
  wdog->key = 0x5555;

  // Set the pre-scaler to 0.  That setting will cause the watchdog
  // clock to be updated at approximately 8KHz.
  wdog->prescale = 0;

  // The reload value gives the number of clock cycles before the
  // watchdog timer times out.  I'll set it to 2000 which gives
  // us about 250ms before a reset.
  wdog->reload = 2000;

  // Since the watchdog timer runs off its own clock which is pretty
  // slow, it takes a little time for the registers to actually get
  // updated.  I wait for the status register to go to zero which
  // means its done.
  while (wdog->status) {
  }

  // Reset the timer.  This also locks the registers again.
  wdog->key = 0xAAAA;
}

// Pet the watchdog so it doesn't bite us.
void HalApi::watchdog_handler() {
  Watchdog_Regs *wdog = WATCHDOG_BASE;
  wdog->key = 0xAAAA;
}

// Enable clocks to a specific peripherial.
// On the STM32 the clocks going to various peripherials on the chip
// are individually selectable and for the most part disabled on startup.
// Clocks to the specific peripherials need to be enabled through the
// RCC (Reset and Clock Controller) module before the peripherial can be
// used.
// Pass in the base address of the peripherial to enable its clock
static void EnableClock(void *ptr) {
  static struct {
    void *base;
    int ndx;
    int bit;
  } rccInfo[] = {
      {FLASH_BASE, 0, 8},  {GPIO_A_BASE, 1, 0}, {GPIO_B_BASE, 1, 1},
      {GPIO_C_BASE, 1, 2}, {GPIO_D_BASE, 1, 3}, {GPIO_E_BASE, 1, 4},
      {GPIO_H_BASE, 1, 7}, {ADC_BASE, 1, 13},   {TIMER2_BASE, 4, 0},
      {TIMER6_BASE, 4, 4}, {UART3_BASE, 4, 18},

      // The following entries are probably correct, but have
      // not been tested yet.  When adding support for one of
      // these peripherials just comment out the line.  And
      // test of course.
      //      {DMA1_BASE, 0, 0},
      //      {DMA2_BASE, 0, 1},
      //      {CRC_BASE, 0, 12},
      //      {TIMER3_BASE, 4, 1},
      //      {TIMER7_BASE, 4, 7},
      //      {SPI2_BASE, 4, 14},
      //      {SPI3_BASE, 4, 15},
      //      {UART2_BASE, 4, 17},
      //      {UART4_BASE, 4, 19},
      //      {I2C1_BASE, 4, 21},
      //      {I2C2_BASE, 4, 22},
      //      {I2C3_BASE, 4, 23},
      //      {I2C4_BASE, 5, 1},
      //      {TIMER1_BASE, 6, 11},
      //      {SPI1_BASE, 6, 12},
      //      {UART1_BASE, 6, 14},
      //      {TIMER15_BASE, 6, 16},
      //      {TIMER16_BASE, 6, 17},
  };

  // I don't include all the peripherials here, just the ones
  // that we currently use or seem likely to be used in the
  // future.  To add more peripherials, just look up the appropriate
  // bit in the reference manual RCC chapter.
  int ndx = -1;
  int bit = 0;
  for (uint32_t i = 0; i < sizeof(rccInfo) / sizeof(rccInfo[0]); i++) {
    if (ptr == rccInfo[i].base) {
      ndx = rccInfo[i].ndx;
      bit = rccInfo[i].bit;
      break;
    }
  }

  // If the input address wasn't found then its definitly
  // a bug.  I'll just loop forever here causing the code
  // to crash.  That should make it easier to find the
  // bug during development.
  if (ndx < 0) {
    Hal.IntDisable();
    while (1) {
    }
  }

  // Enable the clock of the requested peripherial
  RCC_Regs *rcc = RCC_BASE;
  rcc->periphClkEna[ndx] |= (1 << bit);
}

/******************************************************************
 * Interrupt vector table.  The interrupt vector table is a list of
 * pointers to the various interrupt functions.  It is stored at the
 * very start of the flash memory.
 *****************************************************************/

// Fault handlers
static void fault() {
  while (1) {
  }
}

static void NMI() { fault(); }
static void FaultISR() { fault(); }
static void MPUFaultISR() { fault(); }
static void BusFaultISR() { fault(); }
static void UsageFaultISR() { fault(); }
static void BadISR() { fault(); }

extern "C" void Reset_Handler();
__attribute__((used))
__attribute__((section(".isr_vector"))) void (*const vectors[101])() = {
    // The first entry of the ISR holds the initial value of the
    // stack pointer.  The ARM processor initializes the stack
    // pointer based on this address.
    reinterpret_cast<void (*)()>(&systemStack[SYSTEM_STACK_SIZE]),

    // The second ISR entry is the reset vector which is an
    // assembly language routine that does some basic memory
    // initilization and then calls main().
    // Note that the LSB of the reset vector needs to be set
    // (hence the +1 below).  This tells the ARM that this is
    // thumb code.  The cortex m4 processor only supports
    // thumb code, so this will always be set or we'll get
    // a hard fault.
    reinterpret_cast<void (*)()>(reinterpret_cast<uintptr_t>(Reset_Handler) +
                                 1),

    // The rest of the table is a list of exception and
    // interrupt handlers.  Chapter 12 (NVIC) of the reference
    // manual gives a listing of the vector table offsets.
    NMI,           //   2 - 0x008 The NMI handler
    FaultISR,      //   3 - 0x00C The hard fault handler
    MPUFaultISR,   //   4 - 0x010 The MPU fault handler
    BusFaultISR,   //   5 - 0x014 The bus fault handler
    UsageFaultISR, //   6 - 0x018 The usage fault handler
    BadISR,        //   7 - 0x01C Reserved
    BadISR,        //   8 - 0x020 Reserved
    BadISR,        //   9 - 0x024 Reserved
    BadISR,        //  10 - 0x028 Reserved
    BadISR,        //  11 - 0x02C SVCall handler
    BadISR,        //  12 - 0x030 Debug monitor handler
    BadISR,        //  13 - 0x034 Reserved
    BadISR,        //  14 - 0x038 The PendSV handler
    BadISR,        //  15 - 0x03C
    BadISR,        //  16 - 0x040
    BadISR,        //  17 - 0x044
    BadISR,        //  18 - 0x048
    BadISR,        //  19 - 0x04C
    BadISR,        //  20 - 0x050
    BadISR,        //  21 - 0x054
    BadISR,        //  22 - 0x058
    BadISR,        //  23 - 0x05C
    BadISR,        //  24 - 0x060
    BadISR,        //  25 - 0x064
    BadISR,        //  26 - 0x068
    BadISR,        //  27 - 0x06C
    BadISR,        //  28 - 0x070
    BadISR,        //  29 - 0x074
    BadISR,        //  30 - 0x078
    BadISR,        //  31 - 0x07C
    BadISR,        //  32 - 0x080
    BadISR,        //  33 - 0x084
    BadISR,        //  34 - 0x088
    BadISR,        //  35 - 0x08C
    BadISR,        //  36 - 0x090
    BadISR,        //  37 - 0x094
    BadISR,        //  38 - 0x098
    BadISR,        //  39 - 0x09C
    BadISR,        //  40 - 0x0A0
    BadISR,        //  41 - 0x0A4
    BadISR,        //  42 - 0x0A8
    BadISR,        //  43 - 0x0AC
    BadISR,        //  44 - 0x0B0
    BadISR,        //  45 - 0x0B4
    BadISR,        //  46 - 0x0B8
    BadISR,        //  47 - 0x0BC
    BadISR,        //  48 - 0x0C0
    BadISR,        //  49 - 0x0C4
    BadISR,        //  50 - 0x0C8
    BadISR,        //  51 - 0x0CC
    BadISR,        //  52 - 0x0D0
    BadISR,        //  53 - 0x0D4
    BadISR,        //  54 - 0x0D8
    UART3_ISR,     //  55 - 0x0DC
    BadISR,        //  56 - 0x0E0
    BadISR,        //  57 - 0x0E4
    BadISR,        //  58 - 0x0E8
    BadISR,        //  59 - 0x0EC
    BadISR,        //  60 - 0x0F0
    BadISR,        //  61 - 0x0F4
    BadISR,        //  62 - 0x0F8
    BadISR,        //  63 - 0x0FC
    BadISR,        //  64 - 0x100
    BadISR,        //  65 - 0x104
    BadISR,        //  66 - 0x108
    BadISR,        //  67 - 0x10C
    BadISR,        //  68 - 0x110
    BadISR,        //  69 - 0x114
    Timer6ISR,     //  70 - 0x118
    BadISR,        //  71 - 0x11C
    BadISR,        //  72 - 0x120
    BadISR,        //  73 - 0x124
    BadISR,        //  74 - 0x128
    BadISR,        //  75 - 0x12C
    BadISR,        //  76 - 0x130
    BadISR,        //  77 - 0x134
    BadISR,        //  78 - 0x138
    BadISR,        //  79 - 0x13C
    BadISR,        //  80 - 0x140
    BadISR,        //  81 - 0x144
    BadISR,        //  82 - 0x148
    BadISR,        //  83 - 0x14C
    BadISR,        //  84 - 0x150
    BadISR,        //  85 - 0x154
    BadISR,        //  86 - 0x158
    BadISR,        //  87 - 0x15C
    BadISR,        //  88 - 0x160
    BadISR,        //  89 - 0x164
    BadISR,        //  90 - 0x168
    BadISR,        //  91 - 0x16C
    BadISR,        //  92 - 0x170
    BadISR,        //  93 - 0x174
    BadISR,        //  94 - 0x178
    BadISR,        //  95 - 0x17C
    BadISR,        //  96 - 0x180
    BadISR,        //  97 - 0x184
    BadISR,        //  98 - 0x188
    BadISR,        //  99 - 0x18C
    BadISR,        // 100 - 0x190
};

// Enable an interrupt with a specified priority (0 to 15)
// See the NVIC chapter of the manual for more information.
static void EnableInterrupt(int addr, int pri) {
  IntCtrl_Regs *nvic = NVIC_BASE;

  int id = addr / 4 - 16;

  nvic->setEna[id >> 5] = 1 << (id & 0x1F);

  // The STM32 processor implements bits 4-7 of the NVIM priority register.
  nvic->priority[id] = static_cast<BREG>(pri << 4);
}

#endif
