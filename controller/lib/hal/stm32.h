/* stm32.h */

#ifndef _DEF_INC_STM32
#define _DEF_INC_STM32

#include <stdint.h>

// Some useful inline functions to enable/disable interrupts
static inline void IntDisable( void ){
   asm volatile( "cpsid i" );
}

static inline void IntEnable( void ){
   asm volatile( "cpsie i" );
}

static inline void IntRestore( int p ){
   if( p ) IntEnable();
}

static inline int IntSuspend( void ){
   int ret;
   asm volatile( "mrs   %[output], primask\n\t" "cpsid i": [output] "=r" (ret) );
   return (ret==0);
}

// Represents a 32-bit register
typedef volatile uint32_t REG;

// 16-bit short register
typedef volatile uint16_t SREG;

///////////////////////////////////////////////////////////////
// The structures below represent the STM32 registers used
// to configure various modules (like timers, serial ports, etc).
// Detailed information on these modules and the registers
// used to configure them can be found in the reference
// manual for this chip.
///////////////////////////////////////////////////////////////

// Reset & clock controller
#define RCC_BASE                    0x40021000
struct RCC_Regs
{
   REG clkCtrl;                  // 0x00 clock control register (RCC_CR)
   REG clkCal;                   // 0x04 Internal clock sources calibration register (RCC_ICSCR)
   REG clkCfg;                   // 0x08 clock configuration register (RCC_CFGR)
   REG pllCfg;                   // 0x0C PLL configuration register (RCC_PLLCFGR)
   REG pllSaiCfg;                // 0x10 PLLSAI1 configuration register (RCC_PLLSAI1CFGR)
   REG rsvd1;
   REG clkIntEna;                // 0x18 Clock interrupt enable register ( RCC_CIER) 
   REG clkIntFlg;                // 0x1C Clock interrupt flag register ( RCC_CIFR)
   REG clkIntClr;                // 0x20 Clock interrupt clear register ( RCC_CICR)
   REG rsvd2;
   REG periphReset[8];           // 0x28 peripheral reset registers
   REG periphClkEna[8];          // 0x48 peripheral clock registers
   REG sleepClkEna[8];           // 0x68 Clock enable in sleep
   REG indClkCfg;                // 0x88 Peripherals independent clock configuration register (RCC_CCIPR)
   REG rsvd3;
   REG backup;                   // 0x90 Backup domain control register (RCC_BDCR)
   REG status;                   // 0x94 control & status register (RCC_CSR)
   REG recovery;                 // 0x98 Clock recovery RC register (RCC_CRRCR)
   REG indClkCfg2;               // 0x9C Peripherals independent clock configuration register (RCC_CCIPR2)
};

// System control registers
#define SYSCTL_BASE                 0xE000E000
struct SysCtrl_Reg
{
   REG rsvd0;                    // 0xE000E000
   REG intType;                  // 0xE000E004
   REG auxCtrl;                  // 0xE000E008
   REG rsvd1;                    // 0xE000E00C
   REG systick[3];               // 0xE000E010
   REG rsvd2[57];
   REG nvic[768];                // 0xE000E100
   REG cpuid;                    // 0xE000ED00
   REG intCtrl;                  // 0xE000ED04
   REG vtable;                   // 0xE000ED08
   REG apInt;                    // 0xE000ED0C
   REG sysCtrl;                  // 0xE000ED10
   REG cfgCtrl;                  // 0xE000ED14
   REG sysPri[3];                // 0xE000ED18
   REG sysHdnCtrl;               // 0xE000ED24
   REG faultStat;                // 0xE000ED28
   REG hardFaultStat;            // 0xE000ED2C
   REG rsvd3;
   REG mmFaultAddr;              // 0xE000ED34
   REG faultAddr;                // 0xE000ED38
   REG rsvd4[19];
   REG cpac;                     // 0xE000ED88
};

// Digital I/O
#define DIGIO_A_BASE                0x48000000
#define DIGIO_B_BASE                0x48000400
#define DIGIO_C_BASE                0x48000800
struct GPIO_Regs
{
   REG  mode;
   REG  outType;
   REG  outSpeed;
   REG  pullUpDn;
   REG  inDat;
   REG  outDat;
   SREG set;
   SREG clr;
   REG  lock;
   REG  alt[2];
   REG  reset;
};

#define UART1_BASE                  0x40013800
#define UART2_BASE                  0x40004400
#define UART3_BASE                  0x40004800
#define UART4_BASE                  0x40004C00
struct UART_Regs
{
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

#define ADC_BASE                   0X50040000
struct ADC_Regs
{
   // A/D specific registers (0x100 total length)
   struct
   {
      REG stat;                  // 0x00 - interrupt and status register (ADC_ISR)
      REG intEna;                // 0x04 - interrupt enable register (ADC_IER)
      REG ctrl;                  // 0x08 - control register (ADC_CR)
      REG cfg[2];                // 0x0C - configuration registers
      REG samp[2];               // 0x14 - sampling time registers
      REG rsvd1;
      REG wdog[3];               // 0x20 - watchdog threshold registers
      REG rsvd2;
      REG seq[4];                // 0x30 - Regular sequence registers
      REG data;                  // 0x40 - Regular data register
      REG rsvd3[2];
      REG iSeq;                  // 0x4C - Injected sequence regiseter
      REG rsvd4[4];
      REG offset[4];             // 0x60 - Offset registers
      REG rsvd5[4];
      REG iData[4];              // 0x80 - Injected channel data
      REG rsvd6[4];
      REG wdCfg[2];              // 0xA0 - Watchdog config
      REG rsvd7[2];
      REG diffSel;               // 0xB0 - Differential mode selection
      REG cal;                   // 0xB4 - Calibration factors
      REG rsvd8[18];
   } adc[2];

   // A/D common registers
   REG comStat;                  // 0x300 - Common status
   REG rsvd9;
   REG comCtrl;                  // 0x304 - Common control
   REG comData;                  // 0x308 - Common data
};

#define TIMER1_BASE                 0x40012C00
#define TIMER2_BASE                 0x40000000
#define TIMER3_BASE                 0x40000400
#define TIMER6_BASE                 0x40001000
#define TIMER7_BASE                 0x40001400
#define TIMER15_BASE                0x40014000
#define TIMER16_BASE                0x40014400
struct TimerRegs
{
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

#define FLASH_BASE                 0x40022000
struct FlashReg
{
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

#define DMA1_BASE                  0x40020000
#define DMA2_BASE                  0x40020400
struct DMA_Reg
{
   REG intStat;
   REG intClr;
   struct
   {
      REG config;
      REG count;
      REG pAddr;
      REG mAddr;
      REG rsvd;
   } channel[7];
   REG rsvd[5];
   REG chanSel;
};

#define SPI1_BASE                  0x40013000
#define SPI2_BASE                  0x40003800
#define SPI3_BASE                  0x40003C00
struct SPI_Regs
{
   REG ctrl[2];
   REG status;
   REG data;
   REG crcPoly;
   REG rxCRC;
   REG txCRC;
};

#define I2C1_BASE                  0x40005400
#define I2C2_BASE                  0x40005800
#define I2C3_BASE                  0x40005c00
#define I2C4_BASE                  0x40008400
struct I2C_Regs
{
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

#endif
