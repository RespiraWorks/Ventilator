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

#ifndef __HAL_STM32_REGS
#define __HAL_STM32_REGS

#include <stdint.h>

// Represents a 32-bit register
using REG = volatile uint32_t;

// 16-bit short register
using SREG = volatile uint16_t;

// 8-bit byte sized register
using BREG = volatile uint8_t;

/*
SOURCES OF TRUTH
================
The structures below represent the STM32 registers used
to configure various modules (like timers, serial ports, etc).

Detailed information on these modules and the registers
used to configure them can be found in the reference
manual for this chip

[RM] Reference Manual for the STM32L452 processor 
  https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

[PM] Programmer's manual for the Cortex M4 line of processors:
   https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf
*/

// [PM] 4.4 System control block (SCB) (pg 221)
struct RCC_Regs {       // <Offset> <Name>
  REG clkCtrl;          // 0x00 clock control register (RCC_CR)
  REG clkCal;           // 0x04 Internal clock sources calibration register (RCC_ICSCR)
  REG clkCfg;           // 0x08 clock configuration register (RCC_CFGR)
  REG pllCfg;           // 0x0C PLL configuration register (RCC_PLLCFGR)
  REG pllSaiCfg;        // 0x10 PLLSAI1 configuration register (RCC_PLLSAI1CFGR)
  REG rsvd1;            //      Reserved
  REG clkIntEna;        // 0x18 Clock interrupt enable register ( RCC_CIER)
  REG clkIntFlg;        // 0x1C Clock interrupt flag register ( RCC_CIFR)
  REG clkIntClr;        // 0x20 Clock interrupt clear register ( RCC_CICR)
  REG rsvd2;            //      Reserved
  REG periphReset[8];   // 0x28 peripheral reset registers
  REG periphClkEna[8];  // 0x48 peripheral clock registers
  REG sleepClkEna[8];   // 0x68 Clock enable in sleep
  REG indClkCfg;        // 0x88 Peripherals independent clock configuration register
                        // (RCC_CCIPR)
  REG rsvd3;            //      Reserved
  REG backup;           // 0x90 Backup domain control register (RCC_BDCR)
  REG status;           // 0x94 control & status register (RCC_CSR)
  REG recovery;         // 0x98 Clock recovery RC register (RCC_CRRCR)
  REG indClkCfg2;       // 0x9C Peripherals independent clock configuration register
                        // (RCC_CCIPR2)
};
inline RCC_Regs *const RCC_BASE = reinterpret_cast<RCC_Regs *>(0x40021000);

// [PM] 4.4 System control block (SCB) (pg 221)
struct SysCtrl_Reg {
  REG rsvd0;         // 0xE000E000
  REG intType;       // 0xE000E004
  REG auxCtrl;       // 0xE000E008 - Auxiliary Control Register
  REG rsvd1;         // 0xE000E00C
  REG systick[3];    // 0xE000E010 - SysTick Timer 
  REG rsvd2[57];
  REG nvic[768];     // 0xE000E100 - NVIC Register [RM] 4.3 (pg 208)
  REG cpuid;         // 0xE000ED00 - CPUID Base Register
  REG intCtrl;       // 0xE000ED04 - Interrupt Control and State Register
  REG vtable;        // 0xE000ED08 - Vector Table Offset Register   
  REG apInt;         // 0xE000ED0C - Application Interrupt and Reset Control Register
  REG sysCtrl;       // 0xE000ED10 - System Control Register
  REG cfgCtrl;       // 0xE000ED14 - Configuration and Control Register
  REG sysPri[3];     // 0xE000ED18 - System Handler Priority Registers (1-3)
  REG sysHdnCtrl;    // 0xE000ED24 - System Handler Control and State Register
  REG faultStat;     // 0xE000ED28 - Configurable Fault Status Register
  REG hardFaultStat; // 0xE000ED2C - Hard Fault Status Register
  REG rsvd3;
  REG mmFaultAddr;   // 0xE000ED34 - Memory Management Fault Address Register
  REG faultAddr;     // 0xE000ED38 - Bus Fault Address Register
  REG rsvd4[19];
  REG cpac;          // 0xE000ED88 - Coprocessor access control register
};
inline SysCtrl_Reg *const SYSCTL_BASE =
    reinterpret_cast<SysCtrl_Reg *>(0xE000E000);

// [PM] 4.3 Nested vectored interrupt controller (NVIC) (pg 208)
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

// [RM] 38.8 USART Registers (pg 1238)
struct UART_Regs {
  union {
    struct {
      REG ue : 1;     // USART enable
      REG uesm : 1;   // USART enable in Stop mode
      REG re : 1;     // Receiver enable
      REG te : 1;     // Transmitter enable
      REG idleie : 1; // IDLE interrupt enable
      REG rxneie : 1; // RXNE interrupt enable
      REG tcie : 1;   // Transmission complete interrupt enable
      REG txeie : 1;  // Transmit interrupt enable
      REG peie : 1;   // Parity Error interrupt enable
      REG ps : 1;     // Parity selection
      REG pce : 1;    // Parity control enable
      REG wake : 1;   // Receiver wakeup method
      REG m0 : 1;     // Word length 0
      REG mme : 1;    // Mute mode enable
      REG cmie : 1;   // Character match interrupt enable
      REG over8 : 1;  // Oversampling mode
      REG dedt : 5;   // Driver Enable de-assertion time
      REG deat : 5;   // Driver Enable assertion time
      REG rtoie : 1;  // Receiver timeout interrupt enable
      REG eobie : 1;  // End of Block interrupt enable
      REG m1 : 1;     // Word length 1
      REG rsvd : 3;
    } s;
    REG r;
  } ctrl1; // Control Register 1 (USART_CR1) [RM] 38.8.1 (pg 1238)
  union {
    struct {
      REG rsvd2 : 4;
      REG addm7 : 1;    // 7-bit Address Detection/4-bit Address Detection
      REG lbdl : 1;     // LIN break detection length
      REG lbdie : 1;    // LIN break detection interrupt enable
      REG rsvd1 : 1;
      REG lbcl : 1;     // Last bit clock pulse
      REG cpha : 1;     // Clock phase
      REG cpol : 1;     // Clock polarity
      REG clken : 1;    // Clock enable
      REG stop : 2;     // STOP bits
      REG linen : 1;    // LIN mode enable
      REG swap : 1;     // Swap TX/RX pins
      REG rxinv : 1;    // RX pin active level inversion
      REG txinv : 1;    // TX pin active level inversion
      REG datainv : 1;  // Binary data inversion
      REG msbfirst : 1; // Most significant bit first
      REG abren : 1;    // Auto baud rate enable
      REG abrmod : 2;   // Auto baud rate mode
      REG rtoen : 1;    // Receiver timeout enable
      REG addr : 8;     // used for character detection during normal reception
                        // This bit field can only be written when reception is
                        // disabled (RE = 0) or the USART is disabled (UE=0)
    } s;
    REG r;
  } ctrl2; // Control Register 2 (USART_CR2) [RM] 38.8.2 (pg 1241)
  union {
    struct {
      REG eie : 1;     // Error interrupt enable
      REG iren : 1;    // IrDA mode enable
      REG irlp : 1;    // IrDA low-power
      REG hdsel : 1;   // Half-duplex selection
      REG nack : 1;    // Smartcard NACK enable
      REG scen : 1;    // Smartcard mode enable
      REG dmar : 1;    // DMA enable receiver
      REG dmat : 1;    // DMA enable transmitter
      REG rtse : 1;    // RTS enable
      REG ctse : 1;    // CTS enable
      REG ctsie : 1;   // CTS interrupt enable
      REG onebit : 1;  // One sample bit method enable
      REG ovrdis : 1;  // Overrun Disable
      REG ddre : 1;    // DMA Disable on Reception Error
      REG dem : 1;     // Driver enable mode
      REG dep : 1;     // Driver enable polarity selection
      REG rsvd2 : 1;
      REG scarcnt : 3; // Smartcard auto-retry count
      REG wus : 2;     // Wakeup from Stop mode interrupt flag selection
      REG wufie : 1;   // Wakeup from Stop mode interrupt enable
      REG ucesm : 1;   // USART Clock Enable in Stop mode.
      REG tcbgtie : 1; // Transmission complete before guard time interrupt
                       // enable
      REG rsvd : 7;
    } s;
    REG r;
  } ctrl3; // Control Register 3 [RM] 38.8.3  (pg 1245)
  REG baud;
  REG guard;
  union {
    struct {
      REG rto : 24; // receiver timeout
      REG blen : 8; // block length
    } s;
    REG r;
  } timeout; // Receiver Timeout Register (USART_RTOR) [RM] 38.8.6 (pg 1251)
  union {
    struct {
      REG abrrq : 1; // auto baud rate request
      REG sbkrq : 1; // send break request
      REG mmrq : 1;  // mute  mode request
      REG rxfrq : 1; // receive data flush request
      REG txfrq : 1; // transmit data flush request
      REG rsvd : 27;
    } s;
    REG r;
  } request; // Request Register (USART_QRQ) [RM] 38.8.7 (pg 1252)
  union {
    struct {
      const REG pe : 1;    // parity error
      const REG fe : 1;    // framing error
      const REG nf : 1;    // START bit noise detection flag
      const REG ore : 1;   // overrun error
      const REG idle : 1;  // idle line detected
      const REG rxne : 1;  // read data register not empty
      const REG tc : 1;    // transmission complete
      const REG txe : 1;   // transmit data register empty
      const REG lbdf : 1;  // LIN break detection flag
      const REG ctsif : 1; // CTS interrupt flag
      const REG cts : 1;   // CTS flag
      const REG rtof : 1;  // receiver timeout
      const REG eobf : 1;  // end of block flag
      const REG rsvd : 1;
      const REG abre : 1;  // auto baud rate error
      const REG abrf : 1;  // auto baud rate flag
      const REG busy : 1;  // busy flag
      const REG cmf : 1;   // character match flag
      const REG sbkf : 1;  // send break flag
      const REG rwu : 1;   // receiver wakeup from Mute mode
      const REG wuf : 1;   // wakeup from Stop mode flag
      const REG teack : 1; // transmit enable acknowledge flag
      const REG reack : 1; // receive enable acknowledge flag
      const REG rsvd2 : 2;
      const REG tcbgt : 1; // trans. complete before guard time completion
      const REG rsvd3 : 6;
    } s;
    REG r;
  } status; // [RM] 38.8.8 Interrupt And Status Register (USART_ISR) (pg 1253)
  union {
    struct {
      REG pecf : 1;    // parity error clear flag
      REG fecf : 1;    // framing error clear flag
      REG ncf : 1;     // noise detected clear flag
      REG orecf : 1;   // overrun error clear flag
      REG idlecf : 1;  // idle line detected clear flag
      REG rsvd : 1;
      REG tccf : 1;    // transmission complete clear flag
      REG tcbgtcf : 1; // transmission completed before guard time clear fl.
      REG lbdcf : 1;   // LIN break detection clear flag
      REG ctscf : 1;   // CTS clear flag
      REG rsvd2 : 1;
      REG rtocf : 1;   // receiver timeout clear flag
      REG eobcf : 1;   // end of block clear flag
      REG rsvd3 : 4;
      REG cmcf : 1;    // character match clear flag
      REG rsvd4 : 2;
      REG wucf : 1;    // wakeup from stop mode clear flag
      REG rsvd5 : 11;
    } s;
    REG r;
  } intClear; // [RM] 38.8.9 Interrupt Flag Clear Register (USART_ICR) (pg 1257)
  REG rxDat;
  REG txDat;
};
inline UART_Regs *const UART1_BASE = reinterpret_cast<UART_Regs *>(0x40013800);
inline UART_Regs *const UART2_BASE = reinterpret_cast<UART_Regs *>(0x40004400);
inline UART_Regs *const UART3_BASE = reinterpret_cast<UART_Regs *>(0x40004800);
inline UART_Regs *const UART4_BASE = reinterpret_cast<UART_Regs *>(0x40004C00);

// [RM] 16.6 ADC Registers (for each ADC) (pg 450)
struct ADC_Regs {
  // A/D specific registers (0x100 total length)
  struct {
    REG stat;   // 0x00 - interrupt and status register (ADC_ISR)
    REG intEna; // 0x04 - interrupt enable register (ADC_IER)
    REG ctrl;   // 0x08 - control register (ADC_CR)
    struct {
      REG dmaen : 1;      // DMA enable
      REG dmacfg : 1;     // DMA config
      REG dfsdmcfg : 1;   //
      REG resolution : 2; // A/D resolution
      REG align : 1;      // Data alignment
      REG extsel : 4;     // External trigger selection
      REG exten : 2;      // External trigger enable
      REG ovrmod : 1;     // Over run mode
      REG cont : 1;       // Continuous conversion mode
      REG autdlh : 1;     // Delayed conversion mode
      REG rsvd1 : 1;      //
      REG discen : 1;     // Discontinuous mode
      REG discnum : 3;    // Discontinuous mode channel count
      REG jdiscen : 1;    // Discontinuous mode on injected channels
      REG jqm : 1;        // JSQR mode
      REG awd1sgl : 1;
      REG awd1en : 1;
      REG jawd1en : 1;
      REG jauto : 1;
      REG awd1ch : 5;
      REG jqdis : 1;
    } cfg1; // 0x0C - [RM] 16.6.4 ADC Configuration Register (ADC_CFGR) (pg 458)

    struct {
      REG rovse : 1; // bit 0    - regular oversampling enable
      REG jovse : 1; // bit 1    - injected oversampling enable
      REG ovsr : 3;  // bits 2-4 - oversampling ratio
      REG ovss : 4;  // bits 5-8 - oversampling shift (max 0b1000)
      REG trovs : 1; // bit 9    - triggered regular oversampling
      REG rovsm : 1; // bit 10   - regular oversampling mode
      REG rsvd : 21;
    } cfg2; // [RM] 16.6.5 ADC configuration register 2 (ADC_CFGR2) (pg 462)

    struct {
      REG smp0 : 3; // Sample time for channel 0
      REG smp1 : 3; // Sample time for channel 1
      REG smp2 : 3; // Sample time for channel 2
      REG smp3 : 3; // Sample time for channel 3
      REG smp4 : 3; // Sample time for channel 4
      REG smp5 : 3; // Sample time for channel 5
      REG smp6 : 3; // Sample time for channel 6
      REG smp7 : 3; // Sample time for channel 7
      REG smp8 : 3; // Sample time for channel 8
      REG smp9 : 3; // Sample time for channel 9
      REG rsvd : 2;

      REG smp10 : 3; // Sample time for channel 10
      REG smp11 : 3; // Sample time for channel 11
      REG smp12 : 3; // Sample time for channel 12
      REG smp13 : 3; // Sample time for channel 13
      REG smp14 : 3; // Sample time for channel 14
      REG smp15 : 3; // Sample time for channel 15
      REG smp16 : 3; // Sample time for channel 16
      REG smp17 : 3; // Sample time for channel 17
      REG smp18 : 3; // Sample time for channel 18
      REG rsvd2 : 5;
    } samp; // [RM] 16.6.6 ADC Sample Time Register 1 (ADC_SMPR1) (pg 464)

    REG rsvd1;
    REG wdog[3]; // 0x20 - [RM] 16.6.8 ADC Watchdog
                 // Threshold Register 1 (ADC_TR1) (pg 465)
    REG rsvd2;

    // 4x sequence registers.  These registers are used
    // to define the number of A/D readings and the
    // channel numbers being read.
    struct {
      // [RM] 16.6.11 ADC Regular Sequence Register 1 (ADC_SQR1) (pg 468)
      REG len : 6;
      REG sq1 : 6;
      REG sq2 : 6;
      REG sq3 : 6;
      REG sq4 : 6;
      REG rsvd1 : 2;

      // [RM] 16.6.12 ADC Regular Sequence Register 2 (ADC_SQR2) (pg 469)
      REG sq5 : 6;
      REG sq6 : 6;
      REG sq7 : 6;
      REG sq8 : 6;
      REG sq9 : 6;
      REG rsvd2 : 2;

      // [RM] 16.6.13 ADC regular sequence register 3 (ADC_SQR3) (pg 470)
      REG sq10 : 6;
      REG sq11 : 6;
      REG sq12 : 6;
      REG sq13 : 6;
      REG sq14 : 6;
      REG rsvd3 : 2;

      // [RM] 16.6.14 ADC Regular Sequence Register 4 (ADC_SQR4) (pg 471)
      REG sq15 : 6;
      REG sq16 : 6;
      REG rsvd4 : 20;
    } seq; // ADC Regular Sequence Register

    REG data; // 0x40 - Regular data register [RM] 16.6.15 (pg 471)
    REG rsvd3[2];
    REG iSeq; // 0x4C - Injected sequence regiseter [RM] 16.6.16 (pg 472)
    REG rsvd4[4];
    REG offset[4]; // 0x60 - Offset Register [RM] 16.6.17 (pg 473)
    REG rsvd5[4];
    REG iData[4]; // 0x80 - - Injected Channel Data Register [RM] 16.6.18 (pg 474)
    REG rsvd6[4];
    REG wdCfg[2]; // 0xA0 - Analog Watchdog Config Register [RM] 16.6.19 (pg 474)
    REG rsvd7[2];
    REG diffSel; // 0xB0 - Differential Mode Selection Register [RM] 16.6.21 (pg 476)
    REG cal;     // 0xB4 - Calibration Factors [RM] 16.6.22 (pg 476)
    REG rsvd8[18];
  } adc[2]; // Master ADC1, Slave ADC2

  // [RM] 16.7.1 ADC Common Registers (pg 477)
  REG comStat; // 0x300 - Common Status Register [RM] 16.7.1 (pg 477)
  REG rsvd9;
  REG comCtrl; // 0x304 - Common Control Register [RM] 16.7.2 (pg 479)
  REG comData; // 0x308 - Common Data Register for Dual Mode [RM] 16.7.3 (pg 482)
};
inline ADC_Regs *const ADC_BASE = reinterpret_cast<ADC_Regs *>(0X50040000);

// Timer Register
// NOTE: Offset values and applicable registers depend on timer used
struct TimerRegs {
  union {
    struct {
      REG cen : 1;  // counter enable
      REG udis : 1; // update disable
      REG urs : 1;  // update request source
      REG opm : 1;  // one-pulse mode
      REG dir : 1;  // direction
      REG cms : 2;  // center-aligned mode selection
      REG arpe : 1; // auto-reload preload enable
      REG ckd : 2;  // clock division
      REG rsvd : 1;
      REG uifremap : 1; // UIF status bit remapping
      REG rsvd2 : 20;
    } s;
    REG r;
  } ctrl1;
  REG ctrl2;        // Control Register 2
  REG slaveCtrl;    // Slave Mode Control Register
  REG intEna;       // DMA/Interrupt Enable Register
  REG status;       // Status Register
  REG event;        // Even Generation Register
  REG ccMode[2];    // Capture/Compare Mode Register (1,2)
  REG ccEnable;     // Capture/Compare Enable Register
  // The topmost bit of counter will contain contain UIFCOPY if UIFREMAP is
  // enabled, but *this register should not be decomposed into a bitfield
  // struct*.  The idea behind UIFREMAP is to read the counter plus the UIFCOPY
  // value atomically, in one go.
  REG counter;      // Counter
  REG prescale;     // Prescaler
  REG reload;       // Auto-reload Register
  REG repeat;       // Repetition Counter Register
  REG compare[4];   // Capture/Compare Register (1-4)
  REG deadTime;     // Break and Dead-time Register
  REG dmaCtrl;      // DMA Control Register
  REG dmaAddr;      // DMA Address for Full Transfer
  REG opt1;         // Option Register 1
  REG ccMode3;      // Capture/Compare Mode Register 3
  REG compare5;     // Capture/Compare Register 5
  REG compare6;     // Capture/Compare Register 6
  REG opt2;         // Option Register 2
  REG opt3;         // Option Register 3
};

// [RM] 26 Advanced-control Timers (TIM1) (pg 718)
inline TimerRegs *const TIMER1_BASE = reinterpret_cast<TimerRegs *>(0x40012C00);
// [RM] 27 General-purpose Timers (TIM2/TIM3) (pg 817)
inline TimerRegs *const TIMER2_BASE = reinterpret_cast<TimerRegs *>(0x40000000);
inline TimerRegs *const TIMER3_BASE = reinterpret_cast<TimerRegs *>(0x40000400);
// [RM] 29 Basic Timers (TIM6/TIM7) (pg 968)
inline TimerRegs *const TIMER6_BASE = reinterpret_cast<TimerRegs *>(0x40001000);
inline TimerRegs *const TIMER7_BASE = reinterpret_cast<TimerRegs *>(0x40001400);
// [RM] 28 General-purpose Timers (TIM15/TIM16) (pg 887)
inline TimerRegs *const TIMER15_BASE =
    reinterpret_cast<TimerRegs *>(0x40014000);
inline TimerRegs *const TIMER16_BASE =
    reinterpret_cast<TimerRegs *>(0x40014400);

// [RM] 3.7 Flash Registers (pg 100)
struct FlashReg {
  REG access;     // 0x00 - Access Control Register (FLASH_ACR
  REG pdKey;      // 0x04 - Power-down Key Register (FLASH_PDKEYR)
  REG key;        // 0x08 - Key Register (FLASH_KEYR)
  REG optKey;     // 0x0C - Option Key Register (FLASH_OPTKEYR)
  REG status;     // 0x10 - Status Register (FLASH_SR)
  REG ctrl;       // 0x14 - Control Register (FLASH_CR) 
  REG ecc;        // 0x18 - EEC Register (FLASH_EECR)
  REG rsvd1;      
  REG option;     // 0x20 - Option Register (FLASH_OPTR)
  REG pcropStart; // 0x24 - PCROP Start Address Register (FLASH_PCROP1SR)
  REG pcropEnd;   // 0x28 - PCROP End Address Register (FLASH_PCROP1ER)
  REG wrpA;       // 0x2C - WRP Area A Address Register (FLASH_WRP1AR)
  REG wrpB;       // 0x30 - WRP Area B Address Register (FLAS_WRP1BR)
};
inline FlashReg *const FLASH_BASE = reinterpret_cast<FlashReg *>(0x40022000);

// [RM] 11.4.4 DMA channels (pg 302)
enum class DMA_Chan {
  C1 = 0,
  C2 = 1,
  C3 = 2,
  C4 = 3,
  C5 = 4,
  C6 = 5,
  C7 = 6,
};

enum class DmaChannelDir { PERIPHERAL_TO_MEM = 0, MEM_TO_PERIPHERAL = 1 };
enum class DmaTransferSize { BITS8 = 0, BITS16 = 1, BITS32 = 2 };

struct DMA_Regs {
  union {
    struct {
      REG gif1 : 1;  // global interrupt flag
      REG tcif1 : 1; // transfer complete (TC) flag
      REG htif1 : 1; // half transfer (HT) flag
      REG teif1 : 1; // transfer error (TE) flag
      REG gif2 : 1;  // global interrupt flag
      REG tcif2 : 1; // transfer complete (TC) flag
      REG htif2 : 1; // half transfer (HT) flag
      REG teif2 : 1; // transfer error (TE) flag
      REG gif3 : 1;  // global interrupt flag
      REG tcif3 : 1; // transfer complete (TC) flag
      REG htif3 : 1; // half transfer (HT) flag
      REG teif3 : 1; // transfer error (TE) flag
      REG gif4 : 1;  // global interrupt flag
      REG tcif4 : 1; // transfer complete (TC) flag
      REG htif4 : 1; // half transfer (HT) flag
      REG teif4 : 1; // transfer error (TE) flag
      REG gif5 : 1;  // global interrupt flag
      REG tcif5 : 1; // transfer complete (TC) flag
      REG htif5 : 1; // half transfer (HT) flag
      REG teif5 : 1; // transfer error (TE) flag
      REG gif6 : 1;  // global interrupt flag
      REG tcif6 : 1; // transfer complete (TC) flag
      REG htif6 : 1; // half transfer (HT) flag
      REG teif6 : 1; // transfer error (TE) flag
      REG gif7 : 1;  // global interrupt flag
      REG tcif7 : 1; // transfer complete (TC) flag
      REG htif7 : 1; // half transfer (HT) flag
      REG teif7 : 1; // transfer error (TE) flag
      REG rsvd : 4;
    };
    REG r;
  } intStat; // Interrupt Status Register (DMA_ISR) [RM] 11.6.1 (pg 308)

  union {
    struct {
      REG gif1 : 1;  // global interrupt flag
      REG tcif1 : 1; // transfer complete (TC) flag
      REG htif1 : 1; // half transfer (HT) flag
      REG teif1 : 1; // transfer error (TE) flag
      REG gif2 : 1;  // global interrupt flag
      REG tcif2 : 1; // transfer complete (TC) flag
      REG htif2 : 1; // half transfer (HT) flag
      REG teif2 : 1; // transfer error (TE) flag
      REG gif3 : 1;  // global interrupt flag
      REG tcif3 : 1; // transfer complete (TC) flag
      REG htif3 : 1; // half transfer (HT) flag
      REG teif3 : 1; // transfer error (TE) flag
      REG gif4 : 1;  // global interrupt flag
      REG tcif4 : 1; // transfer complete (TC) flag
      REG htif4 : 1; // half transfer (HT) flag
      REG teif4 : 1; // transfer error (TE) flag
      REG gif5 : 1;  // global interrupt flag
      REG tcif5 : 1; // transfer complete (TC) flag
      REG htif5 : 1; // half transfer (HT) flag
      REG teif5 : 1; // transfer error (TE) flag
      REG gif6 : 1;  // global interrupt flag
      REG tcif6 : 1; // transfer complete (TC) flag
      REG htif6 : 1; // half transfer (HT) flag
      REG teif6 : 1; // transfer error (TE) flag
      REG gif7 : 1;  // global interrupt flag
      REG tcif7 : 1; // transfer complete (TC) flag
      REG htif7 : 1; // half transfer (HT) flag
      REG teif7 : 1; // transfer error (TE) flag
      REG rsvd : 4;
    };
    REG r;
  } intClr; // Interrupt Flag Clear Register [RM] 11.6.2 (pg 311)
  struct {
    struct {
      REG enable : 1;   // channel enable
      REG tcie : 1;     // transfer complete interrupt enable
      REG htie : 1;     // half transfer interrupt enable
      REG teie : 1;     // transfer error interrupt enable
      REG dir : 1;      // data xfer direction 0: per->mem, 1: mem->per
      REG circular : 1; // circular mode
      REG perInc : 1;   // peripheral increment mode
      REG memInc : 1;   // memory increment mode
      REG psize : 2;    // peripheral size 0b00 - 8bits, 0b10 - 32bits
      REG msize : 2;    // memory size 0b00 - 8bits, 0b10 - 32bits
      REG priority : 2; // priority level 0b00 - low, 0b11 - high
      REG mem2mem : 1;  // memory-to-memory mode
      REG rsvd : 17;
    } config;             // channel x configuration register [RM] 11.6.3 (pg 312)
    REG count;            // channel x number of data to transfer register
    volatile void *pAddr; // channel x peripheral address register
    volatile void *mAddr; // channel x memory address register
    REG rsvd;             // reserved
  } channel[7];
  REG rsvd[5]; // reserved
  union {
    struct {
      REG c1s : 4;
      REG c2s : 4;
      REG c3s : 4;
      REG c4s : 4;
      REG c5s : 4;
      REG c6s : 4;
      REG c7s : 4;
      REG rsvd : 4;
    };
    REG r;
  } chanSel; // Channel Selection Register [RM] 11.6.7 (pg 317)
};
inline DMA_Regs *const DMA1_BASE = reinterpret_cast<DMA_Regs *>(0x40020000);
inline DMA_Regs *const DMA2_BASE = reinterpret_cast<DMA_Regs *>(0x40020400);

/* Select the source for a DMA channel:
  @param dma        Address of DMA registers
  @param chan       DMA channel to modify.  Channels are numbered from 0
  @param selection  Selects which peripherial request to map to the channel
  
  DMA request mapping [RM] 11.3.2 (pg 296)
  
  Channel Selection Register [RM] 11.6.7 (pg 317)
    Bits 27:24 C7S[3:0]: DMA channel 7 selection
    Bits 23:20 C6S[3:0]: DMA channel 6 selection
    Bits 19:16 C5S[3:0]: DMA channel 5 selection
    Bits 15:12 C4S[3:0]: DMA channel 4 selection 
    Bits 11:8 C3S[3:0]:  DMA channel 3 selection
    Bits 7:4 C2S[3:0]:   DMA channel 2 selection         
    Bits 3:0 C1S[3:0]:   DMA channel 1 selection
    
  selection &= 0x0F                 Converts selection to binary and truncates 
                                      digits after 4th bit
  val = dma->chanSel.r              Holds DMA Channel Selection Register
  2^(4 * chan)                        MSB position
  val &= ~(0xF * 2^(4 * chan))      Clears bits storing channel selection on register
  val |= selection * 2^(4 * chan)   Writes selection to channel selection on register
  dma->chanSel.r = val              Write changes to channel selection register
*/

inline void DMA_SelectChannel(DMA_Regs *dma, DMA_Chan chan, int selection) {
  selection &= 0x0F;

  int x = 4 * static_cast<int>(chan);

  uint32_t val = dma->chanSel.r;
  val &= ~(0xF << x);
  val |= selection << x;
  dma->chanSel.r = val;
}

enum class DmaInterrupt {
  GLOBAL = 1,
  XFER_COMPLETE = 2,
  HALF_COMPLETE = 4,
  XFER_ERR = 8
};

/* Clear a DMA Interrupt 
  @param dma        Address of DMA registers
  @param chan       DMA channel to modify
  @param interrupt  DMA interrupt event 

  DMA interrupt flag clear register (DMA_IFCR) [RM] 11.6.2
  Bit i             global interrupt flag clear for channel i
  Bit i+1           transfer complete flag clear for channel i
  Bit i+2           half transfer flag clear for channel i
  Bit i+3           transfer error flag clear for channel i

  2^(4 * chan)                DMA interrupt event flag clear bits for channel
  interrupt * 2^(4 * chan)    bit that matches appropriate interrupt event for channel
*/
inline void DMA_ClearInt(DMA_Regs *dma, DMA_Chan chan, DmaInterrupt interrupt) {

  int x = static_cast<int>(interrupt);
  x <<= 4 * static_cast<int>(chan);
  dma->intClr.r = x;
}

struct SPI_Regs {
  struct {
    REG cpha : 1;       // Clock phase
    REG cpol : 1;       // Clock polarity
    REG mstr : 1;       // Master mode
    REG br : 3;         // Bit rate
    REG spe : 1;        // SPI enable
    REG lsb_first : 1;  // Send LSB first
    REG ssi : 1;        // Internal slave select
    REG ssm : 1;        // Software slave management
    REG rx_only : 1;    // Receive mode only
    REG crcl : 1;       // CRC length
    REG crc_next : 1;   // CRC transmit next
    REG crc_ena : 1;    // CRC enable
    REG bidir_oe : 1;   // Bidirectional output enable
    REG bidir_mode : 1; // Bidirectional data mode enable
    REG rsvd : 16;
  } ctrl1;
  struct {
    REG rx_dma_en : 1; // Receive buffer DMA enable
    REG tx_dma_en : 1; // Transmit buffer DMA enable
    REG ssoe : 1;      // SS output enable
    REG nssp : 1;      // NSS pulse management
    REG frf : 1;       // Frame format
    REG err_ie : 1;    // Error interrupt enable
    REG rxne_ie : 1;   // Recieve buffer not empty int enable
    REG txe_ie : 1;    // Transmit buffer empty int enable
    REG ds : 4;        // Data size
    REG frxth : 1;     // FIFO reception threshold
    REG ldma_rx : 1;   // Last DMA xfer for receive
    REG ldma_tx : 1;   // Last DMA xfer for transmit
    REG rsvd : 17;
  } ctrl2;
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

#endif
