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

#include "hal.h"

#if defined(BARE_STM32)

#include <optional>

#include "clocks.h"
#include "flash.h"
#include "gpio.h"
#include "hal_stm32_regs.h"
#include "i2c.h"
#include "interrupts.h"
#include "stepper.h"
#include "system_timer.h"
#include "timers.h"
#include "uart.h"
#include "uart_dma.h"
#include "vars.h"
#include "watchdog.h"

static constexpr Frequency CPUFrequency{megahertz(80)};
static constexpr Frequency UARTBaudRate{hertz(115200)};

static constexpr uint32_t SystemStackSize{2500};

// This is the main stack used in our system.
__attribute__((aligned(8))) uint32_t system_stack[SystemStackSize];

// local static functions.  I don't want to add any private
// functions to the Hal class to avoid complexity with other
// builds.
// Those are Interrupt Service Routines, i.e callback functions for the
// interrupt handlers. They are referenced in the Interrupt Vector Table.
void Timer6ISR();
void Timer15ISR();
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

Frequency HalApi::GetCpuFreq() { return CPUFrequency; }

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

  /// \TODO: initialized but never used. Do we still need it?
  Flash::initialize();

  configure_pll();
}

void Timer6ISR() { SystemTimer::singleton().interrupt_handler(); }

/*
 * One time init of HAL.
 */
void HalApi::Init() {
  // Init various components needed by the system.
  GPIO::enable_all_clocks();
  init_PCB_ID_pins();
  LEDs.initialize();
  SystemTimer::singleton().initialize(CPUFrequency);
  /// \TODO: fault somehow if this returns false
  [[maybe_unused]] bool buffer_size_sufficient = adc.initialize(CPUFrequency);
  buzzer.initialize(CPUFrequency);
  InitUARTs();
  I2C::initialize();
  Interrupts::singleton().EnableInterrupts();
  StepMotor::OneTimeInit();
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
 * ID inputs.  These can be used to identify the PCB revision
 * we're running on.
 *  PB1  - ID0
 *  PA12 - ID1
 *****************************************************************/
void HalApi::init_PCB_ID_pins() {
  // Configure PCB ID pins as inputs.
  GPIO::pin_mode(GPIO::Port::B, 1, GPIO::PinMode::Input);
  GPIO::pin_mode(GPIO::Port::A, 12, GPIO::PinMode::Input);
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
  Watchdog::initialize();

  // Find the loop period in clock cycles
  int32_t reload = static_cast<int32_t>(CPUFrequency.hertz() * period.seconds());
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
  Interrupts::singleton().EnableInterrupt(InterruptVector::Timer15, IntPriority::Low);
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

void Timer15ISR() {
  uint32_t start = Timer15Base->counter;
  Timer15Base->status = 0;

  // Keep track of loop latency in uSec
  // Also max latency since it was last zeroed
  latency = static_cast<float>(start) * (1.0f / CPUFrequency.megahertz());
  if (latency > max_latency) max_latency = latency;

  // Call the function
  controller_callback(controller_arg);

  uint32_t end = Timer15Base->counter;
  loop_time = static_cast<float>(end - start) * (1.0f / CPUFrequency.megahertz());

  /// \TODO: Too tightly coupled bc HAL must be aware of steppers. Use another callback?
  // Start sending any queued commands to the stepper motor
  StepMotor::StartQueuedCommands();
}

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
// connected to the STM32 USART2 at pins:
//    PA2 - TX
//    PA3 - RX
//
// Please refer to the PCB schematic as the ultimate source of which
// pin is used for which function.  A less definitive, but perhaps
// easier to read version is available at [PCBsp].
//
// These pins are connected to UART3
// The UART/USART is described in [RM] chapter 38
void HalApi::InitUARTs() {
  // NOTE - The UART functionality hasn't been tested due to lack of hardware!
  //        Need to do that as soon as the boards are available.
  enable_peripheral_clock(PeripheralID::USART2);
  enable_peripheral_clock(PeripheralID::USART3);
#ifdef UART_VIA_DMA
  enable_peripheral_clock(PeripheralID::DMA1);
#endif
  // [DS] Table 17 (pg 76)
  GPIO::alternate_function(GPIO::Port::A, /*pin =*/2,
                           GPIO::AlternativeFunction::AF7);  // USART2_TX
  GPIO::alternate_function(GPIO::Port::A, /*pin =*/3,
                           GPIO::AlternativeFunction::AF7);  // USART2_RX

  // [DS] Table 17 (pg 77)
  GPIO::alternate_function(GPIO::Port::B, /*pin =*/10,
                           GPIO::AlternativeFunction::AF7);  // USART3_TX
  GPIO::alternate_function(GPIO::Port::B, /*pin =*/11,
                           GPIO::AlternativeFunction::AF7);  // USART3_RX
  GPIO::alternate_function(GPIO::Port::B, /*pin =*/13,
                           GPIO::AlternativeFunction::AF7);  // USART3_CTS
  GPIO::alternate_function(GPIO::Port::B, /*pin =*/14,
                           GPIO::AlternativeFunction::AF7);  // USART3_RTS_DE

#ifdef UART_VIA_DMA
  dma_uart.initialize(CPUFrequency, UARTBaudRate);
#else
  rpi_uart.Init(CPUFrequency, UARTBaudRate);
#endif
  debug_uart.Init(CPUFrequency, UARTBaudRate);

  Interrupts::singleton().EnableInterrupt(InterruptVector::Dma1Channel2, IntPriority::Standard);
  Interrupts::singleton().EnableInterrupt(InterruptVector::Dma1Channel3, IntPriority::Standard);
  Interrupts::singleton().EnableInterrupt(InterruptVector::Uart2, IntPriority::Standard);
  Interrupts::singleton().EnableInterrupt(InterruptVector::Uart3, IntPriority::Standard);
}

void Uart2ISR() { debug_uart.ISR(); }

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

// Fault handler
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
[[noreturn]] static void Fault() {
  while (true) {
    ;  // noop
    /// \TODO function is unused. Make useful: blink lights? scream? do something?
    ///       flash and beep morse code?
  }
}
#pragma GCC diagnostic pop

void StepperISR() { StepMotor::DmaISR(); }

/******************************************************************
 * Interrupt vector table.  The interrupt vector table is a list of
 * pointers to the various interrupt functions.  It is stored at the
 * very start of the flash memory.
 *****************************************************************/

/// \TODO: these could optionally call Fault() but not in production, log error to EEPROM
void NMI() {}
void FaultISR() {}
void MPUFaultISR() {}
void BusFaultISR() {}
void UsageFaultISR() {}
void BadISR() {}

// Those interrupt service routines are specific to our configuration, unlike
// the I2C::Channel::*ISR() which are generic ISR associated with an I²C channel
void I2c1EventISR() { i2c1.I2CEventHandler(); };
void I2c1ErrorISR() { i2c1.I2CErrorHandler(); };
void DMA2Channel6ISR() { i2c1.DMAIntHandler(DMA::Channel::Chan6); };
void DMA2Channel7ISR() { i2c1.DMAIntHandler(DMA::Channel::Chan7); };

// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void Reset_Handler();
__attribute__((used)) __attribute__((section(".isr_vector"))) void (*const Vectors[101])() = {
    // The first entry of the ISR holds the initial value of the
    // stack pointer.  The ARM processor initializes the stack
    // pointer based on this address.
    reinterpret_cast<void (*)()>(&system_stack[SystemStackSize]),

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

#endif
