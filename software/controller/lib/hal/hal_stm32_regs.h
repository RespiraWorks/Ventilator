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

#ifndef __HAL_STM32_REGS
#define __HAL_STM32_REGS

#include <stdint.h>

/*
The structures below represent the STM32 registers used
to configure various modules (like timers, serial ports, etc).

Detailed information on these modules and the registers
used to configure them can be found in [RM]

Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
*/

// [PM] 4.4 System control block (SCB) (pg 221)
struct RCC_Regs_ {    // <Offset> <Name>
  uint32_t clkCtrl;   // 0x00 clock control register (RCC_CR)
  uint32_t clkCal;    // 0x04 Internal clock sources calibration register
                      // (RCC_ICSCR)
  uint32_t clkCfg;    // 0x08 clock configuration register (RCC_CFGR)
  uint32_t pllCfg;    // 0x0C PLL configuration register (RCC_PLLCFGR)
  uint32_t pllSaiCfg; // 0x10 PLLSAI1 configuration register (RCC_PLLSAI1CFGR)
  uint32_t rsvd1;
  uint32_t clkIntEna; // 0x18 Clock interrupt enable register ( RCC_CIER)
  uint32_t clkIntFlg; // 0x1C Clock interrupt flag register ( RCC_CIFR)
  uint32_t clkIntClr; // 0x20 Clock interrupt clear register ( RCC_CICR)
  uint32_t rsvd2;
  uint32_t periphReset[8];  // 0x28 peripheral reset registers
  uint32_t periphClkEna[8]; // 0x48 peripheral clock registers
  uint32_t sleepClkEna[8];  // 0x68 Clock enable in sleep
  uint32_t indClkCfg;       // 0x88 Peripherals independent clock configuration
                            // register (RCC_CCIPR)
  uint32_t rsvd3;
  uint32_t backup;     // 0x90 Backup domain control register (RCC_BDCR)
  uint32_t status;     // 0x94 control & status register (RCC_CSR)
  uint32_t recovery;   // 0x98 Clock recovery RC register (RCC_CRRCR)
  uint32_t indClkCfg2; // 0x9C Peripherals independent clock configuration
                       // register
                       // (RCC_CCIPR2)
};
typedef volatile RCC_Regs_ RCC_Regs;
inline RCC_Regs *const RCC_BASE = reinterpret_cast<RCC_Regs *>(0x40021000);

// [PM] 4.4 System control block (SCB) (pg 221)
struct SysCtrl_Reg_ {
  uint32_t rsvd0;      // 0xE000E000
  uint32_t intType;    // 0xE000E004
  uint32_t auxCtrl;    // 0xE000E008 - Auxiliary Control Register
  uint32_t rsvd1;      // 0xE000E00C
  uint32_t systick[3]; // 0xE000E010 - SysTick Timer
  uint32_t rsvd2[57];
  uint32_t nvic[768];  // 0xE000E100 - NVIC Register [RM] 4.3 (pg 208)
  uint32_t cpuid;      // 0xE000ED00 - CPUID Base Register
  uint32_t intCtrl;    // 0xE000ED04 - Interrupt Control and State Register
  uint32_t vtable;     // 0xE000ED08 - Vector Table Offset Register
  uint32_t apInt;      // 0xE000ED0C - Application Interrupt and Reset Control
                       // Register
  uint32_t sysCtrl;    // 0xE000ED10 - System Control Register
  uint32_t cfgCtrl;    // 0xE000ED14 - Configuration and Control Register
  uint32_t sysPri[3];  // 0xE000ED18 - System Handler Priority Registers (1-3)
  uint32_t sysHdnCtrl; // 0xE000ED24 - System Handler Control and State
                       // Register
  uint32_t faultStat;  // 0xE000ED28 - Configurable Fault Status Register
  uint32_t hardFaultStat; // 0xE000ED2C - Hard Fault Status Register
  uint32_t rsvd3;
  uint32_t mmFaultAddr; // 0xE000ED34 - Memory Management Fault Address
                        // Register
  uint32_t faultAddr;   // 0xE000ED38 - Bus Fault Address Register
  uint32_t rsvd4[19];
  uint32_t cpac; // 0xE000ED88 - Coprocessor access control register
};
typedef volatile SysCtrl_Reg_ SysCtrl_Reg;
inline SysCtrl_Reg *const SYSCTL_BASE =
    reinterpret_cast<SysCtrl_Reg *>(0xE000E000);

// [PM] 4.3 Nested vectored interrupt controller (NVIC) (pg 208)
struct IntCtrl_Regs_ {
  uint32_t setEna[32];
  uint32_t clrEna[32];
  uint32_t setPend[32];
  uint32_t clrPend[32];
  uint32_t active[64];
  uint8_t priority[1024];
};
typedef volatile IntCtrl_Regs_ IntCtrl_Regs;
inline IntCtrl_Regs *const NVIC_BASE =
    reinterpret_cast<IntCtrl_Regs *>(0xE000E100);

// [RM] 38.8 USART Registers (pg 1238)
struct UART_Regs_ {
  union {
    struct {
      uint32_t ue : 1;     // USART enable
      uint32_t uesm : 1;   // USART enable in Stop mode
      uint32_t re : 1;     // Receiver enable
      uint32_t te : 1;     // Transmitter enable
      uint32_t idleie : 1; // IDLE interrupt enable
      uint32_t rxneie : 1; // RXNE interrupt enable
      uint32_t tcie : 1;   // Transmission complete interrupt enable
      uint32_t txeie : 1;  // Transmit interrupt enable
      uint32_t peie : 1;   // Parity Error interrupt enable
      uint32_t ps : 1;     // Parity selection
      uint32_t pce : 1;    // Parity control enable
      uint32_t wake : 1;   // Receiver wakeup method
      uint32_t m0 : 1;     // Word length 0
      uint32_t mme : 1;    // Mute mode enable
      uint32_t cmie : 1;   // Character match interrupt enable
      uint32_t over8 : 1;  // Oversampling mode
      uint32_t dedt : 5;   // Driver Enable de-assertion time
      uint32_t deat : 5;   // Driver Enable assertion time
      uint32_t rtoie : 1;  // Receiver timeout interrupt enable
      uint32_t eobie : 1;  // End of Block interrupt enable
      uint32_t m1 : 1;     // Word length 1
      uint32_t rsvd : 3;
    } s;
    uint32_t r;
  } ctrl1; // Control Register 1 (USART_CR1) [RM] 38.8.1 (pg 1238)
  union {
    struct {
      uint32_t rsvd2 : 4;
      uint32_t addm7 : 1; // 7-bit Address Detection/4-bit Address
                          // Detection
      uint32_t lbdl : 1;  // LIN break detection length
      uint32_t lbdie : 1; // LIN break detection interrupt enable
      uint32_t rsvd1 : 1;
      uint32_t lbcl : 1;     // Last bit clock pulse
      uint32_t cpha : 1;     // Clock phase
      uint32_t cpol : 1;     // Clock polarity
      uint32_t clken : 1;    // Clock enable
      uint32_t stop : 2;     // STOP bits
      uint32_t linen : 1;    // LIN mode enable
      uint32_t swap : 1;     // Swap TX/RX pins
      uint32_t rxinv : 1;    // RX pin active level inversion
      uint32_t txinv : 1;    // TX pin active level inversion
      uint32_t datainv : 1;  // Binary data inversion
      uint32_t msbfirst : 1; // Most significant bit first
      uint32_t abren : 1;    // Auto baud rate enable
      uint32_t abrmod : 2;   // Auto baud rate mode
      uint32_t rtoen : 1;    // Receiver timeout enable
      uint32_t addr : 8;     // used for character detection during normal
                             // reception This bit field can only be written
                             // when reception is disabled (RE = 0) or the
                             // USART is disabled (UE=0)
    } s;
    uint32_t r;
  } ctrl2; // Control Register 2 (USART_CR2) [RM] 38.8.2 (pg 1241)
  union {
    struct {
      uint32_t eie : 1;    // Error interrupt enable
      uint32_t iren : 1;   // IrDA mode enable
      uint32_t irlp : 1;   // IrDA low-power
      uint32_t hdsel : 1;  // Half-duplex selection
      uint32_t nack : 1;   // Smartcard NACK enable
      uint32_t scen : 1;   // Smartcard mode enable
      uint32_t dmar : 1;   // DMA enable receiver
      uint32_t dmat : 1;   // DMA enable transmitter
      uint32_t rtse : 1;   // RTS enable
      uint32_t ctse : 1;   // CTS enable
      uint32_t ctsie : 1;  // CTS interrupt enable
      uint32_t onebit : 1; // One sample bit method enable
      uint32_t ovrdis : 1; // Overrun Disable
      uint32_t ddre : 1;   // DMA Disable on Reception Error
      uint32_t dem : 1;    // Driver enable mode
      uint32_t dep : 1;    // Driver enable polarity selection
      uint32_t rsvd2 : 1;
      uint32_t scarcnt : 3; // Smartcard auto-retry count
      uint32_t wus : 2;     // Wakeup from Stop mode interrupt flag selection
      uint32_t wufie : 1;   // Wakeup from Stop mode interrupt enable
      uint32_t ucesm : 1;   // USART Clock Enable in Stop mode.
      uint32_t tcbgtie : 1; // Transmission complete before guard time
                            // interrupt enable
      uint32_t rsvd : 7;
    } s;
    uint32_t r;
  } ctrl3; // Control Register 3 [RM] 38.8.3  (pg 1245)
  uint32_t baud;
  uint32_t guard;
  union {
    struct {
      uint32_t rto : 24; // receiver timeout
      uint32_t blen : 8; // block length
    } s;
    uint32_t r;
  } timeout; // Receiver Timeout Register (USART_RTOR) [RM] 38.8.6 (pg 1251)
  union {
    struct {
      uint32_t abrrq : 1; // auto baud rate request
      uint32_t sbkrq : 1; // send break request
      uint32_t mmrq : 1;  // mute  mode request
      uint32_t rxfrq : 1; // receive data flush request
      uint32_t txfrq : 1; // transmit data flush request
      uint32_t rsvd : 27;
    } s;
    uint32_t r;
  } request; // Request Register (USART_QRQ) [RM] 38.8.7 (pg 1252)
  union {
    struct {
      const uint32_t pe : 1;    // parity error
      const uint32_t fe : 1;    // framing error
      const uint32_t nf : 1;    // START bit noise detection flag
      const uint32_t ore : 1;   // overrun error
      const uint32_t idle : 1;  // idle line detected
      const uint32_t rxne : 1;  // read data register not empty
      const uint32_t tc : 1;    // transmission complete
      const uint32_t txe : 1;   // transmit data register empty
      const uint32_t lbdf : 1;  // LIN break detection flag
      const uint32_t ctsif : 1; // CTS interrupt flag
      const uint32_t cts : 1;   // CTS flag
      const uint32_t rtof : 1;  // receiver timeout
      const uint32_t eobf : 1;  // end of block flag
      const uint32_t rsvd : 1;
      const uint32_t abre : 1;  // auto baud rate error
      const uint32_t abrf : 1;  // auto baud rate flag
      const uint32_t busy : 1;  // busy flag
      const uint32_t cmf : 1;   // character match flag
      const uint32_t sbkf : 1;  // send break flag
      const uint32_t rwu : 1;   // receiver wakeup from Mute mode
      const uint32_t wuf : 1;   // wakeup from Stop mode flag
      const uint32_t teack : 1; // transmit enable acknowledge flag
      const uint32_t reack : 1; // receive enable acknowledge flag
      const uint32_t rsvd2 : 2;
      const uint32_t tcbgt : 1; // trans. complete before guard time
                                // completion
      const uint32_t rsvd3 : 6;
    } s;
    uint32_t r;
  } status; // [RM] 38.8.8 Interrupt And Status Register (USART_ISR) (pg 1253)
  union {
    struct {
      uint32_t pecf : 1;   // parity error clear flag
      uint32_t fecf : 1;   // framing error clear flag
      uint32_t ncf : 1;    // noise detected clear flag
      uint32_t orecf : 1;  // overrun error clear flag
      uint32_t idlecf : 1; // idle line detected clear flag
      uint32_t rsvd : 1;
      uint32_t tccf : 1;    // transmission complete clear flag
      uint32_t tcbgtcf : 1; // transmission completed before guard time
                            // clear fl.
      uint32_t lbdcf : 1;   // LIN break detection clear flag
      uint32_t ctscf : 1;   // CTS clear flag
      uint32_t rsvd2 : 1;
      uint32_t rtocf : 1; // receiver timeout clear flag
      uint32_t eobcf : 1; // end of block clear flag
      uint32_t rsvd3 : 4;
      uint32_t cmcf : 1; // character match clear flag
      uint32_t rsvd4 : 2;
      uint32_t wucf : 1; // wakeup from stop mode clear flag
      uint32_t rsvd5 : 11;
    } s;
    uint32_t r;
  } intClear; // [RM] 38.8.9 Interrupt Flag Clear Register (USART_ICR) (pg
              // 1257)
  uint32_t rxDat;
  uint32_t txDat;
};
typedef volatile UART_Regs_ UART_Regs;
inline UART_Regs *const UART1_BASE = reinterpret_cast<UART_Regs *>(0x40013800);
inline UART_Regs *const UART2_BASE = reinterpret_cast<UART_Regs *>(0x40004400);
inline UART_Regs *const UART3_BASE = reinterpret_cast<UART_Regs *>(0x40004800);
inline UART_Regs *const UART4_BASE = reinterpret_cast<UART_Regs *>(0x40004C00);

// [RM] 16.6 ADC Registers (for each ADC) (pg 450)
struct ADC_Regs_ {
  // A/D specific registers (0x100 total length)
  struct {
    uint32_t stat;   // 0x00 - interrupt and status register (ADC_ISR)
    uint32_t intEna; // 0x04 - interrupt enable register (ADC_IER)
    uint32_t ctrl;   // 0x08 - control register (ADC_CR)
    struct {
      uint32_t dmaen : 1;      // DMA enable
      uint32_t dmacfg : 1;     // DMA config
      uint32_t dfsdmcfg : 1;   //
      uint32_t resolution : 2; // A/D resolution
      uint32_t align : 1;      // Data alignment
      uint32_t extsel : 4;     // External trigger selection
      uint32_t exten : 2;      // External trigger enable
      uint32_t ovrmod : 1;     // Over run mode
      uint32_t cont : 1;       // Continuous conversion mode
      uint32_t autdlh : 1;     // Delayed conversion mode
      uint32_t rsvd1 : 1;      //
      uint32_t discen : 1;     // Discontinuous mode
      uint32_t discnum : 3;    // Discontinuous mode channel count
      uint32_t jdiscen : 1;    // Discontinuous mode on injected channels
      uint32_t jqm : 1;        // JSQR mode
      uint32_t awd1sgl : 1;
      uint32_t awd1en : 1;
      uint32_t jawd1en : 1;
      uint32_t jauto : 1;
      uint32_t awd1ch : 5;
      uint32_t jqdis : 1;
    } cfg1; // 0x0C - [RM] 16.6.4 ADC Configuration Register (ADC_CFGR) (pg
            // 458)

    struct {
      uint32_t rovse : 1; // bit 0    - regular oversampling enable
      uint32_t jovse : 1; // bit 1    - injected oversampling enable
      uint32_t ovsr : 3;  // bits 2-4 - oversampling ratio
      uint32_t ovss : 4;  // bits 5-8 - oversampling shift (max 0b1000)
      uint32_t trovs : 1; // bit 9    - triggered regular oversampling
      uint32_t rovsm : 1; // bit 10   - regular oversampling mode
      uint32_t rsvd : 21;
    } cfg2; // [RM] 16.6.5 ADC configuration register 2 (ADC_CFGR2) (pg 462)

    struct {
      uint32_t smp0 : 3; // Sample time for channel 0
      uint32_t smp1 : 3; // Sample time for channel 1
      uint32_t smp2 : 3; // Sample time for channel 2
      uint32_t smp3 : 3; // Sample time for channel 3
      uint32_t smp4 : 3; // Sample time for channel 4
      uint32_t smp5 : 3; // Sample time for channel 5
      uint32_t smp6 : 3; // Sample time for channel 6
      uint32_t smp7 : 3; // Sample time for channel 7
      uint32_t smp8 : 3; // Sample time for channel 8
      uint32_t smp9 : 3; // Sample time for channel 9
      uint32_t rsvd : 2;

      uint32_t smp10 : 3; // Sample time for channel 10
      uint32_t smp11 : 3; // Sample time for channel 11
      uint32_t smp12 : 3; // Sample time for channel 12
      uint32_t smp13 : 3; // Sample time for channel 13
      uint32_t smp14 : 3; // Sample time for channel 14
      uint32_t smp15 : 3; // Sample time for channel 15
      uint32_t smp16 : 3; // Sample time for channel 16
      uint32_t smp17 : 3; // Sample time for channel 17
      uint32_t smp18 : 3; // Sample time for channel 18
      uint32_t rsvd2 : 5;
    } samp; // [RM] 16.6.6 ADC Sample Time Register 1 (ADC_SMPR1) (pg 464)

    uint32_t rsvd1;
    uint32_t wdog[3]; // 0x20 - [RM] 16.6.8 ADC Watchdog
    uint32_t rsvd2;

    // 4x sequence registers.  These registers are used
    // to define the number of A/D readings and the
    // channel numbers being read.
    struct {
      // [RM] 16.6.11 ADC Regular Sequence Register 1 (ADC_SQR1) (pg 468)
      uint32_t len : 6;
      uint32_t sq1 : 6;
      uint32_t sq2 : 6;
      uint32_t sq3 : 6;
      uint32_t sq4 : 6;
      uint32_t rsvd1 : 2;

      // [RM] 16.6.12 ADC Regular Sequence Register 2 (ADC_SQR2) (pg 469)
      uint32_t sq5 : 6;
      uint32_t sq6 : 6;
      uint32_t sq7 : 6;
      uint32_t sq8 : 6;
      uint32_t sq9 : 6;
      uint32_t rsvd2 : 2;

      // [RM] 16.6.13 ADC regular sequence register 3 (ADC_SQR3) (pg 470)
      uint32_t sq10 : 6;
      uint32_t sq11 : 6;
      uint32_t sq12 : 6;
      uint32_t sq13 : 6;
      uint32_t sq14 : 6;
      uint32_t rsvd3 : 2;

      // [RM] 16.6.14 ADC Regular Sequence Register 4 (ADC_SQR4) (pg 471)
      uint32_t sq15 : 6;
      uint32_t sq16 : 6;
      uint32_t rsvd4 : 20;
    } seq; // ADC Regular Sequence Register

    uint32_t data; // 0x40 - Regular Data Register [RM] 16.6.15 (pg 471)
    uint32_t rsvd3[2];
    uint32_t iSeq; // 0x4C - Injected Sequence Register [RM] 16.6.16 (pg
                   // 472)
    uint32_t rsvd4[4];
    uint32_t offset[4]; // 0x60 - Offset Register [RM] 16.6.17 (pg 473)
    uint32_t rsvd5[4];
    uint32_t iData[4]; // 0x80 - Injected Chan Data Reg [RM] 16.6.18 (pg
                       // 474)
    uint32_t rsvd6[4];
    uint32_t wdCfg[2]; // 0xA0 - Analog Watchdog Config Reg [RM] 16.6.19 (pg
                       // 474)
    uint32_t rsvd7[2];
    uint32_t diffSel; // 0xB0 - Differential Mode Selection Reg [RM] 16.6.21
                      // (pg 476)
    uint32_t cal;     // 0xB4 - Calibration Factors [RM] 16.6.22 (pg 476)
    uint32_t rsvd8[18];
  } adc[2]; // Master ADC1, Slave ADC2

  uint32_t comStat; // 0x300 - Common Status Register [RM] 16.7.1 (pg 477)
  uint32_t rsvd9;
  uint32_t comCtrl; // 0x304 - Common Control Register [RM] 16.7.2 (pg 479)
  uint32_t comData; // 0x308 - Common Data Reg Dual Mode [RM] 16.7.3 (pg 482)
};
typedef volatile ADC_Regs_ ADC_Regs;
inline ADC_Regs *const ADC_BASE = reinterpret_cast<ADC_Regs *>(0X50040000);

// Timer Register
// NOTE: Offset values and applicable registers depend on timer used
struct TimerRegs_ {
  union {
    struct {
      uint32_t cen : 1;  // counter enable
      uint32_t udis : 1; // update disable
      uint32_t urs : 1;  // update request source
      uint32_t opm : 1;  // one-pulse mode
      uint32_t dir : 1;  // direction
      uint32_t cms : 2;  // center-aligned mode selection
      uint32_t arpe : 1; // auto-reload preload enable
      uint32_t ckd : 2;  // clock division
      uint32_t rsvd : 1;
      uint32_t uifremap : 1; // UIF status bit remapping
      uint32_t rsvd2 : 20;
    } s;
    uint32_t r;
  } ctrl1;
  uint32_t ctrl2;     // Control Register 2
  uint32_t slaveCtrl; // Slave Mode Control Register
  uint32_t intEna;    // DMA/Interrupt Enable Register
  uint32_t status;    // Status Register
  uint32_t event;     // Even Generation Register
  uint32_t ccMode[2]; // Capture/Compare Mode Register (1,2)
  uint32_t ccEnable;  // Capture/Compare Enable Register
  // The topmost bit of counter will contain contain UIFCOPY if UIFREMAP is
  // enabled, but *this register should not be decomposed into a bitfield
  // struct*.  The idea behind UIFREMAP is to read the counter plus the UIFCOPY
  // value atomically, in one go.
  uint32_t counter;    // Counter
  uint32_t prescale;   // Prescaler
  uint32_t reload;     // Auto-reload Register
  uint32_t repeat;     // Repetition Counter Register
  uint32_t compare[4]; // Capture/Compare Register (1-4)
  uint32_t deadTime;   // Break and Dead-time Register
  uint32_t dmaCtrl;    // DMA Control Register
  uint32_t dmaAddr;    // DMA Address for Full Transfer
  uint32_t opt1;       // Option Register 1
  uint32_t ccMode3;    // Capture/Compare Mode Register 3
  uint32_t compare5;   // Capture/Compare Register 5
  uint32_t compare6;   // Capture/Compare Register 6
  uint32_t opt2;       // Option Register 2
  uint32_t opt3;       // Option Register 3
};
typedef volatile TimerRegs_ TimerRegs;

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
struct FlashReg_ {

  // Access control register
  struct {
    uint32_t latency : 3; // Number of ait states
    uint32_t rsvd1 : 5;
    uint32_t prefecth_ena : 1; // Enable pre-fetch
    uint32_t icache_ena : 1;   // Instruction cache enable
    uint32_t dcache_ena : 1;   // Data cache enable
    uint32_t icache_rst : 1;   // Instruction cache reset
    uint32_t dcache_rst : 1;   // Data cache reset
    uint32_t run_pd : 1;       // Power-down during run or low power mode
    uint32_t sleep_pd : 1;     // Power-down during sleep mode
    uint32_t rsvd2 : 17;
  } access;

  uint32_t pdKey;  // 0x04 - Power-down Key Register (FLASH_PDKEYR)
  uint32_t key;    // 0x08 - Key Register (FLASH_KEYR)
  uint32_t optKey; // 0x0C - Option Key Register (FLASH_OPTKEYR)
  uint32_t status; // 0x10 - Status Register (FLASH_SR)

  // 0x14 - Control Register (FLASH_CR)
  struct {
    uint32_t program : 1;    // Set to program flash
    uint32_t page_erase : 1; // Pase erase
    uint32_t mass_erase : 1; // Mass erase
    uint32_t page : 8;       // Page number
    uint32_t rsvd1 : 5;
    uint32_t start : 1;     // Flash operation start
    uint32_t opt_start : 1; // Options modification start
    uint32_t fast_prog : 1; // Fast programming
    uint32_t rsvd2 : 5;
    uint32_t eop_ie : 1;     // End of operation interrupt enable
    uint32_t err_ie : 1;     // Error interrupt enable
    uint32_t rderr_ie : 1;   // read error interrupt enable
    uint32_t obl_launch : 1; // Force option byte reloading
    uint32_t rsvd3 : 2;
    uint32_t opt_lock : 1; // Lock the options area
    uint32_t lock : 1;     // Lock the flash
  } ctrl;

  uint32_t ecc; // 0x18 - EEC Register (FLASH_EECR)
  uint32_t rsvd1;
  uint32_t option;     // 0x20 - Option Register (FLASH_OPTR)
  uint32_t pcropStart; // 0x24 - PCROP Start Address Register (FLASH_PCROP1SR)
  uint32_t pcropEnd;   // 0x28 - PCROP End Address Register (FLASH_PCROP1ER)
  uint32_t wrpA;       // 0x2C - WRP Area A Address Register (FLASH_WRP1AR)
  uint32_t wrpB;       // 0x30 - WRP Area B Address Register (FLAS_WRP1BR)
};
typedef volatile FlashReg_ FlashReg;
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

struct DMA_Regs_ {
  union {
    struct {
      uint32_t gif1 : 1;  // global interrupt flag
      uint32_t tcif1 : 1; // transfer complete (TC) flag
      uint32_t htif1 : 1; // half transfer (HT) flag
      uint32_t teif1 : 1; // transfer error (TE) flag
      uint32_t gif2 : 1;  // global interrupt flag
      uint32_t tcif2 : 1; // transfer complete (TC) flag
      uint32_t htif2 : 1; // half transfer (HT) flag
      uint32_t teif2 : 1; // transfer error (TE) flag
      uint32_t gif3 : 1;  // global interrupt flag
      uint32_t tcif3 : 1; // transfer complete (TC) flag
      uint32_t htif3 : 1; // half transfer (HT) flag
      uint32_t teif3 : 1; // transfer error (TE) flag
      uint32_t gif4 : 1;  // global interrupt flag
      uint32_t tcif4 : 1; // transfer complete (TC) flag
      uint32_t htif4 : 1; // half transfer (HT) flag
      uint32_t teif4 : 1; // transfer error (TE) flag
      uint32_t gif5 : 1;  // global interrupt flag
      uint32_t tcif5 : 1; // transfer complete (TC) flag
      uint32_t htif5 : 1; // half transfer (HT) flag
      uint32_t teif5 : 1; // transfer error (TE) flag
      uint32_t gif6 : 1;  // global interrupt flag
      uint32_t tcif6 : 1; // transfer complete (TC) flag
      uint32_t htif6 : 1; // half transfer (HT) flag
      uint32_t teif6 : 1; // transfer error (TE) flag
      uint32_t gif7 : 1;  // global interrupt flag
      uint32_t tcif7 : 1; // transfer complete (TC) flag
      uint32_t htif7 : 1; // half transfer (HT) flag
      uint32_t teif7 : 1; // transfer error (TE) flag
      uint32_t rsvd : 4;
    };
    uint32_t r;
  } intStat; // Interrupt Status Register (DMA_ISR) [RM] 11.6.1 (pg 308)

  union {
    struct {
      uint32_t gif1 : 1;  // global interrupt flag
      uint32_t tcif1 : 1; // transfer complete (TC) flag
      uint32_t htif1 : 1; // half transfer (HT) flag
      uint32_t teif1 : 1; // transfer error (TE) flag
      uint32_t gif2 : 1;  // global interrupt flag
      uint32_t tcif2 : 1; // transfer complete (TC) flag
      uint32_t htif2 : 1; // half transfer (HT) flag
      uint32_t teif2 : 1; // transfer error (TE) flag
      uint32_t gif3 : 1;  // global interrupt flag
      uint32_t tcif3 : 1; // transfer complete (TC) flag
      uint32_t htif3 : 1; // half transfer (HT) flag
      uint32_t teif3 : 1; // transfer error (TE) flag
      uint32_t gif4 : 1;  // global interrupt flag
      uint32_t tcif4 : 1; // transfer complete (TC) flag
      uint32_t htif4 : 1; // half transfer (HT) flag
      uint32_t teif4 : 1; // transfer error (TE) flag
      uint32_t gif5 : 1;  // global interrupt flag
      uint32_t tcif5 : 1; // transfer complete (TC) flag
      uint32_t htif5 : 1; // half transfer (HT) flag
      uint32_t teif5 : 1; // transfer error (TE) flag
      uint32_t gif6 : 1;  // global interrupt flag
      uint32_t tcif6 : 1; // transfer complete (TC) flag
      uint32_t htif6 : 1; // half transfer (HT) flag
      uint32_t teif6 : 1; // transfer error (TE) flag
      uint32_t gif7 : 1;  // global interrupt flag
      uint32_t tcif7 : 1; // transfer complete (TC) flag
      uint32_t htif7 : 1; // half transfer (HT) flag
      uint32_t teif7 : 1; // transfer error (TE) flag
      uint32_t rsvd : 4;
    };
    uint32_t r;
  } intClr; // Interrupt Flag Clear Register [RM] 11.6.2 (pg 311)
  struct ChannelRegs {
    struct {
      uint32_t enable : 1;   // channel enable
      uint32_t tcie : 1;     // transfer complete interrupt enable
      uint32_t htie : 1;     // half transfer interrupt enable
      uint32_t teie : 1;     // transfer error interrupt enable
      uint32_t dir : 1;      // data xfer direction 0: per->mem, 1: mem->per
      uint32_t circular : 1; // circular mode
      uint32_t perInc : 1;   // peripheral increment mode
      uint32_t memInc : 1;   // memory increment mode
      uint32_t psize : 2;    // peripheral size 0b00 - 8bits, 0b10 - 32bits
      uint32_t msize : 2;    // memory size 0b00 - 8bits, 0b10 - 32bits
      uint32_t priority : 2; // priority level 0b00 - low, 0b11 - high
      uint32_t mem2mem : 1;  // memory-to-memory mode
      uint32_t rsvd : 17;
    } config;       // channel x configuration register [RM] 11.6.3 (pg 312)
    uint32_t count; // channel x number of data to transfer register
    volatile void *pAddr; // channel x peripheral address register
    volatile void *mAddr; // channel x memory address register
    uint32_t rsvd;
  } channel[7];
  uint32_t rsvd[5];
  union {
    struct {
      uint32_t c1s : 4;
      uint32_t c2s : 4;
      uint32_t c3s : 4;
      uint32_t c4s : 4;
      uint32_t c5s : 4;
      uint32_t c6s : 4;
      uint32_t c7s : 4;
      uint32_t rsvd : 4;
    };
    uint32_t r;
  } chanSel; // Channel Selection Register [RM] 11.6.7 (pg 317)
};
typedef volatile DMA_Regs_ DMA_Regs;
inline DMA_Regs *const DMA1_BASE = reinterpret_cast<DMA_Regs *>(0x40020000);
inline DMA_Regs *const DMA2_BASE = reinterpret_cast<DMA_Regs *>(0x40020400);

/* Select the source for a DMA channel:
  @param dma        Address of DMA registers
  @param chan       DMA channel to modify.  Channels are numbered from 0
  @param selection  Selects which peripheral request to map to the channel
*/
inline void DMA_SelectChannel(DMA_Regs *dma, DMA_Chan chan, int selection) {
  selection &= 0x0F;

  int x = 4 * static_cast<int>(chan); // 4 bits per channel ([RM] 11.3.2)

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

// Detect a DMA Interrupt
inline bool DMA_IntStatus(DMA_Regs *dma, DMA_Chan chan,
                          DmaInterrupt interrupt) {

  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(chan); // 4 events per channel ([RM] 11.6.2)
  return (x & dma->intStat.r) > 0;
}

// Clear a DMA Interrupt
inline void DMA_ClearInt(DMA_Regs *dma, DMA_Chan chan, DmaInterrupt interrupt) {

  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(chan); // 4 events per channel ([RM] 11.6.2)
  dma->intClr.r = x;
}

// [RM] 40.6 SPI Registers (pg 1330)
struct SPI_Regs_ {
  struct {
    uint32_t cpha : 1;       // Clock phase
    uint32_t cpol : 1;       // Clock polarity
    uint32_t mstr : 1;       // Master mode
    uint32_t br : 3;         // Bit rate
    uint32_t spe : 1;        // SPI enable
    uint32_t lsb_first : 1;  // Send LSB first
    uint32_t ssi : 1;        // Internal slave select
    uint32_t ssm : 1;        // Software slave management
    uint32_t rx_only : 1;    // Receive mode only
    uint32_t crcl : 1;       // CRC length
    uint32_t crc_next : 1;   // CRC transmit next
    uint32_t crc_ena : 1;    // CRC enable
    uint32_t bidir_oe : 1;   // Bidirectional output enable
    uint32_t bidir_mode : 1; // Bidirectional data mode enable
    uint32_t rsvd : 16;
  } ctrl1; // Control register 1 (SPIx_CR1) [RM] 40.6.1 (pg 1330)
  struct {
    uint32_t rx_dma_en : 1; // Receive buffer DMA enable
    uint32_t tx_dma_en : 1; // Transmit buffer DMA enable
    uint32_t ssoe : 1;      // SS output enable
    uint32_t nssp : 1;      // NSS pulse management
    uint32_t frf : 1;       // Frame format
    uint32_t err_ie : 1;    // Error interrupt enable
    uint32_t rxne_ie : 1;   // Receive buffer not empty int enable
    uint32_t txe_ie : 1;    // Transmit buffer empty int enable
    uint32_t ds : 4;        // Data size
    uint32_t frxth : 1;     // FIFO reception threshold
    uint32_t ldma_rx : 1;   // Last DMA xfer for receive
    uint32_t ldma_tx : 1;   // Last DMA xfer for transmit
    uint32_t rsvd : 17;
  } ctrl2;          // Control register 2 (SPIx_CR2) [RM] 40.6.2
  uint32_t status;  // Status register (SPIx_SR) [RM] 40.6.3
  uint32_t data;    // Data register (SPIx_Dr) [RM] 40.6.4
  uint32_t crcPoly; // CRC polynomial register (SPIx_CRCPR) [RM] 40.6.5
  uint32_t rxCRC;   // Rx CRC register (SPIx_RXCRCR) [RM] 40.6.6
  uint32_t txCRC;   // Tx CRC register (SPIx_TXCRCR) [RM] 40.6.7
};
typedef volatile SPI_Regs_ SPI_Regs;
inline SPI_Regs *const SPI1_BASE = reinterpret_cast<SPI_Regs *>(0x40013000);
inline SPI_Regs *const SPI2_BASE = reinterpret_cast<SPI_Regs *>(0x40003800);
inline SPI_Regs *const SPI3_BASE = reinterpret_cast<SPI_Regs *>(0x40003C00);

// [RM] 37.7 I2C Registers
struct I2C_Regs_ {
  union {
    struct {
      uint32_t peripheral_en : 1;   // Set to enable the I2C bus
      uint32_t tx_interrupts : 1;   // Interrupt when TX empty while channel
                                    // active
      uint32_t rx_interrupts : 1;   // Interrupt when RX not empty
      uint32_t addr_interrupts : 1; // Generate interrupts on address
                                    // match (slave)
      uint32_t nack_interrupts : 1; // Generate interrupts on NACK
      uint32_t stop_interrupts : 1; // Generate interrupts on stop
                                    // detection (slave)
      uint32_t tx_complete_interrupts : 1; // Interrupt when transfer
                                           // complete
      uint32_t error_interrupts : 1;       // Interrupt on error
      uint32_t dnf_cntrl : 4;              // Configure digital noise filtering
      uint32_t anf_off : 1;                // Disable analog noise filtering
      uint32_t reserved1 : 1;
      uint32_t dma_tx : 1;        // Enable DMA for TX
      uint32_t dma_rx : 1;        // Enable DMA for RX
      uint32_t slave_byte_en : 1; // Enable slave byte control (slave)
      uint32_t no_stretch : 1;    // Disable Clock stretching (slave)
      uint32_t wup_en : 1;        // Wake up from Stop Mode
      uint32_t gc_en : 1;         // General call (slave)
      uint32_t smd_h_en : 1;      // Use SMBus default host address
      uint32_t smb_d_en : 1;      // Use SMBus default device address
      uint32_t alert_en : 1;      // SMBus Alert enable
      uint32_t pec_en : 1;        // Use SMBus Packet error checking
      uint32_t reserved2 : 8;
    };
    uint32_t r;
  } ctrl1; // Control register 1 [RM] 37.7.1
  union {
    struct {
      uint32_t slave_addr_lsb : 1; // lsb of 10 bits slave address
                                   // (master)
      uint32_t slave_addr_7b : 7;  // middle 7b bits of slave address
                                   // (master)
      uint32_t slave_addr_msb : 2; // msb of 10 bits slave address
                                   // (master)
      uint32_t transfer_dir : 1;   // 0 = write, 1 = read (master)
      uint32_t address_10b : 1;    // Set to enable 10 bits address header
                                   // (master)
      uint32_t read_10b_head : 1;  // Clear to send complete read sequence
                                   // (master)
      uint32_t start : 1;          // Set to generate START condition
      uint32_t stop : 1;           // Set to generate STOP after byte transfer
                                   // (master)
      uint32_t nack : 1;           // Generate NACK after byte reception (slave)
      uint32_t n_bytes : 8;        // Set to the number of bytes to send
      uint32_t reload : 1;         // Set to allow several consecutive transfers
      uint32_t autoend : 1;        // Set to automatically send stop condition
      uint32_t pecbyte : 1;        // Set to send SMBus packet error checking
                                   // byte
      uint32_t reserved : 5;
    };
    uint32_t r;
  } ctrl2;          // Control register 2 [RM] 37.7.2
  uint32_t addr[2]; // Own address (1-2) register [RM] 37.7.{3,4} (slave)
  union {
    struct {
      uint32_t scl_low : 8;   // Duration of SCL Low state (cycles)
      uint32_t scl_high : 8;  // Duration of SCL High state (cycles)
      uint32_t sda_hold : 4;  // Delay between SCL falling edge and SDA
                              // edge (cycles)
      uint32_t scl_delay : 4; // Delay between SDA edge and SCL rising
                              // edge (cycles)
      uint32_t reserved : 4;
      uint32_t prescaler : 4; // Prescaler
    };
    uint32_t r;
  } timing;         // Timing register [RM] 37.7.5
  uint32_t timeout; // Timout register [RM] 37.7.6
  union {
    struct {
      uint32_t tx_empty : 1;
      uint32_t tx_interrupt : 1;
      uint32_t rx_not_empty : 1;
      uint32_t address_match : 1;
      uint32_t nack : 1;
      uint32_t stop : 1;
      uint32_t transfer_complete : 1;
      uint32_t transfer_reload : 1;
      uint32_t bus_error : 1;
      uint32_t arbitration_loss : 1;
      uint32_t overrun : 1;
      uint32_t packet_check_error : 1;
      uint32_t timeout : 1;
      uint32_t alert : 1;
      uint32_t reserved1 : 1;
      uint32_t busy : 1;
      uint32_t transfer_dir : 1;
      uint32_t address_code : 7;
      uint32_t reserved : 8;
    };
    uint32_t r;
  } status;        // Interrupt & status register [RM] 37.7.7
  uint32_t intClr; // Interrupt clear register [RM] 37.7.8
  uint32_t pec;    // PEC register [RM] 37.7.9
  uint32_t rxData; // Receive data register [RM] 37.7.10
  uint32_t txData; // Transmit data register [RM] 37.7.11
};
typedef volatile I2C_Regs_ I2C_Regs;
inline I2C_Regs *const I2C1_BASE = reinterpret_cast<I2C_Regs *>(0x40005400);
inline I2C_Regs *const I2C2_BASE = reinterpret_cast<I2C_Regs *>(0x40005800);
inline I2C_Regs *const I2C3_BASE = reinterpret_cast<I2C_Regs *>(0x40005c00);
inline I2C_Regs *const I2C4_BASE = reinterpret_cast<I2C_Regs *>(0x40008400);

// Watchdog timer
// [RM] 32.4 Watchdog Registers (pg 1016)
struct Watchdog_Regs_ {
  uint32_t key;      // Key register [RM] 32.4.1
  uint32_t prescale; // Prescale register [RM] 32.4.2
  uint32_t reload;   // Reload register [RM] 32.4.3
  uint32_t status;   // Status register [RM] 32.4.4
  uint32_t window;   // Window register [RM] 32.4.5
};
typedef volatile Watchdog_Regs_ Watchdog_Regs;
inline Watchdog_Regs *const WATCHDOG_BASE =
    reinterpret_cast<Watchdog_Regs *>(0x40003000);

// CRC calculation unit
// [RM] 14.4 CRC Registers (pg 341)
struct CRC_Regs_ {
  uint32_t data;    // Data register [RM] 14.4.1
  uint32_t scratch; // Independent data register [RM] 14.4.2
  uint32_t ctrl;    // Control register [RM] 14.4.3
  uint32_t rsvd;
  uint32_t init; // Initial CRC value [RM] 14.4.4
  uint32_t poly; // CRC polynomial [RM] 14.4.5
};
typedef volatile CRC_Regs_ CRC_Regs;
inline CRC_Regs *const CRC_BASE = reinterpret_cast<CRC_Regs *>(0x40023000);

// General Purpose I/O
// [RM] 8.4 GPIO Registers (pg 267)
struct GPIO_Regs_ {
  uint32_t mode;     // Mode register [RM] 8.4.1
  uint32_t outType;  // Output type register [RM] 8.4.2
  uint32_t outSpeed; // Output speed register [RM] 8.4.3
  uint32_t pullUpDn; // Pull-up/pull-down register [RM] 8.4.4
  uint32_t inDat;    // Input data register [RM] 8.4.5
  uint32_t outDat;   // Output data register [RM] 8.4.6
  uint16_t set;      // Bit set register [RM] 8.4.7
  uint16_t clr;      // Bit reset register [RM] 8.4.7
  uint32_t lock;     // Configuration lock register [RM] 8.4.8
  uint32_t alt[2];   // Alternate function low/high register [RM] 8.4.{9,10}
  uint32_t reset;    // Reset register [RM] 8.4.11
};
typedef volatile GPIO_Regs_ GPIO_Regs;
inline GPIO_Regs *const GPIO_A_BASE = reinterpret_cast<GPIO_Regs *>(0x48000000);
inline GPIO_Regs *const GPIO_B_BASE = reinterpret_cast<GPIO_Regs *>(0x48000400);
inline GPIO_Regs *const GPIO_C_BASE = reinterpret_cast<GPIO_Regs *>(0x48000800);
inline GPIO_Regs *const GPIO_D_BASE = reinterpret_cast<GPIO_Regs *>(0x48000C00);
inline GPIO_Regs *const GPIO_E_BASE = reinterpret_cast<GPIO_Regs *>(0x48001000);
inline GPIO_Regs *const GPIO_H_BASE = reinterpret_cast<GPIO_Regs *>(0x48001C00);

#endif
