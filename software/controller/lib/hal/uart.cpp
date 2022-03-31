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

#include "uart.h"

UART::UART(UartReg* const r) : uart_(r) {}

void UART::Init(Frequency cpu_frequency, Frequency baud) {
  // Set baud rate register
  uart_->baudrate = static_cast<uint32_t>(cpu_frequency / baud);

  uart_->control_reg1.bitfield.rx_interrupt = 1;  // enable receive interrupt
  uart_->control_reg1.bitfield.tx_enable = 1;     // enable transmitter
  uart_->control_reg1.bitfield.rx_enable = 1;     // enable receiver
  uart_->control_reg1.bitfield.enable = 1;        // enable uart
}

// This is the interrupt handler for the UART.
void UART::ISR() {
  // Check for overrun error and framing errors.  Clear those errors if
  // they're set to avoid further interrupts from them.
  if (uart_->status.bitfield.framing_error) {
    uart_->interrupt_clear.bitfield.framing_error_clear = 1;
  }
  if (uart_->status.bitfield.overrun_error) {
    uart_->interrupt_clear.bitfield.overrun_clear = 1;
  }

  // See if we received a new byte.
  if (uart_->status.bitfield.rx_not_empty) {
    // Add the byte to rx_data_.  If the buffer is full, we'll drop it --
    // what else can we do?
    //
    // TODO: Perhaps log a warning here so we have an idea of whether
    // this buffer is hitting capacity frequently.
    (void)rx_data_.Put(static_cast<uint8_t>(uart_->rx_data));
  }

  // Check for transmit data register empty
  if (uart_->status.bitfield.tx_empty && uart_->control_reg1.bitfield.tx_interrupt) {
    std::optional<uint8_t> ch = tx_data_.Get();

    // If there's nothing left in the transmit buffer,
    // just disable further transmit interrupts.
    if (ch == std::nullopt) {
      uart_->control_reg1.bitfield.tx_interrupt = 0;
    } else {
      // Otherwise, Send the next byte.
      uart_->tx_data = *ch;
    }
  }
}

// Read up to len bytes and store them in the passed buffer.
// This function does not block, so if less then len bytes
// are available it will only return the available bytes
// Returns the number of bytes actually read.
uint16_t UART::Read(char* buf, uint16_t len) {
  for (uint16_t i = 0; i < len; i++) {
    std::optional<uint8_t> ch = rx_data_.Get();
    if (ch == std::nullopt) {
      return i;
    }
    *buf++ = *ch;
  }

  // Note that we don't need to enable the rx interrupt
  // here.  That one is always enabled.
  return len;
}

// Write up to len bytes to the buffer.
// This function does not block, so if there isn't enough
// space to write len bytes, then only a partial write
// will occur.
// The number of bytes actually written is returned.
uint16_t UART::Write(const char* buf, uint16_t len) {
  uint16_t i;
  for (i = 0; i < len; i++) {
    if (!tx_data_.Put(*buf++)) break;
  }

  // Enable the tx interrupt.  If there was already anything
  // in the buffer this will already be enabled, but enabling
  // it again doesn't hurt anything.
  uart_->control_reg1.bitfield.tx_interrupt = 1;
  return i;
}

// Return the number of bytes currently in the
// receive buffer and ready to be read.
uint16_t UART::RxFull() { return static_cast<uint16_t>(rx_data_.FullCount()); }

// Returns the number of free locations in the
// transmit buffer.
uint16_t UART::TxFree() { return static_cast<uint16_t>(tx_data_.FreeCount()); }
