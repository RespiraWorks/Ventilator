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

// This driver also provides Character Match callback on match_char reception.

extern UartDma uart_dma;

// Performs UART3 initialization
void UartDma::Init(uint32_t baud) {
  baud_ = baud;
  // Set baud rate register
  uart_->baudrate = CPU_FREQ / baud;

  uart_->control3.bitfield.rx_dma = 1;         // set DMAR bit to enable DMA for receiver
  uart_->control3.bitfield.tx_dma = 1;         // set DMAT bit to enable DMA for transmitter
  uart_->control3.bitfield.dma_disable_on_rx_error = 1;  // DMA is disabled following a reception error
  uart_->control2.bitfield.rx_timeout_enable = 1;        // Enable receive timeout feature
  uart_->control2.bitfield.addr = match_char_; // set match char

  uart_->control3.bitfield.error_interrupt = 1; // enable interrupt on error

  uart_->request.bitfield.flush_rx = 1; // Clear RXNE flag before clearing other flags
  
  // Clear error flags.
  uart_->interrupt_clear.bitfield.framing_error_clear = 1;
  uart_->interrupt_clear.bitfield.overrun_clear = 1;
  uart_->interrupt_clear.bitfield.rx_timeout_clear = 1;

  uart_->control_reg1.bitfield.tx_enable = 1; // enable transmitter
  uart_->control_reg1.bitfield.rx_enable = 1; // Enable receiver
  uart_->control_reg1.bitfield.enable = 1;    // enable uart

  // TODO enable parity checking?

  dma_->channel[rx_channel_].config.priority = 0b11; // high priority
  dma_->channel[rx_channel_].config.tx_error_interrupt = 1;        // interrupt on error
  dma_->channel[rx_channel_].config.half_tx_interrupt = 0;        // no half-transfer interrupt
  dma_->channel[rx_channel_].config.tx_complete_interrupt = 1;        // interrupt on DMA complete

  dma_->channel[rx_channel_].config.mem2mem = 0; // memory-to-memory mode disabled
  dma_->channel[rx_channel_].config.memory_size = static_cast<uint32_t>(DmaTransferSize::Byte);
  dma_->channel[rx_channel_].config.peripheral_size = static_cast<uint32_t>(DmaTransferSize::Byte);
  dma_->channel[rx_channel_].config.memory_increment = 1;   // increment destination (memory)
  dma_->channel[rx_channel_].config.peripheral_increment = 0;   // don't increment source
                                          // (peripheral) address
  dma_->channel[rx_channel_].config.circular = 0; // not circular
  dma_->channel[rx_channel_].config.direction =
      static_cast<uint32_t>(DmaChannelDir::PeripheralToMemory);

  dma_->channel[tx_channel_].config.priority = 0b11; // high priority
  dma_->channel[tx_channel_].config.tx_error_interrupt = 1;        // interrupt on error
  dma_->channel[tx_channel_].config.half_tx_interrupt = 0;        // no half-transfer interrupt
  dma_->channel[tx_channel_].config.tx_complete_interrupt = 1;        // DMA complete interrupt enabled

  dma_->channel[tx_channel_].config.mem2mem = 0; // memory-to-memory mode disabled
  dma_->channel[tx_channel_].config.memory_size = static_cast<uint32_t>(DmaTransferSize::Byte);
  dma_->channel[tx_channel_].config.peripheral_size = static_cast<uint32_t>(DmaTransferSize::Byte);
  dma_->channel[tx_channel_].config.memory_increment = 1;   // increment source (memory) address
  dma_->channel[tx_channel_].config.peripheral_increment = 0;   // don't increment dest (peripheral)
                                          // address
  dma_->channel[tx_channel_].config.circular = 0; // not circular
  dma_->channel[tx_channel_].config.direction =
      static_cast<uint32_t>(DmaChannelDir::MemoryToPeripheral);
}

// Sets up an interrupt on matching char incoming form UART3
void UartDma::CharMatchEnable() {
  uart_->interrupt_clear.bitfield.char_match_clear = 1; // Clear char match flag
  uart_->control_reg1.bitfield.char_match_interrupt = 1; // Enable character match interrupt
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
bool UartDma::StartTX(uint8_t *buf, uint32_t length, TxListener *txl) {
  if (TxInProgress()) {
    return false;
  }

  tx_listener_ = txl;

  dma_->channel[tx_channel_].config.enable = 0; // Disable channel before config
  // data sink
  dma_->channel[tx_channel_].peripheral_address = &(uart_->tx_data);
  // data source
  dma_->channel[tx_channel_].memory_address =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma_->channel[tx_channel_].count = length & 0x0000FFFF;

  dma_->channel[tx_channel_].config.enable = 1; // go!

  tx_in_progress_ = true;

  return true;
}

void UartDma::StopTX() {
  if (TxInProgress()) {
    // Disable DMA channel
    dma_->channel[tx_channel_].config.enable = 0;
    // TODO thread safety
    tx_in_progress_ = 0;
  }
}

// Converts Duration to the number of baudrate bits as that is used for rx
// timeout. Max timeout supported by STM32 UART is 24 bit
UartDma::uint24_t UartDma::DurationToBits(Duration d) {
  return {0,
          static_cast<uint32_t>(d.milliseconds() *
          static_cast<float>(baud_) / 1000.0) & 0x00FFFFFF};
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

bool UartDma::StartRX(uint8_t *buf, uint32_t length, Duration timeout,
                       RxListener *rxl) {
  // UART3 reception happens on DMA1 channel 3
  if (RxInProgress()) {
    return false;
  }

  rx_listener_ = rxl;

  dma_->channel[rx_channel_].config.enable = 0; // don't enable yet

  // data source
  dma_->channel[rx_channel_].peripheral_address = &(uart_->rx_data);
  // data sink
  dma_->channel[rx_channel_].memory_address =
      const_cast<void *>(reinterpret_cast<const void *>(buf));
  // data length
  dma_->channel[rx_channel_].count = length;

  // setup rx timeout
  uart_->timeout.bitfield.rx_timeout = DurationToBits(timeout).bits;
  uart_->interrupt_clear.bitfield.rx_timeout_clear = 1; // Clear rx timeout flag
  uart_->request.bitfield.flush_rx = 1;  // Clear RXNE flag
  uart_->control_reg1.bitfield.rx_timeout_interrupt = 1;    // Enable receive timeout interrupt

  dma_->channel[rx_channel_].config.enable = 1; // go!

  rx_in_progress_ = true;

  return true;
}

uint32_t UartDma::GetRxBytesLeft() { return dma_->channel[2].count; }

void UartDma::StopRX() {
  if (RxInProgress()) {
    uart_->control_reg1.bitfield.rx_timeout_interrupt = 0;           // Disable receive timeout interrupt
    dma_->channel[rx_channel_].config.enable = 0; // Disable DMA channel
    // TODO thread safety
    rx_in_progress_ = false;
  }
}

static bool CharacterMatchInterrupt() {
  return Uart3Base->status.bitfield.char_match != 0;
}

static bool GetRxTimeout() {
  // Timeout interrupt enable and RTOF - Receiver timeout
  return Uart3Base->control_reg1.bitfield.rx_timeout_interrupt &&
  Uart3Base->status.bitfield.rx_timeout;
}

static bool GetRxError() {
  return GetRxTimeout() ||
  Uart3Base->status.bitfield.overrun_error || // overrun error
  Uart3Base->status.bitfield.framing_error;   // frame error

  // TODO(miceuz): Enable these?
  // UART3_BASE->status.s.pe || // parity error
  // UART3_BASE->status.s.nf || // START bit noise detection flag
}

// ISR handler for the UART peripheral.
// Calls OnRxError and OnCharacterMatch functions of the rxListener as those
// events are provided by UART peripheral. OnRxComplete is called by DMA ISR
void UartDma::UartISR() {
  if (GetRxError()) {
    RxError e = RxError::Unknown;
    if (GetRxTimeout()) {
      e = RxError::Timeout;
    }
    if (uart_->status.bitfield.overrun_error) {
      e = RxError::Overflow;
    }
    if (uart_->status.bitfield.framing_error) {
      e = RxError::SerialFraming;
    }

    uart_->request.bitfield.flush_rx = 1; // Clear RXNE flag before clearing other flags

    // Clear error flags.
    uart_->interrupt_clear.bitfield.framing_error_clear = 1;
    uart_->interrupt_clear.bitfield.overrun_clear = 1;
    uart_->interrupt_clear.bitfield.rx_timeout_clear = 1;

    if (rx_listener_) {
      rx_listener_->OnRxError(e);
    }
    return;
  }

  if (CharacterMatchInterrupt()) {
    uart_->request.bitfield.flush_rx = 1; // Clear RXNE flag before clearing other flags
    uart_->interrupt_clear.bitfield.char_match_clear = 1; // Clear char match flag

    if (rx_listener_) {
      rx_listener_->OnCharacterMatch();
    }
  }
}

// ISR handler for the DMA peripheral responsible for transmission.
// Calls OnTxError and OnTxComplete functions of the tx_listener_
void UartDma::DmaTxISR() {
  if (dma_->interrupt_status.teif2) {
    StopTX();
    if (tx_listener_) {
      tx_listener_->OnTxError();
    }
  } else {
    StopTX();
    if (tx_listener_) {
      tx_listener_->OnTxComplete();
    }
  }
}

// ISR handler for the DMA peripheral responsible for reception.
// Calls OnRxError and OnRxComplete functions of the rx_listener_
void UartDma::DmaRxISR() {
  if (dma_->interrupt_status.teif3) {
    StopRX();
    if (rx_listener_) {
      rx_listener_->OnRxError(RxError::DMA);
    }
  } else {
    StopRX();
    if (rx_listener_) {
      rx_listener_->OnRxComplete();
    }
  }
}

// TODO: These are declared in hal_stm32.cpp but implemented here, clean this up!

void DMA1Channel2ISR() {
  DmaReg *dma = Dma1Base;
  uart_dma.DmaTxISR();
  dma->interrupt_clear.gif2 = 1; // clear all channel 3 flags
}

void DMA1Channel3ISR() {
  DmaReg *dma = Dma1Base;
  uart_dma.DmaRxISR();
  dma->interrupt_clear.gif3 = 1; // clear all channel 2 flags
}

// This is the interrupt handler for the UART.
void Uart3ISR() { uart_dma.UartISR(); }

#endif
