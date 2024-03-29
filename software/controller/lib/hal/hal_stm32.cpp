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

/*
This file implements the HAL (Hardware Abstraction Layer) for the
STM32L452 processor used on the controller.

Details of the processor's peripherals can be found in [RM].

Abbreviations [RM], [DS], etc are defined in hal/README.md.
*/

#if defined(BARE_STM32)

#include "clocks_stm32.h"
#include "flash.h"
#include "hal.h"
#include "high_priority_trigger.h"
#include "interrupts.h"
#include "stepper.h"
#include "sys_control.h"
#include "system_timer.h"

/// \todo why this number?
static constexpr uint32_t SystemStackSize{2500};

// This is the main stack used in our system.
__attribute__((aligned(8))) uint32_t system_stack[SystemStackSize];

// This function is called from the libc initialization code before any static constructors are
// called. It calls the Hal function used to initialize the processor.
// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void _init() { hal.EarlyInit(); }

// This replaces the standard abort function which really should never be getting called.  It's
// getting linked in from std::variant though and causing lots of other cruft to get linked in as
// well. If we ever did call abort we would just get here and loop until the watchdog timer kills
// us.
// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void abort() {
  while (true) {
    ;  // noop
  };
}

// This function is called by _init() above.  It does some basic chip initialization.
//
// The main things done here are to enable the FPU because if we don't do that then we'll get a
// fatal exception if any constructor uses any floating point math, and to enable the PLL, so we can
// run at full speed (80MHz) rather than the default speed of 4MHz.
void HalApi::EarlyInit() {
  // Enable the FPU.  This allows floating point to be used without generating a hard fault.
  SysControl::enable_fpu();

  /// \TODO not clear why this has to be here, but initialization does not work without it
  Flash::initialize();

  Clocks::configure_pll();
}

/*
 * One time init of HAL.
 */
void HalApi::Init(Frequency cpu_frequency) {
  // Init various components needed by the system.
  Clocks::enable_gpio_clocks();
  cpu_frequency_ = cpu_frequency;
}

void HalApi::bind_channels(I2C::Channel *i2c, UART::DMAChannel *rpi, UART::Channel *debug,
                           SPI::Channel *spi) {
  i2c_ = i2c;
  rpi_uart_ = rpi;
  debug_uart_ = debug;
  stepper_spi_ = spi;
}

// Reset the processor
// this gets called from pure_virtual
[[noreturn]] void HalApi::ResetDevice() {
  SysControl::reset_processor();

  // We promised we wouldn't return, so...
  while (true) {
    ;  // noop
  }
}

// Loop timer
// We use one of the timers (timer 15) to generate the interrupt from which the control loop
// callback function is called. This function runs at a higher priority then normal code, but not as
// high as the hardware interrupts.
void HalApi::StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg) {
  HighPriorityTrigger::singleton().start(PeripheralID::Timer15, InterruptVector::Timer15,
                                         hal.cpu_frequency_, period, callback, arg);
}

void HalApi::Timer6ISR() {
  /// \todo make this a callback
  SystemTimer::singleton().interrupt_handler();
}

void HalApi::Timer15ISR() {
  /// \todo make this a callback
  HighPriorityTrigger::singleton().interrupt_handler();
}

/// \todo make this a callback from main_loop or system
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

/// \TODO these could optionally call Fault() but not in production, log error to EEPROM
/// \todo these should use a callback
void NMI() {}
void FaultISR() {}
void MPUFaultISR() {}
void BusFaultISR() {}
void UsageFaultISR() {}
void BadISR() {}

// Those interrupt service routines are specific to our configuration, unlike
// the I2C::Channel::*ISR() which are generic ISR associated with an I²C channel
void HalApi::I2c1EventISR() {
  if (hal.i2c_) {
    hal.i2c_->I2CEventHandler();
  }
};

void HalApi::I2c1ErrorISR() {
  if (hal.i2c_) {
    hal.i2c_->I2CErrorHandler();
  }
};

void HalApi::DMA2Channel6ISR() {
  if (hal.i2c_) {
    hal.i2c_->DMAInterruptHandler(I2C::ExchangeDirection::Read);
  }
};

void HalApi::DMA2Channel7ISR() {
  if (hal.i2c_) {
    hal.i2c_->DMAInterruptHandler(I2C::ExchangeDirection::Write);
  }
};

void HalApi::DMA1Channel2ISR() {
  if (hal.rpi_uart_) {
    hal.rpi_uart_->TxDMAInterruptHandler();
  }
}
void HalApi::DMA1Channel3ISR() {
  if (hal.rpi_uart_) {
    hal.rpi_uart_->RxDMAInterruptHandler();
  }
}

void HalApi::Uart3ISR() {
  if (hal.rpi_uart_) {
    hal.rpi_uart_->UARTInterruptHandler();
  }
}
void HalApi::Uart2ISR() {
  if (hal.debug_uart_) {
    hal.debug_uart_->UARTInterruptHandler();
  }
}

void HalApi::DMA2Channel4ISR() {
  if (hal.stepper_spi_) {
    hal.stepper_spi_->RxDMAInterruptHandler();
  }
}

// We don't control this function's name, silence the style check
// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" void Reset_Handler();

/*******************************************************************************************
 * Interrupt vector table.  The interrupt vector table is a list of pointers to the various
 * interrupt functions.  It is stored at the very start of the flash memory.
 *******************************************************************************************/
__attribute__((used)) __attribute__((section(".isr_vector"))) void (*const Vectors[101])() = {
    // The first entry of the ISR holds the initial value of the stack pointer.  The ARM processor
    // initializes the stack pointer based on this address.
    reinterpret_cast<void (*)()>(&system_stack[SystemStackSize]),

    // The second ISR entry is the reset vector which is an assembly language routine that does some
    // basic memory initialization and then calls main(). Note that the LSB of the reset vector
    // needs to be set (hence the +1 below).  This tells the ARM that this is thumb code.  The
    // cortex m4 processor only supports thumb code, so this will always be set, or we'll get a hard
    // fault.
    reinterpret_cast<void (*)()>(reinterpret_cast<uintptr_t>(Reset_Handler) + 1),

    // The rest of the table is a list of exception and interrupt handlers.
    // [RM] chapter 12 (NVIC) gives a listing of the vector table offsets.
    NMI,                      //   2 - 0x008 The NMI handler
    FaultISR,                 //   3 - 0x00C The hard fault handler
    MPUFaultISR,              //   4 - 0x010 The MPU fault handler
    BusFaultISR,              //   5 - 0x014 The bus fault handler
    UsageFaultISR,            //   6 - 0x018 The usage fault handler
    BadISR,                   //   7 - 0x01C Reserved
    BadISR,                   //   8 - 0x020 Reserved
    BadISR,                   //   9 - 0x024 Reserved
    BadISR,                   //  10 - 0x028 Reserved
    BadISR,                   //  11 - 0x02C SVCall handler
    BadISR,                   //  12 - 0x030 Debug monitor handler
    BadISR,                   //  13 - 0x034 Reserved
    BadISR,                   //  14 - 0x038 The PendSV handler
    BadISR,                   //  15 - 0x03C SysTick
    BadISR,                   //  16 - 0x040
    BadISR,                   //  17 - 0x044
    BadISR,                   //  18 - 0x048
    BadISR,                   //  19 - 0x04C
    BadISR,                   //  20 - 0x050
    BadISR,                   //  21 - 0x054
    BadISR,                   //  22 - 0x058
    BadISR,                   //  23 - 0x05C
    BadISR,                   //  24 - 0x060
    BadISR,                   //  25 - 0x064
    BadISR,                   //  26 - 0x068
    BadISR,                   //  27 - 0x06C
    HalApi::DMA1Channel2ISR,  //  28 - 0x070 DMA1 CH2
    HalApi::DMA1Channel3ISR,  //  29 - 0x074 DMA1 CH3
    BadISR,                   //  30 - 0x078
    BadISR,                   //  31 - 0x07C
    BadISR,                   //  32 - 0x080
    BadISR,                   //  33 - 0x084
    BadISR,                   //  34 - 0x088
    BadISR,                   //  35 - 0x08C
    BadISR,                   //  36 - 0x090
    BadISR,                   //  37 - 0x094
    BadISR,                   //  38 - 0x098
    BadISR,                   //  39 - 0x09C
    HalApi::Timer15ISR,       //  40 - 0x0A0
    BadISR,                   //  41 - 0x0A4
    BadISR,                   //  42 - 0x0A8
    BadISR,                   //  43 - 0x0AC
    BadISR,                   //  44 - 0x0B0
    BadISR,                   //  45 - 0x0B4
    BadISR,                   //  46 - 0x0B8
    HalApi::I2c1EventISR,     //  47 - 0x0BC I2C1 Events
    HalApi::I2c1ErrorISR,     //  48 - 0x0C0 I2C1 Errors
    BadISR,                   //  49 - 0x0C4
    BadISR,                   //  50 - 0x0C8
    BadISR,                   //  51 - 0x0CC
    BadISR,                   //  52 - 0x0D0
    BadISR,                   //  53 - 0x0D4
    HalApi::Uart2ISR,         //  54 - 0x0D8
    HalApi::Uart3ISR,         //  55 - 0x0DC
    BadISR,                   //  56 - 0x0E0
    BadISR,                   //  57 - 0x0E4
    BadISR,                   //  58 - 0x0E8
    BadISR,                   //  59 - 0x0EC
    BadISR,                   //  60 - 0x0F0
    BadISR,                   //  61 - 0x0F4
    BadISR,                   //  62 - 0x0F8
    BadISR,                   //  63 - 0x0FC
    BadISR,                   //  64 - 0x100
    BadISR,                   //  65 - 0x104
    BadISR,                   //  66 - 0x108
    BadISR,                   //  67 - 0x10C
    BadISR,                   //  68 - 0x110
    BadISR,                   //  69 - 0x114
    HalApi::Timer6ISR,        //  70 - 0x118
    BadISR,                   //  71 - 0x11C
    BadISR,                   //  72 - 0x120
    BadISR,                   //  73 - 0x124
    HalApi::DMA2Channel4ISR,  //  74 - 0x128
    BadISR,                   //  75 - 0x12C
    BadISR,                   //  76 - 0x130
    BadISR,                   //  77 - 0x134
    BadISR,                   //  78 - 0x138
    BadISR,                   //  79 - 0x13C
    BadISR,                   //  80 - 0x140
    BadISR,                   //  81 - 0x144
    BadISR,                   //  82 - 0x148
    BadISR,                   //  83 - 0x14C
    HalApi::DMA2Channel6ISR,  //  84 - 0x150
    HalApi::DMA2Channel7ISR,  //  85 - 0x154
    BadISR,                   //  86 - 0x158
    BadISR,                   //  87 - 0x15C
    BadISR,                   //  88 - 0x160
    BadISR,                   //  89 - 0x164
    BadISR,                   //  90 - 0x168
    BadISR,                   //  91 - 0x16C
    BadISR,                   //  92 - 0x170
    BadISR,                   //  93 - 0x174
    BadISR,                   //  94 - 0x178
    BadISR,                   //  95 - 0x17C
    BadISR,                   //  96 - 0x180
    BadISR,                   //  97 - 0x184
    BadISR,                   //  98 - 0x188
    BadISR,                   //  99 - 0x18C
    BadISR,                   // 100 - 0x190
};

#endif
