/* Copyright 2020-2022, RespiraWorks

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

#include "hal_stm32_regs.h"

// STM32 UART3 driver based on DMA transfers.

// Direct Memory Access mode in MCU allows to set up a memory buffer
// transfer by means of hardware with no CPU intervention.
// CPU responsibility is to set up a DMA channel configuring it's
// endpoints as peripheral and memory. Upon transfer completion
// CPU is notified via interrupt.

// This driver also provides Character Match callback on match_char reception.

// Performs UART initialization
void UartDma::initialize(const Frequency cpu_frequency, const Frequency baud, DMA::Base dma,
                         DMA::Channel tx_channel, DMA::Channel rx_channel) {
  // Set baud rate register
  uart_->baudrate = static_cast<uint32_t>(cpu_frequency / baud);

  uart_->control3.bitfield.rx_dma = 1;  // set DMAR bit to enable DMA for receiver
  uart_->control3.bitfield.tx_dma = 1;  // set DMAT bit to enable DMA for transmitter
  uart_->control3.bitfield.dma_disable_on_rx_error = 1;  // DMA disabled following a reception error
  uart_->control2.bitfield.rx_timeout_enable = 0;        // Disable receive timeout feature
  uart_->control2.bitfield.addr = match_char_;           // set match char

  uart_->control3.bitfield.error_interrupt = 1;  // enable interrupt on error
  uart_->request.bitfield.flush_rx = 1;          // Clear RXNE flag before clearing other flags

  // Clear error flags.
  uart_->interrupt_clear.bitfield.framing_error_clear = 1;
  uart_->interrupt_clear.bitfield.overrun_clear = 1;
  uart_->interrupt_clear.bitfield.rx_timeout_clear = 1;

  uart_->control_reg1.bitfield.tx_enable = 1;  // enable transmitter
  uart_->control_reg1.bitfield.rx_enable = 1;  // Enable receiver
  uart_->control_reg1.bitfield.enable = 1;     // enable uart

  // TODO enable parity checking?

  rx_dma_.emplace(dma, rx_channel);
  tx_dma_.emplace(dma, tx_channel);
  rx_dma_->Initialize(2, &(uart_->rx_data), DMA::ChannelDir::PeripheralToMemory, 1,
                      DMA::ChannelPriority::Highest);
  tx_dma_->Initialize(2, &(uart_->tx_data), DMA::ChannelDir::MemoryToPeripheral, 1,
                      DMA::ChannelPriority::Highest);
}

// Sets up an interrupt on matching char incoming form UART3
void UartDma::enable_character_match() {
  uart_->interrupt_clear.bitfield.char_match_clear = 1;   // Clear char match flag
  uart_->control_reg1.bitfield.char_match_interrupt = 1;  // Enable character match interrupt
}

// Returns true if DMA TX is in progress
bool UartDma::tx_in_progress() const {
  // TODO thread safety
  return tx_in_progress_;
}

// Returns true if DMA RX is in progress
bool UartDma::rx_in_progress() const {
  // TODO thread safety
  return rx_in_progress_;
}

// Sets up UART to transfer [length] characters from [buf]
// Returns false if DMA transmission is in progress, does not
// interrupt previous transmission.
// Returns true if no transmission is in progress
bool UartDma::start_tx(uint8_t *buffer, uint32_t length, TxListener *txl) {
  if (tx_in_progress()) {
    return false;
  }

  tx_listener_ = txl;

  tx_dma_->Disable();  // Disable channel before config
  tx_dma_->SetupTransfer(buffer, length & 0x0000FFFF);
  tx_in_progress_ = true;
  tx_dma_->Enable();

  return true;
}

void UartDma::stop_tx() {
  if (tx_in_progress()) {
    // Disable DMA channel
    tx_dma_->Disable();
    // TODO thread safety
    tx_in_progress_ = false;
  }
}

// Sets up reception of exactly [length] chars from UART3 into [buf]
// [timeout] is the number of baud rate bits for which RX line is
// allowed to be idle before issuing timeout error.
// on_character_match callback will be called if a match_char is seen on the RX
// line
// on_rx_complete callback will be called when the [length] bytes are received.
// DMA and UART is disabled in this case.
// on_rx_error is called if UART or DMA errors occur:
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

bool UartDma::start_rx(uint8_t *buffer, uint32_t length, RxListener *rxl) {
  // UART3 reception happens on DMA1 channel 3
  if (rx_in_progress()) {
    return false;
  }

  rx_listener_ = rxl;

  rx_dma_->Disable();  // Disable channel before config
  rx_dma_->SetupTransfer(buffer, length & 0x0000FFFF);

  uart_->interrupt_clear.bitfield.rx_timeout_clear = 1;  // Clear rx timeout flag
  uart_->request.bitfield.flush_rx = 1;                  // Clear RXNE flag

  rx_in_progress_ = true;

  rx_dma_->Enable();

  return true;
}

uint32_t UartDma::rx_bytes_left() { return tx_dma_->Remaining(); }

void UartDma::stop_rx() {
  if (rx_in_progress()) {
    uart_->control_reg1.bitfield.rx_timeout_interrupt = 0;  // Disable receive timeout interrupt
    rx_dma_->Disable();
    // TODO thread safety
    rx_in_progress_ = false;
  }
}

static bool character_match_interrupt() { return Uart3Base->status.bitfield.char_match != 0; }

static bool rx_error() {
  return Uart3Base->status.bitfield.overrun_error ||  // overrun error
         Uart3Base->status.bitfield.framing_error;    // frame error

  // TODO(miceuz): Enable these?
  // UART3_BASE->status.s.pe || // parity error
  // UART3_BASE->status.s.nf || // START bit noise detection flag
}

// ISR handler for the UART peripheral.
// Calls on_rx_error and on_character_match functions of the rx_listener_ as
// those events are provided by UART peripheral. on_rx_complete is called by DMA
// ISR
void UartDma::UART_interrupt_handler() {
  if (rx_error()) {
    RxError e = RxError::Unknown;
    if (uart_->status.bitfield.overrun_error) {
      e = RxError::Overrun;
    }
    if (uart_->status.bitfield.framing_error) {
      e = RxError::SerialFraming;
    }

    uart_->request.bitfield.flush_rx = 1;  // Clear RXNE flag before clearing other flags

    // Clear error flags.
    uart_->interrupt_clear.bitfield.framing_error_clear = 1;
    uart_->interrupt_clear.bitfield.overrun_clear = 1;
    uart_->interrupt_clear.bitfield.rx_timeout_clear = 1;

    stop_rx();
    if (rx_listener_) {
      rx_listener_->on_rx_error(e);
    }
    return;
  }

  if (character_match_interrupt()) {
    uart_->request.bitfield.flush_rx = 1;  // Clear RXNE flag before clearing other flags
    uart_->interrupt_clear.bitfield.char_match_clear = 1;  // Clear char match flag

    if (rx_listener_) {
      rx_listener_->on_character_match();
    }
  }
}

// ISR handler for the DMA peripheral responsible for transmission.
// Calls on_tx_error and on_tx_complete functions of the tx_listener_
void UartDma::DMA_tx_interrupt_handler() {
  stop_tx();
  if (tx_dma_->InterruptStatus(DMA::Interrupt::TransferError)) {
    if (tx_listener_) {
      tx_listener_->on_tx_error();
    }
  } else {
    if (tx_listener_) {
      tx_listener_->on_tx_complete();
    }
  }
  tx_dma_->ClearInterrupt(DMA::Interrupt::Global);
}

// ISR handler for the DMA peripheral responsible for reception.
// Calls on_rx_error and on_rx_complete functions of the rx_listener_
void UartDma::DMA_rx_interrupt_handler() {
  stop_rx();
  if (rx_dma_->InterruptStatus(DMA::Interrupt::TransferError)) {
    if (rx_listener_) {
      rx_listener_->on_rx_error(RxError::DMA);
    }
  } else {
    if (rx_listener_) {
      rx_listener_->on_rx_complete();
    }
  }
  rx_dma_->ClearInterrupt(DMA::Interrupt::Global);
}

#endif
