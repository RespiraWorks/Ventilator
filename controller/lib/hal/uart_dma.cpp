#if defined(BARE_STM32)

#include "uart_dma.h"
#include "hal_stm32.h"
#include "hal_stm32_regs.h"
#include "units.h"

// STM32 UART3 driver based on DMA transfers.

// Direct Memory Access mode in MCU allows to set up a memory buffer
// transfer by means of hardware with no CPU intervention.
// CPU responsibility is to set up a DMA channel configuring it's
// endpoints as peripheral and memory. Upon transfer completion
// CPU is notified via interrupt.

// This driver also provides Character Match interrupt on reception.
// UART will issue an interrupt upon receipt of the specified
// character.

extern UART_DMA uart_dma;

// Performs UART3 initialization
void UART_DMA::init(uint32_t baud) {
  baud_ = baud;
  // Set baud rate register
  uart->baud = CPU_FREQ / baud;

  uart->ctrl3.s.dmar = 1;         // set DMAR bit to enable DMA for receiver
  uart->ctrl3.s.dmat = 1;         // set DMAT bit to enable DMA for transmitter
  uart->ctrl3.s.ddre = 1;         // DMA is disabled following a reception error
  uart->ctrl2.s.rtoen = 1;        // Enable receive timeout feature
  uart->ctrl2.s.addr = matchChar; // set match char

  uart->ctrl3.s.eie = 1; // enable interrupt on error

  uart->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags

  // Clear error flags.
  uart->intClear.s.fecf = 1;
  uart->intClear.s.orecf = 1;
  uart->intClear.s.rtocf = 1;

  uart->ctrl1.s.te = 1; // enable transmitter
  uart->ctrl1.s.re = 1; // Enable receiver
  uart->ctrl1.s.ue = 1; // enable uart

  // TODO enable parity checking?

  dma->channel[rxCh].config.priority = 0b11; // high priority
  dma->channel[rxCh].config.teie = 1;        // interrupt on error
  dma->channel[rxCh].config.htie = 0;        // no half-transfer interrupt
  dma->channel[rxCh].config.tcie = 1;        // interrupt on DMA complete

  dma->channel[rxCh].config.mem2mem = 0; // memory-to-memory mode disabled
  dma->channel[rxCh].config.msize = static_cast<REG>(DmaTransferSize::BITS8);
  dma->channel[rxCh].config.psize = static_cast<REG>(DmaTransferSize::BITS8);
  dma->channel[rxCh].config.memInc = 1;   // increment destination (memory)
  dma->channel[rxCh].config.perInc = 0;   // don't increment source
                                          // (peripheral) address
  dma->channel[rxCh].config.circular = 0; // not circular
  dma->channel[rxCh].config.dir =
      static_cast<REG>(DmaChannelDir::PERIPHERAL_TO_MEM);

  dma->channel[txCh].config.priority = 0b11; // high priority
  dma->channel[txCh].config.teie = 1;        // interrupt on error
  dma->channel[txCh].config.htie = 0;        // no half-transfer interrupt
  dma->channel[txCh].config.tcie = 1;        // DMA complete interrupt enabled

  dma->channel[txCh].config.mem2mem = 0; // memory-to-memory mode disabled
  dma->channel[txCh].config.msize = static_cast<REG>(DmaTransferSize::BITS8);
  dma->channel[txCh].config.psize = static_cast<REG>(DmaTransferSize::BITS8);
  dma->channel[txCh].config.memInc = 1;   // increment source (memory) address
  dma->channel[txCh].config.perInc = 0;   // don't increment dest (peripheral)
                                          // address
  dma->channel[txCh].config.circular = 0; // not circular
  dma->channel[txCh].config.dir =
      static_cast<REG>(DmaChannelDir::MEM_TO_PERIPHERAL);
}

// Sets up an interrupt on matching char incomming form UART3
void UART_DMA::charMatchEnable() {
  uart->intClear.s.cmcf = 1; // Clear char match flag
  uart->ctrl1.s.cmie = 1;    // Enable character match interrupt
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
bool UART_DMA::startTX(const uint8_t *buf, uint32_t length, TxListener *txl) {
  if (isTxInProgress()) {
    return false;
  }

  txListener = txl;

  dma->channel[txCh].config.enable = 0; // Disable channel before config
  // data sink
  dma->channel[txCh].pAddr = (&(uart->txDat));
  // data source
  dma->channel[txCh].mAddr =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
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

// Converts Duration to the number of baudrate bits as that is used for rx
// timeout. Max timeout supported by STM32 UART is 24 bit
UART_DMA::uint24_t UART_DMA::DurationToBits(Duration d) {
  return {0,
          static_cast<uint32_t>(d.milliseconds() * baud_ / 1000) & 0x00FFFFFF};
}

// Sets up reception of at least [length] chars from UART3 into [buf]
// Returns false if reception is in progress, new reception is not
// setup. Returns true if no reception is in progress and new reception
// was setup.

bool UART_DMA::startRX(const uint8_t *buf, uint32_t length, Duration timeout,
                       RxListener *rxl) {
  // UART3 reception happens on DMA1 channel 3
  if (isRxInProgress()) {
    return false;
  }

  rxListener = rxl;

  dma->channel[rxCh].config.enable = 0; // don't enable yet

  // data source
  dma->channel[rxCh].pAddr = &(uart->rxDat);
  // data sink
  dma->channel[rxCh].mAddr =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma->channel[rxCh].count = length;

  // setup rx timeout
  uart->timeout.s.rto = DurationToBits(timeout).bits;
  uart->intClear.s.rtocf = 1; // Clear rx timeout flag
  uart->request.s.rxfrq = 1;  // Clear RXNE flag
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

static bool isCharacterMatchInterrupt() {
  return UART3_BASE->status.s.cmf != 0;
}

static bool isRxTimeout() {
  // Timeout interrupt enable and RTOF - Receiver timeout
  return UART3_BASE->ctrl1.s.rtoie && UART3_BASE->status.s.rtof;
}

static bool isRxError() {
  return isRxTimeout() || UART3_BASE->status.s.ore || // overrun error
         UART3_BASE->status.s.fe;                     // frame error

  // TODO(miceuz): Enable these?
  // UART3_BASE->status.s.pe || // parity error
  // UART3_BASE->status.s.nf || // START bit noise detection flag
}

void UART_DMA::UART_ISR() {
  if (isRxError()) {
    RxError_t e = RxError_t::RX_ERROR_UNKNOWN;
    if (isRxTimeout()) {
      e = RxError_t::RX_ERROR_TIMEOUT;
    }
    if (uart->status.s.ore) {
      e = RxError_t::RX_ERROR_OVR;
    }
    if (uart->status.s.fe) {
      e = RxError_t::RX_ERROR_SERIAL_FRAMING;
    }

    uart->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags

    // Clear error flags.
    uart->intClear.s.fecf = 1;
    uart->intClear.s.orecf = 1;
    uart->intClear.s.rtocf = 1;

    // TODO define logic if stopRX() has to be here
    if (rxListener) {
      rxListener->onRxError(e);
    }
  }

  if (isCharacterMatchInterrupt()) {
    uart->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags
    uart->intClear.s.cmcf = 1; // Clear char match flag
    // TODO define logic if stopRX() has to be here
    if (rxListener) {
      rxListener->onCharacterMatch();
    }
  }
}

void UART_DMA::DMA_TX_ISR() {
  if (dma->intStat.teif2) {
    stopTX();
    if (txListener) {
      txListener->onTxError();
    }
  } else {
    stopTX();
    if (txListener) {
      txListener->onTxComplete();
    }
  }
}

void UART_DMA::DMA_RX_ISR() {
  if (dma->intStat.teif3) {
    stopRX();
    if (rxListener) {
      rxListener->onRxError(RxError_t::RX_ERROR_DMA);
    }
  } else {
    stopRX();
    if (rxListener) {
      rxListener->onRxComplete();
    }
  }
}

void DMA1_CH2_ISR() {
  DMA_Regs *dma = DMA1_BASE;
  uart_dma.DMA_TX_ISR();
  dma->intClr.gif2 = 1; // clear all channel 3 flags
}

void DMA1_CH3_ISR() {
  DMA_Regs *dma = DMA1_BASE;
  uart_dma.DMA_RX_ISR();
  dma->intClr.gif3 = 1; // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void UART3_ISR() { uart_dma.UART_ISR(); }

#endif
