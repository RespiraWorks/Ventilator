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

/*
This file implements the HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.

Details of the processor's peripherals can be found in [RM].

Abbreviations [RM], [DS], etc are defined in hal/README.md.
*/

#if defined(BARE_STM32)

#include "hal_stm32.h"

#include <cstdarg>
#include <cstdio>
#include <optional>

#include "checksum.h"
#include "circular_buffer.h"
#include "clocks.h"
#include "gpio.h"
#include "hal.h"
#include "stepper.h"
#include "uart_dma.h"
#include "vars.h"

#define SYSTEM_STACK_SIZE 2500

// This is the main stack used in our system.
__attribute__((aligned(8))) uint32_t system_stack[SYSTEM_STACK_SIZE];

// local data
static volatile int64_t ms_count;

// local static functions.  I don't want to add any private
// functions to the Hal class to avoid complexity with other
// builds.
// Those are Interrupt Service Routines, i.e callback functions for the
// interrupt handlers. They are referenced in the Interrupt Vector Table.
static void Timer6ISR();
static void Timer15ISR();
void Uart3ISR();
void DMA1Channel2ISR();
void DMA1Channel3ISR();
void I2c1EventISR();
void I2c1ErrorISR();
void DMA2Channel6ISR();
void DMA2Channel7ISR();

// This function is called from the libc initialization code
// before any static constructors are called.
//
// It calls the Hal function used to initialize the processor.
// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void _init() { hal.EarlyInit(); }

// This replaces the standard abort function which really should never
// be getting called.  It's getting linked in from std::variant though
// and causing lots of other cruft to get linked in as well.
// If we ever did call abort we would just get here and loop until
// the watchdog timer kills us.
// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void abort() {
  while (true) {
    ;  // noop
  };
}

// This function is called _init() above.  It does some basic
// chip initialization.
//
// The main things done here are to enable the FPU because if
// we don't do that then we'll get a fatal exception if any
// constructor uses any floating point math, and to enable
// the PLL so we can run at full speed (80MHz) rather then the
// default speed of 4MHz.
void HalApi::EarlyInit() {
  // Enable the FPU.  This allows floating point to be used without
  // generating a hard fault.
  // The system control registers are documented in [PM] chapter 4.
  // Details on enabling the FPU are in section 4.6.6.
  SysControlReg *sys_ctl = SysControlBase;
  sys_ctl->coproc_access_control = 0x00F00000;

  // Reset caches and set latency for 80MHz operation
  // See chapter 3 of [RM] for details on the embedded flash module
  enable_peripheral_clock(PeripheralID::Flash);
  FlashReg *flash = FlashBase;

  // Set four wait states (required to run at 80MHz)
  flash->access.latency = 4;

  // Reset the instruction and data caches
  flash->access.instruction_cache_reset = 1;
  flash->access.data_cache_reset = 1;
  flash->access.instruction_cache_reset = 0;
  flash->access.data_cache_reset = 0;

  // Enable the caches
  flash->access.instruction_cache_enable = 0;
  flash->access.data_cache_enable = 0;

  configure_pll();
}

/*
 * One time init of HAL.
 */
void HalApi::Init() {
  // Init various components needed by the system.
  InitGpio();
  InitSysTimer();
  InitADC();
  InitPwmOut();
  InitUARTs();
  InitBuzzer();
  InitPSOL();
  InitI2C();
  EnableInterrupts();
  StepperMotorInit();
}

// Reset the processor
[[noreturn]] void HalApi::ResetDevice() {
  // Note that the system control registers are a standard ARM peripheral
  // they are documented in the [PM] rather than the [RM].
  // The register we use to reset the system is called the
  // "Application interrupt and reset control register (AIRCR)"
  SysControlReg *sys_ctl = SysControlBase;
  sys_ctl->app_interrupt = 0x05FA0004;

  // We promised we wouldn't return, so...
  while (true) {
    ;  // noop
  }
}

/******************************************************************
 * General Purpose I/O support.
 *
 * The following pins are used as GPIO on the rev-1 PCB
 *
 * Please refer to the PCB schematic as the ultimate source of which
 * pin is used for which function.  A less definitive, but perhaps
 * easier to read version is available in [PCBsp]
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
 *****************************************************************/
void HalApi::InitGpio() {
  // See [RM] chapter 8 for details on GPIO

  using namespace GPIO;

  // Enable all the GPIO clocks
  enable_peripheral_clock(PeripheralID::GPIOA);
  enable_peripheral_clock(PeripheralID::GPIOB);
  enable_peripheral_clock(PeripheralID::GPIOC);
  enable_peripheral_clock(PeripheralID::GPIOD);
  enable_peripheral_clock(PeripheralID::GPIOE);
  enable_peripheral_clock(PeripheralID::GPIOH);

  // Configure PCB ID pins as inputs.
  SetPinMode(Port::PortB, 1, GPIO::PinMode::Input);
  SetPinMode(Port::PortA, 12, GPIO::PinMode::Input);

  // Configure LED pins as outputs
  SetPinMode(Port::PortC, 13, GPIO::PinMode::Output);
  SetPinMode(Port::PortC, 14, GPIO::PinMode::Output);
  SetPinMode(Port::PortC, 15, GPIO::PinMode::Output);

  // Turn all three LEDs off initially
  ClrPin(Port::PortC, 13);
  ClrPin(Port::PortC, 14);
  ClrPin(Port::PortC, 15);
}

// Set or clear the specified digital output
void HalApi::DigitalWrite(BinaryPin binary_pin, VoltageLevel value) {
  auto [port, pin] = [&]() -> std::pair<GPIO::Port, uint8_t> {
    switch (binary_pin) {
      case BinaryPin::RedLED:
        return {GPIO::Port::PortC, 13};
      case BinaryPin::YellowLED:
        return {GPIO::Port::PortC, 14};
      case BinaryPin::GreenLED:
        return {GPIO::Port::PortC, 15};
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  switch (value) {
    case VoltageLevel::High:
      GPIO::SetPin(port, pin);
      break;

    case VoltageLevel::Low:
      GPIO::ClrPin(port, pin);
      break;
  }
}

/******************************************************************
 * System timer
 *
 * I use one of the basic timers (timer 6) for general system timing.
 * I configure it to count every 100ns and generate an interrupt
 * every millisecond
 *
 * The basic timers (like timer 6) are documented in [RM] chapter 29.
 *****************************************************************/
void HalApi::InitSysTimer() {
  // Enable the clock to the timer
  enable_peripheral_clock(PeripheralID::Timer6);

  // Just set the timer up to count every microsecond.
  TimerReg *tmr = Timer6Base;

  // The reload register gives the number of clock ticks (100ns in our case)
  // -1 until the clock wraps back to zero and generates an interrupt. This
  // setting will cause an interrupt every 10,000 clocks or 1 millisecond
  tmr->auto_reload = 9999;
  tmr->prescaler = (CPUFrequencyMhz / 10 - 1);
  tmr->event = 1;
  // Enable UIFREMAP.  This causes the top bit of tmr->counter to be true if a
  // timer interrupt is pending.
  tmr->control_reg1.bitfield.uif_remapping = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
  tmr->interrupts_enable = 1;

  EnableInterrupt(InterruptVector::Timer6, IntPriority::Standard);
}

static void Timer6ISR() {
  Timer6Base->status = 0;
  ms_count++;
}

void HalApi::Delay(Duration d) {
  Time start = Now();
  while (Now() - start < d) {
  }
}

Time HalApi::Now() {
  // Disable interrupts so we can read ms_count and the timer state without
  // racing with the timer's interrupt handler.
  BlockInterrupts block_interrupts;

  // Bottom 16 bits of the counter are a value in the range [0, 1ms) in units
  // of 100ns.  Top bit of the counter is UIFCOPY, which indicates whether the
  // counter has rolled over and an interrupt to increment ms_count is pending.
  //
  // Since the counter is actively running, we need to read both the counter
  // value and UIFCOPY atomically.
  uint32_t counter = Timer6Base->counter;
  int64_t micros = (counter & 0xffff) / 10;
  bool interrupt_pending = counter >> 31;

  return microsSinceStartup(ms_count * 1000 + micros + (interrupt_pending ? 1 : 0));
}

/******************************************************************
 * Loop timer
 *
 * I use one of the timers (timer 15) to generate the interrupt
 * from which the control loop callback function is called.
 * This function runs at a higher priority then normal code,
 * but not as high as the hardware interrupts.
 *****************************************************************/
static void (*controller_callback)(void *);
static void *controller_arg;
void HalApi::StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg) {
  controller_callback = callback;
  controller_arg = arg;

  // Init the watchdog timer now.  The watchdog timer is serviced by the
  // loop callback function.  I don't init it until the loop starts because
  // otherwise it may expire before the function that resets it starts running
  WatchdogInit();

  // Find the loop period in clock cycles
  int32_t reload = static_cast<int32_t>(CPUFrequency * period.seconds());
  int prescale = 1;

  // Adjust the prescaler so that my reload count will fit in the 16-bit
  // timer.
  if (reload > 65536) {
    prescale = static_cast<int>(reload / 65536.0) + 1;
    reload /= prescale;
  }

  // Enable the clock to the timer
  enable_peripheral_clock(PeripheralID::Timer15);

  // Just set the timer up to count every microsecond.
  TimerReg *tmr = Timer15Base;
  tmr->auto_reload = reload - 1;
  tmr->prescaler = prescale - 1;
  tmr->event = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
  tmr->interrupts_enable = 1;

  // Enable the interrupt that will call the controller
  // function callback periodically.
  // I'm using a lower priority then that which I use
  // for normal hardware interrupts.  This means that other
  // interrupts can be serviced while controller functions
  // are running.
  EnableInterrupt(InterruptVector::Timer15, IntPriority::Low);
}

static float latency, max_latency, loop_time;
static Debug::Variable::Primitive32 dbg_loop_latency("loop_latency",
                                                     Debug::Variable::Access::ReadOnly, &latency,
                                                     "\xB5s", "Latency of loop function", "%.2f");
static Debug::Variable::Primitive32 dbg_max_latency("max_latency",
                                                    Debug::Variable::Access::ReadOnly, &max_latency,
                                                    "\xB5s", "Maximum latency of loop function",
                                                    "%.2f");
static Debug::Variable::Primitive32 dbg_loop_time("loop_time", Debug::Variable::Access::ReadOnly,
                                                  &loop_time, "\xB5s", "Duration of loop function",
                                                  "%.2f");

static void Timer15ISR() {
  uint32_t start = Timer15Base->counter;
  Timer15Base->status = 0;

  // Keep track of loop latency in uSec
  // Also max latency since it was last zeroed
  latency = static_cast<float>(start) * (1.0f / CPUFrequencyMhz);
  if (latency > max_latency) max_latency = latency;

  // Call the function
  controller_callback(controller_arg);

  uint32_t end = Timer15Base->counter;
  loop_time = static_cast<float>(end - start) * (1.0f / CPUFrequencyMhz);

  // Start sending any queued commands to the stepper motor
  StepMotor::StartQueuedCommands();
}

/******************************************************************
 * PWM outputs
 *
 * The following four outputs could be driven
 * as PWM outputs:
 *
 * PA8  - Timer 1 Channel 1 - heater control
 * PB3  - Timer 2 Channel 2 - blower control
 *
 * For now I'll just set up the blower since that's the only
 * one called out in the HAL
 *
 * These timers are documented in [RM] chapters 26 and 27.
 *****************************************************************/
void HalApi::InitPwmOut() {
  // The PWM frequency isn't mentioned anywhere that I can find, so
  // I'm just picking a reasonable number.  This can be refined later
  //
  // The selection of PWM frequency is a trade off between latency and
  // resolution.  Higher frequencies give lower latency and lower resolution.
  //
  // Latency is the time between setting the value and it taking effect,
  // this is essentially the PWM period (1/frequency).  For example, a
  // 20kHz frequency would give a latency of up to 50 usec.
  //
  // Resultion is based on the ratio of the clock frequency (80MHz) to the
  // PWM frequency.  For example, a 20kHz PWM would have a resolution of one
  // part in 4000 (80000000/20000) or about 12 bits.
  static constexpr int PwmFreqHz = 20000;

  enable_peripheral_clock(PeripheralID::Timer2);

  // Connect PB3 to timer 2
  GPIO::PinAltFunc(GPIO::Port::PortB, 3, 1);

  TimerReg *tmr = Timer2Base;

  // Set the frequency
  tmr->auto_reload = (CPUFrequency / PwmFreqHz) - 1;

  // Configure channel 2 in PWM output mode 1
  // with preload enabled.  The preload means that
  // the new PWM duty cycle gets written to a shadow
  // register and copied to the active register
  // at the start of the next cycle.
  tmr->capture_compare_mode[0] = 0x6800;

  tmr->capture_compare_enable = 0x10;

  // Start with 0% duty cycle
  tmr->capture_compare[1] = 0;

  // Load the shadow registers
  tmr->event = 1;

  // Start the counter
  tmr->control_reg1.bitfield.auto_reload_preload = 1;
  tmr->control_reg1.bitfield.counter_enable = 1;
}

// Set the PWM period.
void HalApi::AnalogWrite(PwmPin pin, float duty) {
  auto [tmr, chan] = [&]() -> std::pair<TimerReg *, int> {
    switch (pin) {
      case PwmPin::Blower:
        return {Timer2Base, 1};
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  tmr->capture_compare[chan] = static_cast<uint32_t>(static_cast<float>(tmr->auto_reload) * duty);
}

/******************************************************************
 * Serial port to GUI
 * [RM] Chapter 38 defines the USART registers.
 *****************************************************************/

class UART {
  CircularBuffer<uint8_t, 128> rx_data_;
  CircularBuffer<uint8_t, 128> tx_data_;
  UartReg *const uart_;

 public:
  explicit UART(UartReg *const r) : uart_(r) {}

  void Init(uint32_t baud) {
    // Set baud rate register
    uart_->baudrate = CPUFrequency / baud;

    uart_->control_reg1.bitfield.rx_interrupt = 1;  // enable receive interrupt
    uart_->control_reg1.bitfield.tx_enable = 1;     // enable transmitter
    uart_->control_reg1.bitfield.rx_enable = 1;     // enable receiver
    uart_->control_reg1.bitfield.enable = 1;        // enable uart
  }

  // This is the interrupt handler for the UART.
  void ISR() {
    // Check for overrun error and framing errors.  Clear those errors if
    // they're set to avoid further interrupts from them.
    if (uart_->status.bitfield.framing_error) {
      uart_->interrupt_clear.bitfield.framing_error_clear = 1;
    }
    if (uart_->status.bitfield.overrun_error) {
      uart_->interrupt_clear.bitfield.overrun_clear = 1;
    }

    // See if we received a new byte.
    if (uart_->status.bitfield.rx_not_empty) {
      // Add the byte to rx_data_.  If the buffer is full, we'll drop it --
      // what else can we do?
      //
      // TODO: Perhaps log a warning here so we have an idea of whether
      // this buffer is hitting capacity frequently.
      (void)rx_data_.Put(static_cast<uint8_t>(uart_->rx_data));
    }

    // Check for transmit data register empty
    if (uart_->status.bitfield.tx_empty && uart_->control_reg1.bitfield.tx_interrupt) {
      std::optional<uint8_t> ch = tx_data_.Get();

      // If there's nothing left in the transmit buffer,
      // just disable further transmit interrupts.
      if (ch == std::nullopt) {
        uart_->control_reg1.bitfield.tx_interrupt = 0;
      } else {
        // Otherwise, Send the next byte.
        uart_->tx_data = *ch;
      }
    }
  }

  // Read up to len bytes and store them in the passed buffer.
  // This function does not block, so if less then len bytes
  // are available it will only return the available bytes
  // Returns the number of bytes actually read.
  uint16_t Read(char *buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
      std::optional<uint8_t> ch = rx_data_.Get();
      if (ch == std::nullopt) {
        return i;
      }
      *buf++ = *ch;
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
  uint16_t Write(const char *buf, uint16_t len) {
    uint16_t i;
    for (i = 0; i < len; i++) {
      if (!tx_data_.Put(*buf++)) break;
    }

    // Enable the tx interrupt.  If there was already anything
    // in the buffer this will already be enabled, but enabling
    // it again doesn't hurt anything.
    uart_->control_reg1.bitfield.tx_interrupt = 1;
    return i;
  }

  // Return the number of bytes currently in the
  // receive buffer and ready to be read.
  uint16_t RxFull() { return static_cast<uint16_t>(rx_data_.FullCount()); }

  // Returns the number of free locations in the
  // transmit buffer.
  uint16_t TxFree() { return static_cast<uint16_t>(tx_data_.FreeCount()); }
};

static UART rpi_uart(Uart3Base);
static UART debug_uart(Uart2Base);
#ifdef UART_VIA_DMA
extern UartDma dma_uart;
#endif
// The UART that talks to the rPi uses the following pins:
//    PB10 - TX
//    PB11 - RX
//    PB13 - RTS
//    PB14 - CTS
//
// The Nucleo board also includes a secondary serial port that's
// indirectly connected to its USB connector.  This port is
// connected to the STM32 UART2 at pins:
//    PA2 - TX
//    PA3 - RX
//
// Please refer to the PCB schematic as the ultimate source of which
// pin is used for which function.  A less definitive, but perhaps
// easier to read version is available at [PCBsp].
//
// These pins are connected to UART3
// The UART is described in [RM] chapter 38
void HalApi::InitUARTs() {
  // NOTE - The UART functionality hasn't been tested due to lack of hardware!
  //        Need to do that as soon as the boards are available.
  enable_peripheral_clock(PeripheralID::UART2);
  enable_peripheral_clock(PeripheralID::UART3);
#ifdef UART_VIA_DMA
  enable_peripheral_clock(PeripheralID::DMA1);
#endif
  GPIO::PinAltFunc(GPIO::Port::PortA, 2, 7);
  GPIO::PinAltFunc(GPIO::Port::PortA, 3, 7);

  GPIO::PinAltFunc(GPIO::Port::PortB, 10, 7);
  GPIO::PinAltFunc(GPIO::Port::PortB, 11, 7);
  GPIO::PinAltFunc(GPIO::Port::PortB, 13, 7);
  GPIO::PinAltFunc(GPIO::Port::PortB, 14, 7);

#ifdef UART_VIA_DMA
  dma_uart.Init(115200);
#else
  rpi_uart.Init(115200);
#endif
  debug_uart.Init(115200);

  EnableInterrupt(InterruptVector::Dma1Channel2, IntPriority::Standard);
  EnableInterrupt(InterruptVector::Dma1Channel3, IntPriority::Standard);
  EnableInterrupt(InterruptVector::Uart2, IntPriority::Standard);
  EnableInterrupt(InterruptVector::Uart3, IntPriority::Standard);
}

static void Uart2ISR() { debug_uart.ISR(); }

#ifndef UART_VIA_DMA
void Uart3ISR() { rpi_uart.ISR(); }
#endif

uint16_t HalApi::SerialRead(char *buf, uint16_t len) { return rpi_uart.Read(buf, len); }

uint16_t HalApi::SerialBytesAvailableForRead() { return rpi_uart.RxFull(); }

uint16_t HalApi::SerialWrite(const char *buf, uint16_t len) { return rpi_uart.Write(buf, len); }

uint16_t HalApi::SerialBytesAvailableForWrite() { return rpi_uart.TxFree(); }

uint16_t HalApi::DebugWrite(const char *buf, uint16_t len) { return debug_uart.Write(buf, len); }

uint16_t HalApi::DebugRead(char *buf, uint16_t len) { return debug_uart.Read(buf, len); }

uint16_t HalApi::DebugBytesAvailableForWrite() { return debug_uart.TxFree(); }

/******************************************************************
 * Watchdog timer (see [RM] chapter 32).
 *
 * The watchdog timer will reset the system if it hasn't been
 * re-initialized within a specific amount of time.  It's used
 * to catch bugs that would otherwise hang the system.  When
 * the watchdog is enabled such a bug will reset the system
 * rather then let it hang indefinitely.
 *****************************************************************/
void HalApi::WatchdogInit() {
  WatchdogReg *wdog = WatchdogBase;

  // Enable the watchdog timer by writing the appropriate value to its key
  // register
  wdog->key = 0xCCCC;

  // Enable register access
  wdog->key = 0x5555;

  // Set the pre-scaler to 0.  That setting will cause the watchdog
  // clock to be updated at approximately 8KHz.
  wdog->prescaler = 0;

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
void HalApi::WatchdogHandler() {
  WatchdogReg *wdog = WatchdogBase;
  wdog->key = 0xAAAA;
}

// Fault handler
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
[[noreturn]] static void Fault() {
  while (true) {
    ;  // noop
    /* \todo this function is unused; could be made useful if implementation
     * made sense? blink lights? scream? do something? */
  }
}
#pragma GCC diagnostic pop

static void StepperISR() { StepMotor::DmaISR(); }

/******************************************************************
 * Interrupt vector table.  The interrupt vector table is a list of
 * pointers to the various interrupt functions.  It is stored at the
 * very start of the flash memory.
 *****************************************************************/

// TODO: these could optionally call Fault() but not in production
static void NMI() {}
static void FaultISR() {}
static void MPUFaultISR() {}
static void BusFaultISR() {}
static void UsageFaultISR() {}
static void BadISR() {}

// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void Reset_Handler();
__attribute__((used)) __attribute__((section(".isr_vector"))) void (*const Vectors[101])() = {
    // The first entry of the ISR holds the initial value of the
    // stack pointer.  The ARM processor initializes the stack
    // pointer based on this address.
    reinterpret_cast<void (*)()>(&system_stack[SYSTEM_STACK_SIZE]),

    // The second ISR entry is the reset vector which is an
    // assembly language routine that does some basic memory
    // initialization and then calls main().
    // Note that the LSB of the reset vector needs to be set
    // (hence the +1 below).  This tells the ARM that this is
    // thumb code.  The cortex m4 processor only supports
    // thumb code, so this will always be set or we'll get
    // a hard fault.
    reinterpret_cast<void (*)()>(reinterpret_cast<uintptr_t>(Reset_Handler) + 1),

    // The rest of the table is a list of exception and interrupt handlers.
    // [RM] chapter 12 (NVIC) gives a listing of the vector table offsets.
    NMI,            //   2 - 0x008 The NMI handler
    FaultISR,       //   3 - 0x00C The hard fault handler
    MPUFaultISR,    //   4 - 0x010 The MPU fault handler
    BusFaultISR,    //   5 - 0x014 The bus fault handler
    UsageFaultISR,  //   6 - 0x018 The usage fault handler
    BadISR,         //   7 - 0x01C Reserved
    BadISR,         //   8 - 0x020 Reserved
    BadISR,         //   9 - 0x024 Reserved
    BadISR,         //  10 - 0x028 Reserved
    BadISR,         //  11 - 0x02C SVCall handler
    BadISR,         //  12 - 0x030 Debug monitor handler
    BadISR,         //  13 - 0x034 Reserved
    BadISR,         //  14 - 0x038 The PendSV handler
    BadISR,         //  15 - 0x03C SysTick
    BadISR,         //  16 - 0x040
    BadISR,         //  17 - 0x044
    BadISR,         //  18 - 0x048
    BadISR,         //  19 - 0x04C
    BadISR,         //  20 - 0x050
    BadISR,         //  21 - 0x054
    BadISR,         //  22 - 0x058
    BadISR,         //  23 - 0x05C
    BadISR,         //  24 - 0x060
    BadISR,         //  25 - 0x064
    BadISR,         //  26 - 0x068
    BadISR,         //  27 - 0x06C
#ifdef UART_VIA_DMA
    DMA1Channel2ISR,  //  28 - 0x070 DMA1 CH2
    DMA1Channel3ISR,  //  29 - 0x074 DMA1 CH3
#else
    BadISR,  //  28 - 0x070
    BadISR,  //  29 - 0x074
#endif
    BadISR,           //  30 - 0x078
    BadISR,           //  31 - 0x07C
    BadISR,           //  32 - 0x080
    BadISR,           //  33 - 0x084
    BadISR,           //  34 - 0x088
    BadISR,           //  35 - 0x08C
    BadISR,           //  36 - 0x090
    BadISR,           //  37 - 0x094
    BadISR,           //  38 - 0x098
    BadISR,           //  39 - 0x09C
    Timer15ISR,       //  40 - 0x0A0
    BadISR,           //  41 - 0x0A4
    BadISR,           //  42 - 0x0A8
    BadISR,           //  43 - 0x0AC
    BadISR,           //  44 - 0x0B0
    BadISR,           //  45 - 0x0B4
    BadISR,           //  46 - 0x0B8
    I2c1EventISR,     //  47 - 0x0BC I2C1 Events
    I2c1ErrorISR,     //  48 - 0x0C0 I2C1 Errors
    BadISR,           //  49 - 0x0C4
    BadISR,           //  50 - 0x0C8
    BadISR,           //  51 - 0x0CC
    BadISR,           //  52 - 0x0D0
    BadISR,           //  53 - 0x0D4
    Uart2ISR,         //  54 - 0x0D8
    Uart3ISR,         //  55 - 0x0DC
    BadISR,           //  56 - 0x0E0
    BadISR,           //  57 - 0x0E4
    BadISR,           //  58 - 0x0E8
    BadISR,           //  59 - 0x0EC
    BadISR,           //  60 - 0x0F0
    BadISR,           //  61 - 0x0F4
    BadISR,           //  62 - 0x0F8
    BadISR,           //  63 - 0x0FC
    BadISR,           //  64 - 0x100
    BadISR,           //  65 - 0x104
    BadISR,           //  66 - 0x108
    BadISR,           //  67 - 0x10C
    BadISR,           //  68 - 0x110
    BadISR,           //  69 - 0x114
    Timer6ISR,        //  70 - 0x118
    BadISR,           //  71 - 0x11C
    BadISR,           //  72 - 0x120
    BadISR,           //  73 - 0x124
    StepperISR,       //  74 - 0x128
    BadISR,           //  75 - 0x12C
    BadISR,           //  76 - 0x130
    BadISR,           //  77 - 0x134
    BadISR,           //  78 - 0x138
    BadISR,           //  79 - 0x13C
    BadISR,           //  80 - 0x140
    BadISR,           //  81 - 0x144
    BadISR,           //  82 - 0x148
    BadISR,           //  83 - 0x14C
    DMA2Channel6ISR,  //  84 - 0x150
    DMA2Channel7ISR,  //  85 - 0x154
    BadISR,           //  86 - 0x158
    BadISR,           //  87 - 0x15C
    BadISR,           //  88 - 0x160
    BadISR,           //  89 - 0x164
    BadISR,           //  90 - 0x168
    BadISR,           //  91 - 0x16C
    BadISR,           //  92 - 0x170
    BadISR,           //  93 - 0x174
    BadISR,           //  94 - 0x178
    BadISR,           //  95 - 0x17C
    BadISR,           //  96 - 0x180
    BadISR,           //  97 - 0x184
    BadISR,           //  98 - 0x188
    BadISR,           //  99 - 0x18C
    BadISR,           // 100 - 0x190
};

// Enable an interrupt with a specified priority (0 to 15)
// See [RM] chapter 12 for more information on the NVIC.
void HalApi::EnableInterrupt(InterruptVector vec, IntPriority pri) {
  InterruptControlReg *nvic = NvicBase;

  int addr = static_cast<int>(vec);

  int id = addr / 4 - 16;

  nvic->set_enable[id >> 5] = 1 << (id & 0x1F);

  // The STM32 processor implements bits 4-7 of the NVIM priority register.
  int p = static_cast<int>(pri);
  nvic->priority[id] = static_cast<uint8_t>(p << 4);
}

#endif
