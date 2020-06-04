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

#if defined(BARE_STM32) && defined(UART_VIA_DMA)

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

extern UartDma uart_dma;

// Performs UART3 initialization
void UartDma::init(uint32_t baud) {
  baud_ = baud;
  // Set baud rate register
  uart->baudrate = CPU_FREQ / baud;

  uart->ctrl3.s.dmar = 1;         // set DMAR bit to enable DMA for receiver
  uart->ctrl3.s.dmat = 1;         // set DMAT bit to enable DMA for transmitter
  uart->ctrl3.s.ddre = 1;         // DMA is disabled following a reception error
  uart->ctrl2.s.rtoen = 1;        // Enable receive timeout feature
  uart->ctrl2.s.addr = matchChar; // set match char

  uart->control3.bitfield.error_interrupt = 1; // enable interrupt on error

  uart->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags

  // Clear error flags.
  uart->interrupt_clear.bitfield.framing_error_clear = 1;
  uart->interrupt_clear.bitfield.overrun_clear = 1;
  uart->interrupt_clear.bitfield.rx_timeout_clear = 1;

  uart->control_reg1.bitfield.tx_enable = 1; // enable transmitter
  uart->control_reg1.bitfield.rx_enable = 1; // Enable receiver
  uart->control_reg1.bitfield.enable = 1;    // enable uart

  // TODO enable parity checking?

  dma->channel[rxCh].config.priority = 0b11; // high priority
  dma->channel[rxCh].config.teie = 1;        // interrupt on error
  dma->channel[rxCh].config.htie = 0;        // no half-transfer interrupt
  dma->channel[rxCh].config.tcie = 1;        // interrupt on DMA complete

  dma->channel[rxCh].config.mem2mem = 0; // memory-to-memory mode disabled
  dma->channel[rxCh].config.msize = static_cast<uint32_t>((DmaTransferSize::BITS8);
  dma->channel[rxCh].config.psize = static_cast<uint32_t>((DmaTransferSize::BITS8);
  dma->channel[rxCh].config.memInc = 1;   // increment destination (memory)
  dma->channel[rxCh].config.perInc = 0;   // don't increment source
                                          // (peripheral) address
  dma->channel[rxCh].config.circular = 0; // not circular
  dma->channel[rxCh].config.dir =
      static_cast<uint32_t>((DmaChannelDir::PERIPHERAL_TO_MEM);

  dma->channel[txCh].config.priority = 0b11; // high priority
  dma->channel[txCh].config.teie = 1;        // interrupt on error
  dma->channel[txCh].config.htie = 0;        // no half-transfer interrupt
  dma->channel[txCh].config.tcie = 1;        // DMA complete interrupt enabled

  dma->channel[txCh].config.mem2mem = 0; // memory-to-memory mode disabled
  dma->channel[txCh].config.msize = static_cast<uint32_t>((DmaTransferSize::BITS8);
  dma->channel[txCh].config.psize = static_cast<uint32_t>((DmaTransferSize::BITS8);
  dma->channel[txCh].config.memInc = 1;   // increment source (memory) address
  dma->channel[txCh].config.perInc = 0;   // don't increment dest (peripheral)
                                          // address
  dma->channel[txCh].config.circular = 0; // not circular
  dma->channel[txCh].config.dir =
      static_cast<uint32_t>((DmaChannelDir::MEM_TO_PERIPHERAL);
}

// Sets up an interrupt on matching char incoming form UART3
void UartDma::CharMatchEnable() {
  uart->intClear.s.cmcf = 1; // Clear char match flag
  uart->ctrl1.s.cmie = 1;    // Enable character match interrupt
}

// Returns true if DMA TX is in progress
bool UartDma::TxInProgress() const {
  // TODO thread safety
  return tx_in_progress_;
}

// Returns true if DMA RX is in progress
bool UartDma::RxInProgress() const {
  // TODO thread safety
  return rx_in_progress_;
}

// Sets up UART to transfer [length] characters from [buf]
// Returns false if DMA transmission is in progress, does not
// interrupt previous transmission.
// Returns true if no transmission is in progress
bool UartDma::startTX(uint8_t *buf, uint32_t length, TxListener *txl) {
  if (TxInProgress()) {
    return false;
  }

  tx_listener_ = txl;

  dma->channel[tx_channel_].config.enable = 0; // Disable channel before config
  // data sink
  dma->channel[tx_channel_].peripheral_address = &(uart->tx_data);
  // data source
  dma->channel[tx_channel_].memory_addres =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma->channel[tx_channel_].count = length & 0x0000FFFF;

  dma->channel[tx_channel_].config.enable = 1; // go!

  tx_in_progress_ = true;

  return true;
}

void UartDma::stopTX() {
  if (TxInProgress()) {
    // Disable DMA channel
    dma->channel[tx_channel_].config.enable = 0;
    // TODO thread safety
    tx_in_progress_ = 0;
  }
}

// Converts Duration to the number of baudrate bits as that is used for rx
// timeout. Max timeout supported by STM32 UART is 24 bit
UartDma::uint24_t UartDma::DurationToBits(Duration d) {
  return {0,
          static_cast<uint32_t>(d.milliseconds() * baud_ / 1000) & 0x00FFFFFF};
}

// Sets up reception of at least [length] chars from UART3 into [buf]
// Returns false if reception is in progress, new reception is not
// setup. Returns true if no reception is in progress and new reception
// was setup.

bool UartDma::startRX(const uint8_t *buf, uint32_t length, Duration timeout,
                       RxListener *rxl) {
  // UART3 reception happens on DMA1 channel 3
  if (isRxInProgress()) {
    return false;
  }

  rx_listener_ = rxl;

  dma->channel[rx_channel_].config.enable = 0; // don't enable yet

  // data source
  dma->channel[rx_channel_].peripheral_address = &(uart->rx_data);
  // data sink
  dma->channel[rx_channel_].memory_address =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma->channel[rx_channel_].count = length;

  // setup rx timeout
  uart->timeout.s.rto = DurationToBits(timeout).bits;
  uart->intClear.s.rtocf = 1; // Clear rx timeout flag
  uart->request.s.rxfrq = 1;  // Clear RXNE flag
  uart->ctrl1.s.rtoie = 1;    // Enable receive timeout interrupt

  dma->channel[rx_channel_].config.enable = 1; // go!

  rx_in_progress = true;

  return true;
}

uint32_t UartDma::getRxBytesLeft() { return dma->channel[2].count; }

void UartDma::stopRX() {
  if (RxInProgress()) {
    uart->ctrl1.s.rtoie = 0;           // Disable receive timeout interrupt
    dma->channel[rx_channel_].config.enable = 0; // Disable DMA channel
    // TODO thread safety
    rx_in_progress = false;
  }
}

static bool isCharacterMatchInterrupt() {
  return UART3_BASE->status.s.cmf != 0;
}

static bool GetRxTimeout() {
  // Timeout interrupt enable and RTOF - Receiver timeout
  return UART3_BASE->ctrl1.s.rtoie && UART3_BASE->status.s.rtof;
}

static bool GetRxError() {
  return isRxTimeout() || UART3_BASE->status.s.ore || // overrun error
         UART3_BASE->status.s.fe;                     // frame error

  // TODO(miceuz): Enable these?
  // UART3_BASE->status.s.pe || // parity error
  // UART3_BASE->status.s.nf || // START bit noise detection flag
}

void UartDma::UartISR() {
  if (GetRxError()) {
    RxError_t e = RxError_t::RX_ERROR_UNKNOWN;
    if (RetRxTimeout()) {
      e = RxError_t::RX_ERROR_TIMEOUT;
    }
    if (uart->status.bitfield.overrun_error) {
      e = RxError_t::RX_ERROR_OVR;
    }
    if (uart->status.bitfield.framing_error) {
      e = RxError_t::RX_ERROR_SERIAL_FRAMING;
    }

    uart->request.s.rxfrq = 1; // Clear RXNE flag before clearing other flags

    // Clear error flags.
    uart->interrupt_clear.bitfield.framing_error_clear = 1;
    uart->interrupt_clear.bitfield.overrun_clear = 1;
    uart->interrupt_clear.bitfield.rx_timeout_clear = 1;

    // TODO define logic if stopRX() has to be here
    if (rx_listener_) {
      rx_ristener_->OnRxError(e);
    }
  }

  if (CharacterMatchInterrupt()) {
    uart->request.bitfield.flush_rx = 1; // Clear RXNE flag before clearing other flags
    uart->interrupt_clear.bitfield.char_match_clear = 1; // Clear char match flag
    // TODO define logic if stopRX() has to be here
    if (rx_listener_) {
      rx_listener_->OnCharacterMatch();
    }
  }
}

void UartDma::DMA_TX_ISR() {
  if (dma->interrupt_status.teif2) {
    StopTX();
    if (tx_listener_) {
      tx_listener_->onTxError();
    }
  } else {
    StopTX();
    if (tx_listener_) {
      tx_listener_->onTxComplete();
    }
  }
}

void UartDma::DMA_RX_ISR() {
  if (dma->interrupt_status.teif3) {
    StopRX();
    if (rx_listener_) {
      rx_listener_->OnRxError(RxError_t::RX_ERROR_DMA);
    }
  } else {
    StopRX();
    if (rx_listener_) {
      rx_listener_->OnRxComplete();
    }
  }
}

void DMA1_CH2_ISR() {
  DmaReg *dma = Dma1Base;
  dma_uart.DmaTxISR();
  dma->interrupt_clear.gif2 = 1; // clear all channel 3 flags
}

void DMA1_CH3_ISR() {
  DmaReg *dma = Dma1Base;
  dma_uart.DmaRxISR();
  dma->interrupt_clear.gif3 = 1; // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void UART3_ISR() { uart_dma.UART_ISR(); }

#endif
