#ifndef __HAL_STM32_REGS
#define __HAL_STM32_REGS

#include <stdint.h>

// Represents a 32-bit register
typedef volatile uint32_t REG;

// 16-bit short register
typedef volatile uint16_t SREG;

// 8-bit byte sized register
typedef volatile uint8_t BREG;

///////////////////////////////////////////////////////////////
// The structures below represent the STM32 registers used
// to configure various modules (like timers, serial ports, etc).
// Detailed information on these modules and the registers
// used to configure them can be found in the reference
// manual for this chip:
// https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
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
  } ctrl1;
  union {
    struct {
      REG rsvd2 : 4;
      REG addm7 : 1; // 7-bit Address Detection/4-bit Address Detection
      REG lbdl : 1;  // LIN break detection length
      REG lbdie : 1; // LIN break detection interrupt enable
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
  } ctrl2;
  union {
    struct {
      REG eie : 1;    // Error interrupt enable
      REG iren : 1;   // IrDA mode enable
      REG irlp : 1;   // IrDA low-power
      REG hdsel : 1;  // Half-duplex selection
      REG nack : 1;   // Smartcard NACK enable
      REG scen : 1;   // Smartcard mode enable
      REG dmar : 1;   // DMA enable receiver
      REG dmat : 1;   // DMA enable transmitter
      REG rtse : 1;   // RTS enable
      REG ctse : 1;   // CTS enable
      REG ctsie : 1;  // CTS interrupt enable
      REG onebit : 1; // One sample bit method enable
      REG ovrdis : 1; // Overrun Disable
      REG ddre : 1;   // DMA Disable on Reception Error
      REG dem : 1;    // Driver enable mode
      REG dep : 1;    // Driver enable polarity selection
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
  } ctrl3;
  REG baud;
  REG guard;
  union {
    struct {
      REG rto : 24; // receiver timeout
      REG blen : 8; // block length
    } s;
    REG r;
  } timeout;
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
  } request;
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
  } status;
  union {
    struct {
      REG pecf : 1;   // parity error clear flag
      REG fecf : 1;   // framing error clear flag
      REG ncf : 1;    // noise detected clear flag
      REG orecf : 1;  // overrun error clear flag
      REG idlecf : 1; // idle line detected clear flag
      REG rsvd : 1;
      REG tccf : 1;    // transmission complete clear flag
      REG tcbgtcf : 1; // transmission completed before guard time clear fl.
      REG lbdcf : 1;   // LIN break detection clear flag
      REG ctscf : 1;   // CTS clear flag
      REG rsvd2 : 1;
      REG rtocf : 1; // receiver timeout clear flag
      REG eobcf : 1; // end of block clear flag
      REG rsvd3 : 4;
      REG cmcf : 1; // character match clear flag
      REG rsvd4 : 2;
      REG wucf : 1; // wakeup from stop mode clear flag
      REG rsvd5 : 11;
    } s;
    REG r;
  } intClear;
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
    REG stat;   // 0x00 - interrupt and status register (ADC_ISR)
    REG intEna; // 0x04 - interrupt enable register (ADC_IER)
    REG ctrl;   // 0x08 - control register (ADC_CR)
    REG cfgr;   // 0x0C - configuration register (ADC_CFGR)
    // 0x10 - Begin ADC_CFGR2
    REG rovse : 1;           // bit 0    - regular oversampling enable
    REG jovse : 1;           // bit 1    - injected oversampling enable
    REG ovsr : 3;            // bits 2-4 - oversampling ratio
    REG ovss : 4;            // bits 5-8 - oversampling shift (max 0b1000)
    REG trovs : 1;           // bit 9    - triggered regular oversampling
    REG rovsm : 1;           // bit 10   - regular oversampling mode
    REG cfgr2_reserved : 21; // Reserved bits in ADC_CFGR2.
    // End ADC_CFGR2
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
inline TimerRegs *const TIMER7_BASE = reinterpret_cast<TimerRegs *>(0x40001400);
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

typedef enum { PERIPHERAL_TO_MEM = 0, MEM_TO_PERIPHERAL = 1 } DmaChannelDir;
typedef enum { BITS8 = 0, BITS16 = 1, BITS32 = 2 } DmaTransferSize;

struct DMA_Regs {
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
  } intStat; // interrupt status register

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
  } intClr; // interrupt flag clear register
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
    } config;  // channel x configuration register
    REG count; // channel x number of data to transfer register
    REG pAddr; // channel x peripheral address register
    REG mAddr; // channel x memory address register
    REG rsvd;  // reserved
  } channel[7];
  REG rsvd[5]; // reserved
  struct {
    REG c1s : 4;
    REG c2s : 4;
    REG c3s : 4;
    REG c4s : 4;
    REG c5s : 4;
    REG c6s : 4;
    REG c7s : 4;
    REG rsvd : 4;
  } chanSel; // channel selection register
};
inline DMA_Regs *const DMA1_BASE = reinterpret_cast<DMA_Regs *>(0x40020000);
inline DMA_Regs *const DMA2_BASE = reinterpret_cast<DMA_Regs *>(0x40020400);

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

#endif
