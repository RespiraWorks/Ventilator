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

// STM32 UART3 driver based on DMA transfers.

// Direct Memory Access mode in MCU allows to set up a memory buffer
// transfer by means of hardware with no CPU intervention.
// CPU responsibility is to set up a DMA channel configuring it's
// endpoints as peripheral and memory. Upon transfer completion
// CPU is notified via interrupt.

// This driver also provides Character Match interrupt on reception.
// UART will issue an interrupt upon receipt of the specified
// character.

extern UartDma dma_uart;

// Performs UART3 initialization
void UartDma::Init(int baud) {
  // Set baud rate register
  uart->baudrate = CPU_FREQ / baud;

  uart->control3.bitfield.rx_dma = 1; // set DMAR bit to enable DMA for receiver
  uart->control3.bitfield.tx_dma =
      1; // set DMAT bit to enable DMA for transmitter
  uart->control3.bitfield.dma_disable_on_rx_error =
      1; // DMA is disabled following a reception error
  uart->control2.bitfield.rx_timeout_enable =
      1;                                      // Enable receive timeout feature
  uart->control2.bitfield.addr = match_char_; // set match char

  uart->control3.bitfield.error_interrupt = 1; // enable interrupt on error

  uart->request.bitfield.flush_rx =
      1; // Clear RXNE flag before clearing other flags

  // Clear error flags.
  uart->interrupt_clear.bitfield.framing_error_clear = 1;
  uart->interrupt_clear.bitfield.overrun_clear = 1;
  uart->interrupt_clear.bitfield.rx_timeout_clear = 1;

  uart->control_reg1.bitfield.tx_enable = 1; // enable transmitter
  uart->control_reg1.bitfield.rx_enable = 1; // Enable receiver
  uart->control_reg1.bitfield.enable = 1;    // enable uart

  // TODO enable parity checking?

  dma->channel[rx_channel_].config.priority = 0b11;        // high priority
  dma->channel[rx_channel_].config.tx_error_interrupt = 1; // interrupt on error
  dma->channel[rx_channel_].config.half_tx_interrupt =
      0; // no half-transfer interrupt
  dma->channel[rx_channel_].config.tx_complete_interrupt =
      1; // interrupt on DMA complete

  dma->channel[rx_channel_].config.mem2mem =
      0; // memory-to-memory mode disabled
  dma->channel[rx_channel_].config.memory_size =
      static_cast<uint32_t>(DmaTransferSize::Byte);
  dma->channel[rx_channel_].config.peripheral_size =
      static_cast<uint32_t>(DmaTransferSize::Byte);
  dma->channel[rx_channel_].config.memory_increment =
      1; // increment destination (memory)
  dma->channel[rx_channel_].config.peripheral_increment =
      0;                                         // don't increment source
                                                 // (peripheral) address
  dma->channel[rx_channel_].config.circular = 0; // not circular
  dma->channel[rx_channel_].config.direction =
      static_cast<uint32_t>(DmaChannelDir::PeripheralToMemory);

  dma->channel[tx_channel_].config.priority = 0b11;        // high priority
  dma->channel[tx_channel_].config.tx_error_interrupt = 1; // interrupt on error
  dma->channel[tx_channel_].config.half_tx_interrupt =
      0; // no half-transfer interrupt
  dma->channel[tx_channel_].config.tx_complete_interrupt =
      1; // DMA complete interrupt enabled

  dma->channel[tx_channel_].config.mem2mem =
      0; // memory-to-memory mode disabled
  dma->channel[tx_channel_].config.memory_size =
      static_cast<uint32_t>(DmaTransferSize::Byte);
  dma->channel[tx_channel_].config.peripheral_size =
      static_cast<uint32_t>(DmaTransferSize::Byte);
  dma->channel[tx_channel_].config.memory_increment =
      1; // increment source (memory) address
  dma->channel[tx_channel_].config.peripheral_increment =
      0;                                         // don't increment dest
                                                 // (peripheral) address
  dma->channel[tx_channel_].config.circular = 0; // not circular
  dma->channel[tx_channel_].config.direction =
      static_cast<uint32_t>(DmaChannelDir::MemoryToPeripheral);
}

// Sets up an interrupt on matching char incoming form UART3
void UartDma::CharMatchEnable() {
  uart->interrupt_clear.bitfield.char_match_clear = 1; // Clear char match flag
  uart->control_reg1.bitfield.char_match_interrupt =
      1; // Enable character match interrupt
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
bool UartDma::StartTX(char *buf, uint32_t length) {
  if (TxInProgress()) {
    return false;
  }

  dma->channel[tx_channel_].config.enable = 0; // Disable channel before config
  // data sink
  dma->channel[tx_channel_].peripheral_address = &(uart->tx_data);
  // data source
  dma->channel[tx_channel_].memory_address = buf;
  // data length
  dma->channel[tx_channel_].count = length & 0x0000FFFF;

  dma->channel[tx_channel_].config.enable = 1; // go!

  tx_in_progress_ = true;

  return true;
}

void UartDma::StopTX() {
  if (TxInProgress()) {
    // Disable DMA channel
    dma->channel[tx_channel_].config.enable = 0;
    // TODO thread safety
    tx_in_progress_ = 0;
  }
}

// Sets up reception of at least [length] chars from UART3 into [buf]
// [timeout] is the number of baudrate bits for which RX line is
// allowed to be idle before asserting timeout error.
// Returns false if reception is in progress, new reception is not
// setup. Returns true if no reception is in progress and new reception
// was setup.

bool UartDma::StartRX(char *buf, uint32_t length, uint32_t timeout) {
  // UART3 reception happens on DMA1 channel 3
  if (RxInProgress()) {
    return false;
  }

  dma->channel[rx_channel_].config.enable = 0; // don't enable yet

  // data source
  dma->channel[rx_channel_].peripheral_address = &(uart->rx_data);
  // data sink
  dma->channel[rx_channel_].memory_address = buf;
  // data length
  dma->channel[rx_channel_].count = length;

  // setup rx timeout
  // max timeout is 24 bit
  uart->timeout.bitfield.rx_timeout = timeout & 0x00FFFFFF;
  uart->interrupt_clear.bitfield.rx_timeout_clear = 1; // Clear rx timeout flag
  uart->request.bitfield.flush_rx = 1;                 // Clear RXNE flag
  uart->control_reg1.bitfield.rx_timeout_interrupt =
      1; // Enable receive timeout interrupt

  dma->channel[rx_channel_].config.enable = 1; // go!

  rx_in_progress_ = true;

  return true;
}

uint32_t UartDma::getRxBytesLeft() { return dma->channel[2].count; }

void UartDma::stopRX() {
  if (RxInProgress()) {
    uart->control_reg1.bitfield.rx_timeout_interrupt =
        0;                             // Disable receive timeout interrupt
    dma->channel[2].config.enable = 0; // Disable DMA channel
    // TODO thread safety
    rx_in_progress_ = false;
  }
}

static bool CharacterMatchInterrupt() {
  return kUart3Base->status.bitfield.char_match != 0;
}

static bool RxTimeout() {
  // Timeout interrupt enable and RTOF - Receiver timeout
  return kUart3Base->control_reg1.bitfield.rx_timeout_interrupt &&
         kUart3Base->status.bitfield.rx_timeout;
}

static bool GetRxError() {
  return RxTimeout() ||
         kUart3Base->status.bitfield.overrun_error || // overrun error
         kUart3Base->status.bitfield.framing_error;   // frame error

  // TODO(miceuz): Enable these?
  // kUart3Base->status.bitfield.parity_error || // parity error
  // kUart3Base->status.bitfield.noise_detection || // START bit noise
  // detection flag
}

void UartDma::UartISR() {
  if (GetRxError()) {
    RxError e = RxError::RxUnknownError;
    if (RxTimeout()) {
      e = RxError::RxTimeout;
    }
    if (uart->status.bitfield.overrun_error) {
      e = RxError::RxOverflow;
    }
    if (uart->status.bitfield.framing_error) {
      e = RxError::RxFramingError;
    }

    uart->request.bitfield.flush_rx =
        1; // Clear RXNE flag before clearing other flags

    // Clear error flags.
    uart->interrupt_clear.bitfield.framing_error_clear = 1;
    uart->interrupt_clear.bitfield.overrun_clear = 1;
    uart->interrupt_clear.bitfield.rx_timeout_clear = 1;

    // TODO define logic if stopRX() has to be here
    rx_listener_->OnRxError(e);
  }

  if (CharacterMatchInterrupt()) {
    uart->request.bitfield.flush_rx =
        1; // Clear RXNE flag before clearing other flags
    uart->interrupt_clear.bitfield.char_match_clear =
        1; // Clear char match flag
    // TODO define logic if stopRX() has to be here
    rx_listener_->OnCharacterMatch();
  }
}

void UartDma::DmaTxISR() {
  if (dma->interrupt_status.teif2) {
    StopTX();
    tx_listener_->OnTxError();
  } else {
    StopTX();
    tx_listener_->OnTxComplete();
  }
}

void UartDma::DmaRxISR() {
  if (dma->interrupt_status.teif3) {
    StopRX();
    rx_listener_->OnRxError(RxError_t::RxDmaError);
  } else {
    StopRX();
    rx_listener_->onRxComplete();
  }
}

void DMA1Channel2ISR() {
  DmaReg *dma = Dma1Base;
  dma_uart.DmaTxISR();
  dma->interrupt_clear.gif2 = 1; // clear all channel 3 flags
}

void DMA1Channel3ISR() {
  DmaReg *dma = Dma1Base;
  dma_uart.DmaRxISR();
  dma->interrupt_clear.gif3 = 1; // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void Uart3ISR() { dma_uart.UartISR(); }

#endif
