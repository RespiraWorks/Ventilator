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

#ifndef HAL_STM32_H_
#define HAL_STM32_H_

#if !defined(BARE_STM32)
#error                                                                         \
    "the header stm32.h is part of the HAL for the BARE_STM32 build.  It should only be used as part of that build"
#endif

#include <stdint.h>

#define CPU_FREQ_MHZ 80
#define CPU_FREQ (CPU_FREQ_MHZ * 1000000)

// Interrupt vectors that we currently use.
// The values here are the offsets into the interrupt table.
// These can be found in the NVIC chapter (chapter 12) of the
// processor reference manual
#define INT_VEC_TIMER15 0xA0
#define INT_VEC_UART2 0x0D8
#define INT_VEC_UART3 0x0DC
#define INT_VEC_TIMER6 0x118

// Represents a 32-bit register
typedef volatile uint32_t REG;

// 16-bit short register
typedef volatile uint16_t SREG;

// 8-bit byte sized register
typedef volatile uint8_t BREG;

// Interrupts on the STM32 are prioritized.  This allows
// more important interrupts to interrupt less important
// ones.  When interrupts are enabled we give a priority
// value to indicate how important the interrupt is.
// The lower the priority number the more important the
// interrupt.  The range is 0 to 15, but I only use a few
// here.  Hard faults, NMI, resets, etc have a fixed
// priority of -1, so they can always interrupt any other
// priority level.
enum class IntPriority {
  CRITICAL = 2, // Very important interrupt
  STANDARD = 5, // Normal hardware interrupts
  LOW = 8,      // Less important.  Hardware interrutps can interrupt this
};

///////////////////////////////////////////////////////////////
// The structures below represent the STM32 registers used
// to configure various modules (like timers, serial ports, etc).
// Detailed information on these modules and the registers
// used to configure them can be found in the reference
// manual for this chip.
///////////////////////////////////////////////////////////////

// Reset & clock controller
struct RCC_Regs {
  REG clkCtrl;   // 0x00 clock control register (RCC_CR)
  REG clkCal;    // 0x04 Internal clock sources calibration register (RCC_ICSCR)
  REG clkCfg;    // 0x08 clock configuration register (RCC_CFGR)
  REG pllCfg;    // 0x0C PLL configuration register (RCC_PLLCFGR)
  REG pllSaiCfg; // 0x10 PLLSAI1 configuration register (RCC_PLLSAI1CFGR)
  REG rsvd1;
  REG clkIntEna; // 0x18 Clock interrupt enable register ( RCC_CIER)
  REG clkIntFlg; // 0x1C Clock interrupt flag register ( RCC_CIFR)
  REG clkIntClr; // 0x20 Clock interrupt clear register ( RCC_CICR)
  REG rsvd2;
  REG periphReset[8];  // 0x28 peripheral reset registers
  REG periphClkEna[8]; // 0x48 peripheral clock registers
  REG sleepClkEna[8];  // 0x68 Clock enable in sleep
  REG indClkCfg; // 0x88 Peripherals independent clock configuration register
                 // (RCC_CCIPR)
  REG rsvd3;
  REG backup;     // 0x90 Backup domain control register (RCC_BDCR)
  REG status;     // 0x94 control & status register (RCC_CSR)
  REG recovery;   // 0x98 Clock recovery RC register (RCC_CRRCR)
  REG indClkCfg2; // 0x9C Peripherals independent clock configuration register
                  // (RCC_CCIPR2)
};
inline RCC_Regs *const RCC_BASE = reinterpret_cast<RCC_Regs *>(0x40021000);

// System control registers
struct SysCtrl_Reg {
  REG rsvd0;      // 0xE000E000
  REG intType;    // 0xE000E004
  REG auxCtrl;    // 0xE000E008
  REG rsvd1;      // 0xE000E00C
  REG systick[3]; // 0xE000E010
  REG rsvd2[57];
  REG nvic[768];     // 0xE000E100
  REG cpuid;         // 0xE000ED00
  REG intCtrl;       // 0xE000ED04
  REG vtable;        // 0xE000ED08
  REG apInt;         // 0xE000ED0C
  REG sysCtrl;       // 0xE000ED10
  REG cfgCtrl;       // 0xE000ED14
  REG sysPri[3];     // 0xE000ED18
  REG sysHdnCtrl;    // 0xE000ED24
  REG faultStat;     // 0xE000ED28
  REG hardFaultStat; // 0xE000ED2C
  REG rsvd3;
  REG mmFaultAddr; // 0xE000ED34
  REG faultAddr;   // 0xE000ED38
  REG rsvd4[19];
  REG cpac; // 0xE000ED88
};
inline SysCtrl_Reg *const SYSCTL_BASE =
    reinterpret_cast<SysCtrl_Reg *>(0xE000E000);

// Interrupt controller
struct IntCtrl_Regs {
  REG setEna[32];
  REG clrEna[32];
  REG setPend[32];
  REG clrPend[32];
  REG active[64];
  BREG priority[1024];
};
inline IntCtrl_Regs *const NVIC_BASE =
    reinterpret_cast<IntCtrl_Regs *>(0xE000E100);

struct UART_Regs {
  REG ctrl[3];
  REG baud;
  REG guard;
  REG timeout;
  REG request;
  REG status;
  REG intClear;
  REG rxDat;
  REG txDat;
};
inline UART_Regs *const UART1_BASE = reinterpret_cast<UART_Regs *>(0x40013800);
inline UART_Regs *const UART2_BASE = reinterpret_cast<UART_Regs *>(0x40004400);
inline UART_Regs *const UART3_BASE = reinterpret_cast<UART_Regs *>(0x40004800);
inline UART_Regs *const UART4_BASE = reinterpret_cast<UART_Regs *>(0x40004C00);

struct ADC_Regs {
  // A/D specific registers (0x100 total length)
  struct {
    REG stat;    // 0x00 - interrupt and status register (ADC_ISR)
    REG intEna;  // 0x04 - interrupt enable register (ADC_IER)
    REG ctrl;    // 0x08 - control register (ADC_CR)
    REG cfg[2];  // 0x0C - configuration registers
    REG samp[2]; // 0x14 - sampling time registers
    REG rsvd1;
    REG wdog[3]; // 0x20 - watchdog threshold registers
    REG rsvd2;
    REG seq[4]; // 0x30 - Regular sequence registers
    REG data;   // 0x40 - Regular data register
    REG rsvd3[2];
    REG iSeq; // 0x4C - Injected sequence regiseter
    REG rsvd4[4];
    REG offset[4]; // 0x60 - Offset registers
    REG rsvd5[4];
    REG iData[4]; // 0x80 - Injected channel data
    REG rsvd6[4];
    REG wdCfg[2]; // 0xA0 - Watchdog config
    REG rsvd7[2];
    REG diffSel; // 0xB0 - Differential mode selection
    REG cal;     // 0xB4 - Calibration factors
    REG rsvd8[18];
  } adc[2];

  // A/D common registers
  REG comStat; // 0x300 - Common status
  REG rsvd9;
  REG comCtrl; // 0x304 - Common control
  REG comData; // 0x308 - Common data
};
inline ADC_Regs *const ADC_BASE = reinterpret_cast<ADC_Regs *>(0X50040000);

struct TimerRegs {
  REG ctrl[2];
  REG slaveCtrl;
  REG intEna;
  REG status;
  REG event;
  REG ccMode[2];
  REG ccEnable;
  REG counter;
  REG prescale;
  REG reload;
  REG repeat;
  REG compare[4];
  REG deadTime;
  REG dmaCtrl;
  REG dmaAddr;
  REG opt1;
  REG ccMode3;
  REG compare5;
  REG compare6;
  REG opt2;
  REG opt3;
};
inline TimerRegs *const TIMER1_BASE = reinterpret_cast<TimerRegs *>(0x40012C00);
inline TimerRegs *const TIMER2_BASE = reinterpret_cast<TimerRegs *>(0x40000000);
inline TimerRegs *const TIMER3_BASE = reinterpret_cast<TimerRegs *>(0x40000400);
inline TimerRegs *const TIMER6_BASE = reinterpret_cast<TimerRegs *>(0x40001000);
inline TimerRegs *const TIMER15_BASE =
    reinterpret_cast<TimerRegs *>(0x40014000);
inline TimerRegs *const TIMER16_BASE =
    reinterpret_cast<TimerRegs *>(0x40014400);

struct FlashReg {
  REG access;
  REG pdKey;
  REG key;
  REG optKey;
  REG status;
  REG ctrl;
  REG ecc;
  REG rsvd1;
  REG option;
  REG pcropStart;
  REG pcropEnd;
  REG wrpA;
  REG wrpB;
};
inline FlashReg *const FLASH_BASE = reinterpret_cast<FlashReg *>(0x40022000);

struct DMA_Reg {
  REG intStat;
  REG intClr;
  struct {
    REG config;
    REG count;
    REG pAddr;
    REG mAddr;
    REG rsvd;
  } channel[7];
  REG rsvd[5];
  REG chanSel;
};
inline DMA_Reg *const DMA1_BASE = reinterpret_cast<DMA_Reg *>(0x40020000);
inline DMA_Reg *const DMA2_BASE = reinterpret_cast<DMA_Reg *>(0x40020400);

struct SPI_Regs {
  REG ctrl[2];
  REG status;
  REG data;
  REG crcPoly;
  REG rxCRC;
  REG txCRC;
};
inline SPI_Regs *const SPI1_BASE = reinterpret_cast<SPI_Regs *>(0x40013000);
inline SPI_Regs *const SPI2_BASE = reinterpret_cast<SPI_Regs *>(0x40003800);
inline SPI_Regs *const SPI3_BASE = reinterpret_cast<SPI_Regs *>(0x40003C00);

struct I2C_Regs {
  REG ctrl[2];
  REG addr[2];
  REG timing;
  REG timeout;
  REG status;
  REG intClr;
  REG pec;
  REG rxData;
  REG txData;
};
inline I2C_Regs *const I2C1_BASE = reinterpret_cast<I2C_Regs *>(0x40005400);
inline I2C_Regs *const I2C2_BASE = reinterpret_cast<I2C_Regs *>(0x40005800);
inline I2C_Regs *const I2C3_BASE = reinterpret_cast<I2C_Regs *>(0x40005c00);
inline I2C_Regs *const I2C4_BASE = reinterpret_cast<I2C_Regs *>(0x40008400);

// Watchdog timer
struct Watchdog_Regs {
  REG key;
  REG prescale;
  REG reload;
  REG status;
  REG window;
};
inline Watchdog_Regs *const WATCHDOG_BASE =
    reinterpret_cast<Watchdog_Regs *>(0x40003000);

// CRC calculation unit
struct CRC_Regs {
  REG data;
  REG scratch;
  REG ctrl;
  REG rsvd;
  REG init;
  REG poly;
};
inline CRC_Regs *const CRC_BASE = reinterpret_cast<CRC_Regs *>(0x40023000);

// General Purpose I/O
struct GPIO_Regs {
  REG mode;
  REG outType;
  REG outSpeed;
  REG pullUpDn;
  REG inDat;
  REG outDat;
  SREG set;
  SREG clr;
  REG lock;
  REG alt[2];
  REG reset;
};
inline GPIO_Regs *const GPIO_A_BASE = reinterpret_cast<GPIO_Regs *>(0x48000000);
inline GPIO_Regs *const GPIO_B_BASE = reinterpret_cast<GPIO_Regs *>(0x48000400);
inline GPIO_Regs *const GPIO_C_BASE = reinterpret_cast<GPIO_Regs *>(0x48000800);
inline GPIO_Regs *const GPIO_D_BASE = reinterpret_cast<GPIO_Regs *>(0x48000C00);
inline GPIO_Regs *const GPIO_E_BASE = reinterpret_cast<GPIO_Regs *>(0x48001000);
inline GPIO_Regs *const GPIO_H_BASE = reinterpret_cast<GPIO_Regs *>(0x48001C00);

// Handy functions for controlling GPIO
enum class GPIO_PinMode {
  IN = 0,
  OUT = 1,
  ALT = 2,
  ANALOG = 3,
};
inline void GPIO_PinMode(GPIO_Regs *const gpio, int pin, GPIO_PinMode mode) {
  gpio->mode &= ~(3 << (pin * 2));
  gpio->mode |= ((int)mode << (pin * 2));
}

enum class GPIO_OutType { PUSHPULL = 0, OPENDRAIN = 1 };
inline void GPIO_OutType(GPIO_Regs *const gpio, int pin, GPIO_OutType outType) {
  if (outType == GPIO_OutType::OPENDRAIN)
    gpio->outType |= 1 << pin;
  else
    gpio->outType &= ~(1 << pin);
}

inline void GPIO_PinAltFunc(GPIO_Regs *const gpio, int pin, int func) {
  GPIO_PinMode(gpio, pin, GPIO_PinMode::ALT);

  int x = (pin < 8) ? 0 : 1;
  gpio->alt[x] |= (func << ((pin & 7) * 4));
}

inline void GPIO_SetPin(GPIO_Regs *const gpio, int pin) {
  gpio->set = static_cast<SREG>(1 << pin);
}

inline void GPIO_ClrPin(GPIO_Regs *const gpio, int pin) {
  gpio->clr = static_cast<SREG>(1 << pin);
}

inline int GPIO_GetPin(GPIO_Regs *const gpio, int pin) {
  return (gpio->inDat & (1 << pin)) ? 1 : 0;
}

inline void GPIO_PullUp(GPIO_Regs *const gpio, int pin) {
  uint32_t x = gpio->pullUpDn & ~(3 << (2 * pin));
  x |= 1 << (2 * pin);
  gpio->pullUpDn = x;
}

inline void GPIO_PullDn(GPIO_Regs *const gpio, int pin) {
  uint32_t x = gpio->pullUpDn & ~(3 << (2 * pin));
  x |= 2 << (2 * pin);
  gpio->pullUpDn = x;
}

#endif
