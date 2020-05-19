#if defined(BARE_STM32) && defined(UART_VIA_DMA)
#include "uart_dma.h"
#include "hal_stm32.h"
#include "hal_stm32_regs.h"

#include "debug.h"
// STM32 UART3 driver based on DMA transfers.

// Direct Memory Access mode in MCU allows to set up a memory buffer
// transfer by means of hardware with no CPU intervention.
// CPU responsibility is to set up a DMA channel configuring it's
// endpoints as peripheral and memory. Upon transfer completion
// CPU is notified via interrupt.

// This driver also provides Character Match interrupt on reception.
// UART will issue an interrupt upot the reception of the specified
// character.

extern UART_DMA dmaUART;

// Performs UART3 initialization
void UART_DMA::init(int baud) {
  // Set baud rate register
  uart->baud = CPU_FREQ / baud;

  uart->ctrl3.s.dmar = 1;         // set DMAR bit to enable DMA for receiver
  uart->ctrl3.s.dmat = 1;         // set DMAT bit to enable DMA for transmitter
  uart->ctrl3.s.ddre = 1;         // DMA is disabled following a reception error
  uart->ctrl2.s.rtoen = 1;        // Enable receive timeout feature
  uart->ctrl2.s.addr = matchChar; // set match char

  uart->ctrl3.s.eie = 1; // enable interrupt on error

  uart->request = (1 << 3); // Clear RXNE flag before clearing other flags
  uart->intClear = (1 << 11) | (1 << 3) | (1 << 1); // Clear error flags

  uart->ctrl1.s.te = 1; // enable transmitter
  uart->ctrl1.s.re = 1; // Enable receiver
  uart->ctrl1.s.ue = 1; // enable uart

  // TODO enable parity checking?

  dma->channel[rxCh].config.priority = 0b11; // high priority
  dma->channel[rxCh].config.teie = 1;        // interrupt on error
  dma->channel[rxCh].config.htie = 0;        // no half-transfer interrupt
  dma->channel[rxCh].config.tcie = 1;        // interrupt on DMA complete

  dma->channel[rxCh].config.mem2mem = 0; // memory-to-memory mode disabled
  dma->channel[rxCh].config.msize = DmaTransferSize::BITS8;
  dma->channel[rxCh].config.psize = DmaTransferSize::BITS8;
  dma->channel[rxCh].config.memInc = 1;   // increment destination (memory)
  dma->channel[rxCh].config.perInc = 0;   // don't increment source
                                          // (peripheral) address
  dma->channel[rxCh].config.circular = 0; // not circular
  dma->channel[rxCh].config.dir = DmaChannelDir::PERIPHERAL_TO_MEM;

  dma->channel[txCh].config.priority = 0b11; // high priority
  dma->channel[txCh].config.teie = 1;        // interrupt on error
  dma->channel[txCh].config.htie = 0;        // no half-transfer interrupt
  dma->channel[txCh].config.tcie = 1;        // DMA complete interrupt enabled

  dma->channel[txCh].config.mem2mem = 0; // memory-to-memory mode disabled
  dma->channel[txCh].config.msize = DmaTransferSize::BITS8;
  dma->channel[txCh].config.psize = DmaTransferSize::BITS8;
  dma->channel[txCh].config.memInc = 1;   // increment source (memory) address
  dma->channel[txCh].config.perInc = 0;   // don't increment dest (peripheral)
                                          // address
  dma->channel[txCh].config.circular = 0; // not circular
  dma->channel[txCh].config.dir = DmaChannelDir::MEM_TO_PERIPHERAL;
}

// Sets up an interrupt on matching char incomming form UART3
void UART_DMA::charMatchEnable() {
  uart->intClear = (1 << 17); // Clear char match flag
  uart->ctrl1.s.cmie = 1;     // Enable character match interrupt
}

// Returns true if DMA TX is in progress
bool UART_DMA::isTxInProgress() {
  // TODO thread safety
  return tx_in_progress;
}

// Returns true if DMA RX is in progress
bool UART_DMA::isRxInProgress() {
  // TODO thread safety
  return rx_in_progress;
}

// Sets up UART to transfer [length] characters from [buf]
// Returns false if DMA transmission is in progress, does not
// interrupt previous transmission.
// Returns true if no transmission is in progress
bool UART_DMA::startTX(const char *buf, uint32_t length) {
  if (isTxInProgress()) {
    return false;
  }

  dma->channel[txCh].config.enable = 0; // Disable channel before config
  // data sink
  dma->channel[txCh].pAddr = reinterpret_cast<REG>(&(uart->txDat));
  // data source
  dma->channel[txCh].mAddr = reinterpret_cast<REG>(buf);
  // data length
  dma->channel[txCh].count = length & 0x0000FFFF;

  dma->channel[txCh].config.enable = 1; // go!

  tx_in_progress = true;

  return true;
}

void UART_DMA::stopTX() {
  if (isTxInProgress()) {
    // Disable DMA channel
    dma->channel[1].config.enable = 0;
    // TODO thread safety
    tx_in_progress = 0;
  }
}

// Sets up reception of at least [length] chars from UART3 into [buf]
// [timeout] is the number of baudrate bits for which RX line is
// allowed to be idle before asserting timeout error.
// Returns false if reception is in progress, new reception is not
// setup. Returns true if no reception is in progress and new reception
// was setup.

bool UART_DMA::startRX(const char *buf, const uint32_t length,
                       const uint32_t timeout) {
  // UART3 reception happens on DMA1 channel 3
  if (isRxInProgress()) {
    return false;
  }

  dma->channel[rxCh].config.enable = 0; // don't enable yet

  // data source
  dma->channel[rxCh].pAddr = reinterpret_cast<REG>(&(uart->rxDat));
  // data sink
  dma->channel[rxCh].mAddr = reinterpret_cast<REG>(buf);
  // data length
  dma->channel[rxCh].count = length;

  // setup rx timeout
  // max timeout is 24 bit
  uart->timeout.s.rto = timeout & 0x00FFFFFF;
  uart->intClear = (1 << 11); // Clear rx timeout flag
  uart->request = (1 << 3);   // Clear RXNE flag
  uart->ctrl1.s.rtoie = 1;    // Enable receive timeout interrupt

  dma->channel[rxCh].config.enable = 1; // go!

  rx_in_progress = true;

  return true;
}

uint32_t UART_DMA::getRxBytesLeft() { return dma->channel[2].count; }

void UART_DMA::stopRX() {
  if (isRxInProgress()) {
    uart->ctrl1.s.rtoie = 0;           // Disable receive timeout interrupt
    dma->channel[2].config.enable = 0; // Disable DMA channel
    // TODO thread safety
    rx_in_progress = 0;
  }
}

inline bool isCharacterMatchInterrupt() {
  UART_Regs *const uart = UART3_BASE;
  return 0 != (uart->status & (1 << 17));
}

inline bool isRxTimeout() {
  UART_Regs *const uart = UART3_BASE;
  // Timeout interrupt enable and RTOF - Receiver timeout
  return uart->ctrl1.s.rtoie && (uart->status & (1 << 11));
}

inline bool isRxError() {
  UART_Regs *const uart = UART3_BASE;

  return isRxTimeout() ||
         0 != (uart->status & (1 << 3)) || // ORE - Overrun error
         0 != (uart->status & (1 << 1));   // FE - frame error

  // 0 != uart->status & (1 << 0)  || // PE - parity error
  // 0 != uart->status & (1 << 2)  || // START bit Noise detection flag
}

inline void UART_DMA::UART_ISR() {
  if (isRxError()) {
    RxError_t e = RxError_t::RX_ERROR_UNKNOWN;
    if (isRxTimeout()) {
      e = RxError_t::RX_ERROR_TIMEOUT;
    }
    if (uart->status & (1 << 3)) {
      e = RxError_t::RX_ERROR_OVR;
    }
    if (uart->status & (1 << 1)) {
      e = RxError_t::RX_ERROR_FRAMING;
    }

    uart->request = (1 << 3); // Clear RXNE flag before clearing other flags
    uart->intClear = (1 << 11) | (1 << 3) | (1 << 1); // Clear error flags

    // TODO define logic if stopRX() has to be here
    rxListener.onRxError(e);
  }

  if (isCharacterMatchInterrupt()) {
    uart->request = (1 << 3);   // Clear RXNE flag before clearing other flags
    uart->intClear = (1 << 17); // Clear char match flag
    // TODO define logic if stopRX() has to be here
    rxListener.onCharacterMatch();
  }
}

inline void UART_DMA::DMA_TX_ISR() {
  if (dma->intStat.teif2) {
    stopTX();
    txListener.onTxError();
  } else {
    stopTX();
    txListener.onTxComplete();
  }
}

inline void UART_DMA::DMA_RX_ISR() {
  if (dma->intStat.teif3) {
    stopRX();
    rxListener.onRxError(RxError_t::RX_ERROR_DMA);
  } else {
    stopRX();
    rxListener.onRxComplete();
  }
}

void DMA1_CH2_ISR() {
  DMA_Regs *dma = DMA1_BASE;
  dmaUART.DMA_TX_ISR();
  dma->intClr.gif2 = 1; // clear all channel 3 flags
}

void DMA1_CH3_ISR() {
  DMA_Regs *dma = DMA1_BASE;
  dmaUART.DMA_RX_ISR();
  dma->intClr.gif3 = 1; // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void UART3_ISR() { dmaUART.UART_ISR(); }

#endif
