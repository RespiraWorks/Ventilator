#if defined(BARE_STM32) && defined(UART_VIA_DMA)

#include "uart_dma.h"
#include "hal_stm32.h"
#include "hal_stm32_regs.h"

// STM32 UART3 driver based on DMA transfers.

// Direct Memory Access mode in MCU allows to set up a memory buffer
// transfer by means of hardware with no CPU intervention.
// CPU responsibility is to set up a DMA channel configuring it's
// endpoints as peripheral and memory. Upon transfer completion
// CPU is notified via interrupt.

// This driver also provides Character Match callback on match_char reception.

extern UART_DMA uart_dma;

// Performs UART3 initialization
void UART_DMA::init(uint32_t baud) {
  baud_ = baud;
  // Set baud rate register
  uart_->baud = CPU_FREQ / baud;

  uart_->ctrl3.s.dmar = 1;  // set DMAR bit to enable DMA for receiver
  uart_->ctrl3.s.dmat = 1;  // set DMAT bit to enable DMA for transmitter
  uart_->ctrl3.s.ddre = 1;  // DMA is disabled following a reception error
  uart_->ctrl2.s.rtoen = 0; // Disable receive timeout feature
  uart_->ctrl2.s.addr = match_char_; // set match char

  uart_->ctrl3.s.eie = 1; // enable interrupt on error

  uart_->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags

  // Clear error flags.
  uart_->intClear.s.fecf = 1;
  uart_->intClear.s.orecf = 1;
  uart_->intClear.s.rtocf = 1;

  uart_->ctrl1.s.te = 1; // enable transmitter
  uart_->ctrl1.s.re = 1; // Enable receiver
  uart_->ctrl1.s.ue = 1; // enable uart

  dma_->channel[rx_ch_].config.priority = 0b11; // high priority
  dma_->channel[rx_ch_].config.teie = 1;        // interrupt on error
  dma_->channel[rx_ch_].config.htie = 0;        // no half-transfer interrupt
  dma_->channel[rx_ch_].config.tcie = 1;        // interrupt on DMA complete

  dma_->channel[rx_ch_].config.mem2mem = 0; // memory-to-memory mode disabled
  dma_->channel[rx_ch_].config.msize = static_cast<REG>(DmaTransferSize::BITS8);
  dma_->channel[rx_ch_].config.psize = static_cast<REG>(DmaTransferSize::BITS8);
  dma_->channel[rx_ch_].config.memInc = 1;   // increment destination (memory)
  dma_->channel[rx_ch_].config.perInc = 0;   // don't increment source
                                             // (peripheral) address
  dma_->channel[rx_ch_].config.circular = 0; // not circular
  dma_->channel[rx_ch_].config.dir =
      static_cast<REG>(DmaChannelDir::PERIPHERAL_TO_MEM);

  dma_->channel[tx_ch_].config.priority = 0b11; // high priority
  dma_->channel[tx_ch_].config.teie = 1;        // interrupt on error
  dma_->channel[tx_ch_].config.htie = 0;        // no half-transfer interrupt
  dma_->channel[tx_ch_].config.tcie = 1; // DMA complete interrupt enabled

  dma_->channel[tx_ch_].config.mem2mem = 0; // memory-to-memory mode disabled
  dma_->channel[tx_ch_].config.msize = static_cast<REG>(DmaTransferSize::BITS8);
  dma_->channel[tx_ch_].config.psize = static_cast<REG>(DmaTransferSize::BITS8);
  dma_->channel[tx_ch_].config.memInc = 1; // increment source (memory) address
  dma_->channel[tx_ch_].config.perInc = 0; // don't increment dest (peripheral)
                                           // address
  dma_->channel[tx_ch_].config.circular = 0; // not circular
  dma_->channel[tx_ch_].config.dir =
      static_cast<REG>(DmaChannelDir::MEM_TO_PERIPHERAL);
}

// Sets up an interrupt on matching char incomming form UART3
void UART_DMA::charMatchEnable() {
  uart_->intClear.s.cmcf = 1; // Clear char match flag
  uart_->ctrl1.s.cmie = 1;    // Enable character match interrupt
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

  tx_listener_ = txl;

  dma_->channel[tx_ch_].config.enable = 0; // Disable channel before config
  // data sink
  dma_->channel[tx_ch_].pAddr = &uart_->txDat;
  // data source
  dma_->channel[tx_ch_].mAddr =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma_->channel[tx_ch_].count = length & 0x0000FFFF;

  dma_->channel[tx_ch_].config.enable = 1; // go!

  tx_in_progress = true;

  return true;
}

void UART_DMA::stopTX() {
  if (isTxInProgress()) {
    // Disable DMA channel
    dma_->channel[1].config.enable = 0;
    // TODO thread safety
    tx_in_progress = 0;
  }
}

// Sets up reception of exactly [length] chars from UART3 into [buf]
// [timeout] is the number of baudrate bits for which RX line is
// allowed to be idle before issuing timeout error.
// OnCharacterMatch callback will be called if a match_char is seen on the RX
// line
// OnRxComplete callback will be called when the [length] bytes are received.
// DMA and UART is disabled in this case.
// OnRxError is called if UART or DMA errors occur:
// OVERRUN - if received byte was not read before a new byte is received
// SERIAL_FRAMING - when a de-synchronization, excessive noise or a break
// character is detected
// TIMEOUT - if RX line is idle for [timeout] after last reception of
// character.
// DMA - if DMA transfer is ordered into a restricted memory address
// Note that OVERRUN, SERIAL_FRAMING, TIMEOUT errors do not
// stop the reception, DMA will still wait for ordered number of bytes.
// DMA error means that DMA transfer was stopped though.
//
// Returns false if reception is in progress, new reception is not setup.
// Returns true if no reception is in progress and new reception was setup.

bool UART_DMA::startRX(const uint8_t *buf, uint32_t length, RxListener *rxl) {
  // UART3 reception happens on DMA1 channel 3
  if (isRxInProgress()) {
    return false;
  }

  rx_listener_ = rxl;

  dma_->channel[rx_ch_].config.enable = 0; // don't enable yet

  // data source
  dma_->channel[rx_ch_].pAddr = &uart_->rxDat;
  // data sink
  dma_->channel[rx_ch_].mAddr =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma_->channel[rx_ch_].count = length;

  uart_->intClear.s.rtocf = 1; // Clear rx timeout flag
  uart_->request.s.rxfrq = 1;  // Clear RXNE flag

  dma_->channel[rx_ch_].config.enable = 1; // go!

  rx_in_progress = true;

  return true;
}

uint32_t UART_DMA::getRxBytesLeft() { return dma_->channel[2].count; }

void UART_DMA::stopRX() {
  if (isRxInProgress()) {
    uart_->ctrl1.s.rtoie = 0;           // Disable receive timeout interrupt
    dma_->channel[2].config.enable = 0; // Disable DMA channel
    // TODO thread safety
    rx_in_progress = 0;
  }
}

static bool isCharacterMatchInterrupt() {
  return UART3_BASE->status.s.cmf != 0;
}

static bool isRxError() {
  return UART3_BASE->status.s.ore || // overrun error
         UART3_BASE->status.s.fe;    // frame error

  // TODO(miceuz): Enable these?
  // UART3_BASE->status.s.pe || // parity error
  // UART3_BASE->status.s.nf || // START bit noise detection flag
}

// ISR handler for the UART peripheral.
// Calls OnRxError and OnCharacterMatch functions of the rx_listener_ as those
// events are provided by UART peripheral. OnRxComplete is called by DMA ISR
void UART_DMA::UART_ISR() {
  if (isRxError()) {
    RxError e = RxError::UNKNOWN;
    if (isRxTimeout()) {
      e = RxError::TIMEOUT;
    }
    if (uart_->status.s.ore) {
      e = RxError::OVERFLOW;
    }
    if (uart_->status.s.fe) {
      e = RxError::SERIAL_FRAMING;
    }

    uart_->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags

    // Clear error flags.
    uart_->intClear.s.fecf = 1;
    uart_->intClear.s.orecf = 1;
    uart_->intClear.s.rtocf = 1;

    if (rx_listener_) {
      rx_listener_->OnRxError(e);
    }
    return;
  }

  if (isCharacterMatchInterrupt()) {
    uart_->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags
    uart_->intClear.s.cmcf = 1; // Clear char match flag

    if (rx_listener_) {
      rx_listener_->OnCharacterMatch();
    }
  }
}

// ISR handler for the DMA peripheral responsible for transmission.
// Calls OnRxError and OnTxComplete functions of the tx_listener_
void UART_DMA::DMA_TX_ISR() {
  if (dma_->intStat.teif2) {
    stopTX();
    if (tx_listener_) {
      tx_listener_->OnTxError();
    }
  } else {
    stopTX();
    if (tx_listener_) {
      tx_listener_->OnTxComplete();
    }
  }
}

// ISR handler for the DMA peripheral responsible for reception.
// Calls OnRxError and OnRxComplete functions of the rx_listener_
void UART_DMA::DMA_RX_ISR() {
  if (dma_->intStat.teif3) {
    stopRX();
    if (rx_listener_) {
      rx_listener_->OnRxError(RxError::DMA);
    }
  } else {
    stopRX();
    if (rx_listener_) {
      rx_listener_->OnRxComplete();
    }
  }
}

void DMA1_CH2_ISR() {
  DMA_Regs *dma = DMA1_BASE;
  uart_dma.DMA_TX_ISR();
  dma_->intClr.gif2 = 1; // clear all channel 3 flags
}

void DMA1_CH3_ISR() {
  DMA_Regs *dma = DMA1_BASE;
  uart_dma.DMA_RX_ISR();
  dma_->intClr.gif3 = 1; // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void UART3_ISR() { uart_dma.UART_ISR(); }

#endif
