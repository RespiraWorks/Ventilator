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

#include "hal_stm32_regs.h"

// STM32 UART3 driver based on DMA transfers.

// Direct Memory Access mode in MCU allows to set up a memory buffer
// transfer by means of hardware with no CPU intervention.
// CPU responsibility is to set up a DMA channel configuring it's
// endpoints as peripheral and memory. Upon transfer completion
// CPU is notified via interrupt.

// This driver also provides Character Match callback on match_char reception.

extern UartDma uart_dma;

// Performs UART3 initialization
void UartDma::initialize(const Frequency cpu_frequency, const Frequency baud) {
  baud_ = baud;
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

  auto &rx_dma_config = dma_->channel[rx_channel_].config;
  rx_dma_config.priority = 0b11;            // high priority
  rx_dma_config.tx_error_interrupt = 1;     // interrupt on error
  rx_dma_config.half_tx_interrupt = 0;      // no half-transfer interrupt
  rx_dma_config.tx_complete_interrupt = 1;  // interrupt on DMA complete
  rx_dma_config.mem2mem = 0;                // memory-to-memory mode disabled
  rx_dma_config.memory_size = static_cast<uint32_t>(DMA::TransferSize::Byte);
  rx_dma_config.peripheral_size = static_cast<uint32_t>(DMA::TransferSize::Byte);
  rx_dma_config.memory_increment = 1;      // increment destination (memory)
  rx_dma_config.peripheral_increment = 0;  // don't increment source (peripheral) address
  rx_dma_config.circular = 0;              // not circular
  rx_dma_config.direction = static_cast<uint32_t>(DMA::ChannelDir::PeripheralToMemory);

  auto &tx_dma_config = dma_->channel[tx_channel_].config;
  tx_dma_config.priority = 0b11;            // high priority
  tx_dma_config.tx_error_interrupt = 1;     // interrupt on error
  tx_dma_config.half_tx_interrupt = 0;      // no half-transfer interrupt
  tx_dma_config.tx_complete_interrupt = 1;  // DMA complete interrupt enabled
  tx_dma_config.mem2mem = 0;                // memory-to-memory mode disabled
  tx_dma_config.memory_size = static_cast<uint32_t>(DMA::TransferSize::Byte);
  tx_dma_config.peripheral_size = static_cast<uint32_t>(DMA::TransferSize::Byte);
  tx_dma_config.memory_increment = 1;      // increment source (memory) address
  tx_dma_config.peripheral_increment = 0;  // don't increment dest (peripheral) address
  tx_dma_config.circular = 0;              // not circular
  tx_dma_config.direction = static_cast<uint32_t>(DMA::ChannelDir::MemoryToPeripheral);
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
bool UartDma::start_tx(uint8_t *buf, uint32_t length, TxListener *txl) {
  if (tx_in_progress()) {
    return false;
  }

  tx_listener_ = txl;

  auto &tx_dma = dma_->channel[tx_channel_];
  tx_dma.config.enable = 0;                       // Disable channel before config
  tx_dma.peripheral_address = &(uart_->tx_data);  // data sink
  tx_dma.memory_address = const_cast<void *>(reinterpret_cast<const void *>(buf));  // data source
  tx_dma.count = length & 0x0000FFFF;                                               // data length
  tx_dma.config.enable = 1;                                                         // go!

  tx_in_progress_ = true;

  return true;
}

void UartDma::stop_tx() {
  if (tx_in_progress()) {
    // Disable DMA channel
    dma_->channel[tx_channel_].config.enable = 0;
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

bool UartDma::start_rx(uint8_t *buf, uint32_t length, RxListener *rxl) {
  // UART3 reception happens on DMA1 channel 3
  if (rx_in_progress()) {
    return false;
  }

  rx_listener_ = rxl;

  auto &rx_dma = dma_->channel[rx_channel_];

  rx_dma.config.enable = 0;  // don't enable yet

  rx_dma.peripheral_address = &(uart_->rx_data);                                    // data source
  rx_dma.memory_address = const_cast<void *>(reinterpret_cast<const void *>(buf));  // data sink
  rx_dma.count = length;                                                            // data length

  uart_->interrupt_clear.bitfield.rx_timeout_clear = 1;  // Clear rx timeout flag
  uart_->request.bitfield.flush_rx = 1;                  // Clear RXNE flag

  rx_dma.config.enable = 1;  // go!

  rx_in_progress_ = true;

  return true;
}

uint32_t UartDma::rx_bytes_left() { return dma_->channel[2].count; }

void UartDma::stop_rx() {
  if (rx_in_progress()) {
    uart_->control_reg1.bitfield.rx_timeout_interrupt = 0;  // Disable receive timeout interrupt
    dma_->channel[rx_channel_].config.enable = 0;           // Disable DMA channel
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
  if (dma_->interrupt_status.teif2) {
    if (tx_listener_) {
      tx_listener_->on_tx_error();
    }
  } else {
    if (tx_listener_) {
      tx_listener_->on_tx_complete();
    }
  }
}

// ISR handler for the DMA peripheral responsible for reception.
// Calls on_rx_error and on_rx_complete functions of the rx_listener_
void UartDma::DMA_rx_interrupt_handler() {
  stop_rx();
  if (dma_->interrupt_status.teif3) {
    if (rx_listener_) {
      rx_listener_->on_rx_error(RxError::DMA);
    }
  } else {
    if (rx_listener_) {
      rx_listener_->on_rx_complete();
    }
  }
}

// TODO: These are declared in hal_stm32.cpp but implemented here, clean this up!

void DMA1Channel2ISR() {
  uart_dma.DMA_tx_interrupt_handler();
  DMA::get_register(DMA::Base::DMA1)->interrupt_clear.gif2 = 1;  // clear all channel 3 flags
}

void DMA1Channel3ISR() {
  uart_dma.DMA_rx_interrupt_handler();
  DMA::get_register(DMA::Base::DMA1)->interrupt_clear.gif3 = 1;  // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void Uart3ISR() { uart_dma.UART_interrupt_handler(); }

#endif
