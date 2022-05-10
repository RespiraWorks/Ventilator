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

namespace UART {

// [RM] 38.8 USART Registers (pg 1238)
struct Registers {
  union {
    struct {
      uint32_t enable : 1;
      uint32_t stop_mode : 1;
      uint32_t rx_enable : 1;
      uint32_t tx_enable : 1;
      uint32_t idle_interrupt : 1;
      uint32_t rx_interrupt : 1;
      uint32_t tx_complete_interrupt : 1;
      uint32_t tx_interrupt : 1;
      uint32_t parity_error_interrupt : 1;
      uint32_t parity : 1;
      uint32_t parity_control_enable : 1;
      uint32_t rx_wakeup_method : 1;
      uint32_t word_length0 : 1;
      uint32_t mute_mode_enable : 1;
      uint32_t char_match_interrupt : 1;
      uint32_t oversampling_mode : 1;
      uint32_t driver_deassertion_time : 5;  // Driver Enable de-assertion time
      uint32_t driver_assertion_time : 5;    // Driver Enable assertion time
      uint32_t rx_timeout_interrupt : 1;
      uint32_t eob_interrupt : 1;  // End of Block interrupt enable
      uint32_t word_length1 : 1;
      uint32_t reserved : 3;
    } bitfield;
    uint32_t full_reg;
  } control_reg1;  // Control Register 1 (USART_CR1) [RM] 38.8.1 (pg 1238)
  union {
    struct {
      uint32_t reserved2 : 4;
      uint32_t address_detection_mode : 1;  // 7-bit | 4bit Address Detection
      uint32_t break_detection_length : 1;
      uint32_t break_detection_interrput : 1;
      uint32_t reserved1 : 1;
      uint32_t last_bit_clock_pulse : 1;
      uint32_t clock_phase : 1;
      uint32_t clock_polarity : 1;
      uint32_t clock_enable : 1;
      uint32_t stop_bits : 2;
      uint32_t lin_mode_enable : 1;
      uint32_t swap_rx_tx_pins : 1;
      uint32_t rx_inversion : 1;
      uint32_t tx_inversion : 1;
      uint32_t data_inversion : 1;
      uint32_t msb_first : 1;
      uint32_t auto_baudrate_enable : 1;
      uint32_t auto_baudrate_mode : 2;
      uint32_t rx_timeout_enable : 1;
      uint32_t addr : 8;  // used for character detection during normal
      // reception.  This bit field can only be written
      // when reception is disabled (RE = 0) or the
      // USART is disabled (UE=0)
    } bitfield;
    uint32_t full_reg;
  } control2;  // Control Register 2 (USART_CR2) [RM] 38.8.2 (pg 1241)
  union {
    struct {
      uint32_t error_interrupt : 1;
      uint32_t irda_mode : 1;
      uint32_t irda_low_power : 1;
      uint32_t half_duplex : 1;
      uint32_t smartcard_nack_enable : 1;
      uint32_t smartcard_mode : 1;
      uint32_t rx_dma : 1;
      uint32_t tx_dma : 1;
      uint32_t rts_enable : 1;
      uint32_t cts_enable : 1;
      uint32_t cts_interrupt : 1;
      uint32_t onebit : 1;  // One sample bit method enable
      uint32_t overrun_disable : 1;
      uint32_t dma_disable_on_rx_error : 1;
      uint32_t driver_enable_mode : 1;
      uint32_t driver_enable_polarity : 1;
      uint32_t reserved2 : 1;
      uint32_t smartcard_auto_retry_count : 3;
      uint32_t wakeup_stop_interrupt_select : 2;
      uint32_t wakeup_stop_interrupt_enable : 1;
      uint32_t uart_clock_stop_mode : 1;
      uint32_t tx_complete_before_guard_interrupt : 1;
      uint32_t reserved : 7;
    } bitfield;
    uint32_t full_reg;
  } control3;  // Control Register 3 [RM] 38.8.3  (pg 1245)
  uint32_t baudrate;
  uint32_t guard_time;
  union {
    struct {
      uint32_t rx_timeout : 24;
      uint32_t block_length : 8;
    } bitfield;
    uint32_t full_reg;
  } timeout;  // Receiver Timeout Register (USART_RTOR) [RM] 38.8.6 (pg 1251)
  union {
    struct {
      uint32_t auto_baudrate : 1;
      uint32_t send_break : 1;
      uint32_t mute_mode : 1;
      uint32_t flush_rx : 1;
      uint32_t flush_tx : 1;
      uint32_t reserved : 27;
    } bitfield;
    uint32_t full_reg;
  } request;  // Request Register (USART_QRQ) [RM] 38.8.7 (pg 1252)
  union {
    struct {
      const uint32_t parity_error : 1;
      const uint32_t framing_error : 1;
      const uint32_t noise_detection : 1;
      const uint32_t overrun_error : 1;
      const uint32_t idle : 1;
      const uint32_t rx_not_empty : 1;
      const uint32_t tx_complete : 1;
      const uint32_t tx_empty : 1;
      const uint32_t lin_break_detected : 1;
      const uint32_t cts_interrupt : 1;
      const uint32_t cts : 1;
      const uint32_t rx_timeout : 1;
      const uint32_t end_of_block : 1;
      const uint32_t reserved : 1;
      const uint32_t auto_baudrate_error : 1;
      const uint32_t auto_baudrate_flag : 1;
      const uint32_t busy : 1;
      const uint32_t char_match : 1;
      const uint32_t send_break : 1;
      const uint32_t rx_wakeup : 1;
      const uint32_t wakeup_from_stop : 1;
      const uint32_t tx_enable_ack : 1;
      const uint32_t rx_enable_ack : 1;
      const uint32_t reserved2 : 2;
      const uint32_t tx_complete_before_guard : 1;
      const uint32_t reserved3 : 6;
    } bitfield;
    uint32_t full_reg;
  } status;  // [RM] 38.8.8 Interrupt And Status Register (USART_ISR) (pg 1253)
  union {
    struct {
      uint32_t parity_error_clear : 1;
      uint32_t framing_error_clear : 1;
      uint32_t noise_detected_clear : 1;
      uint32_t overrun_clear : 1;
      uint32_t clear_idle : 1;
      uint32_t reserved : 1;
      uint32_t tx_complete_clear : 1;
      uint32_t tx_complete_before_guard_clear : 1;
      uint32_t lin_break_detected_clear : 1;
      uint32_t cts_clear : 1;
      uint32_t reserved2 : 1;
      uint32_t rx_timeout_clear : 1;
      uint32_t end_of_block_clear : 1;
      uint32_t reserved3 : 4;
      uint32_t char_match_clear : 1;
      uint32_t reserved4 : 2;
      uint32_t wakeup_from_stop_clear : 1;
      uint32_t reserved5 : 11;
    } bitfield;
    uint32_t full_reg;
  } interrupt_clear;  // [RM] 38.8.9 Interrupt Flag Clear Register (USART_ICR)
  // (pg 1257)
  uint32_t rx_data;
  uint32_t tx_data;
};
typedef volatile Registers UartReg;

// UART base registers, per [RM] table 2 (p68)
UartReg *get_register(const Base id) {
  switch (id) {
    case Base::UART1:
      return reinterpret_cast<UartReg *>(0x40013800);
    case Base::UART2:
      return reinterpret_cast<UartReg *>(0x40004400);
    case Base::UART3:
      return reinterpret_cast<UartReg *>(0x40004800);
    case Base::UART4:
      return reinterpret_cast<UartReg *>(0x40004C00);
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}

Channel::Channel(Base const base, uint8_t match_char) : uart_(base), match_char_(match_char){};
Channel::Channel(Base base, DMA::Base dma, uint8_t match_char)
    : uart_(base), match_char_(match_char) {
  // DMA mapping for UART (see [RM] p299)
  static struct {
    DMA::Base dma_base;
    Base uart_base;
    DMA::Channel tx_channel_id;
    DMA::Channel rx_channel_id;
    uint8_t request_number;
  } DmaMap[] = {
      {DMA::Base::DMA1, Base::UART1, DMA::Channel::Chan4, DMA::Channel::Chan5, 2},
      {DMA::Base::DMA1, Base::UART2, DMA::Channel::Chan7, DMA::Channel::Chan6, 2},
      {DMA::Base::DMA1, Base::UART3, DMA::Channel::Chan2, DMA::Channel::Chan3, 2},
      {DMA::Base::DMA2, Base::UART1, DMA::Channel::Chan6, DMA::Channel::Chan7, 2},
      {DMA::Base::DMA2, Base::UART4, DMA::Channel::Chan3, DMA::Channel::Chan5, 2},
  };

  for (auto &map : DmaMap) {
    if (dma == map.dma_base && uart_ == map.uart_base) {
      tx_dma_.emplace(dma, map.tx_channel_id);
      rx_dma_.emplace(dma, map.rx_channel_id);
      request_ = map.request_number;
      dma_enable_ = true;
      break;
    }
  }
};

void Channel::Initialize(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin,
                         std::optional<uint8_t> cts_pin, std::optional<uint8_t> rts_pin,
                         GPIO::AlternativeFunction alt_function, Frequency cpu_frequency,
                         Frequency baud) {
  // Enable clock and interrupts
  switch (uart_) {
    case Base::UART1:
      enable_peripheral_clock(PeripheralID::USART1);
      Interrupts::singleton().EnableInterrupt(InterruptVector::Uart1, InterruptPriority::Standard);
      break;
    case Base::UART2:
      enable_peripheral_clock(PeripheralID::USART2);
      Interrupts::singleton().EnableInterrupt(InterruptVector::Uart2, InterruptPriority::Standard);
      break;
    case Base::UART3:
      enable_peripheral_clock(PeripheralID::USART3);
      Interrupts::singleton().EnableInterrupt(InterruptVector::Uart3, InterruptPriority::Standard);
      break;
    case Base::UART4:
      enable_peripheral_clock(PeripheralID::UART4);
      Interrupts::singleton().EnableInterrupt(InterruptVector::Uart4, InterruptPriority::Standard);
      break;
      // All cases covered above (and GCC checks this).
      __builtin_unreachable();
  }
  // Set Tx/Rx pins
  GPIO::AlternatePin(port, tx_pin, alt_function);
  GPIO::AlternatePin(port, rx_pin, alt_function);

  // Set hardware flow control pins (when provided)
  if (cts_pin.has_value()) GPIO::AlternatePin(port, *cts_pin, alt_function);
  if (rts_pin.has_value()) GPIO::AlternatePin(port, *rts_pin, alt_function);

  // Set baud rate register
  UartReg *uart = get_register(uart_);
  uart->baudrate = static_cast<uint32_t>(cpu_frequency / baud);

  // Set control register
  uart->control_reg1.bitfield.rx_interrupt = 1;  // enable receive interrupt
  uart->control_reg1.bitfield.tx_enable = 1;     // enable transmitter
  uart->control_reg1.bitfield.rx_enable = 1;     // enable receiver
  uart->control_reg1.bitfield.enable = 1;        // enable uart
}

// Read up to length bytes and store them in the passed buffer.
// This function does not block, so if less then len bytes
// are available it will only return the available bytes
// Returns the number of bytes actually read.
uint16_t Channel::Read(char *buffer, uint16_t length, RxListener *rxl) {
  rx_listener_ = rxl;
  for (uint16_t i = 0; i < length; i++) {
    std::optional<uint8_t> ch = rx_data_.Get();
    if (ch == std::nullopt) {
      return i;
    }
    *buffer++ = *ch;
  }

  // Note that we don't need to enable the rx interrupt
  // here.  That one is always enabled.
  return length;
}

// Write up to len bytes to the buffer.
// This function does not block, so if there isn't enough
// space to write len bytes, then only a partial write
// will occur.
// The number of bytes actually written is returned.
uint16_t Channel::Write(const char *buffer, uint16_t length, TxListener *txl) {
  uint16_t i;
  for (i = 0; i < length; i++) {
    if (!tx_data_.Put(*buffer++)) break;
  }

  tx_listener_ = txl;
  // Enable the tx interrupt.  If there was already anything
  // in the buffer this will already be enabled, but enabling
  // it again doesn't hurt anything.
  get_register(uart_)->control_reg1.bitfield.tx_interrupt = 1;

  return i;
}

// Return the number of bytes currently in the receive buffer and ready to be read.
uint16_t Channel::RxFull() const { return static_cast<uint16_t>(rx_data_.FullCount()); }

// Returns the number of free locations in the transmit buffer.
uint16_t Channel::TxFree() const { return static_cast<uint16_t>(tx_data_.FreeCount()); }

// Provide a way to stop a transfer and flush the corresponding buffer
void Channel::StopTx(){};
void Channel::StopRx(){};

// This is the interrupt handler for the UART.
void Channel::UARTInterruptHandler() {
  UartReg *uart = get_register(uart_);
  // Check for overrun error and framing errors.  Clear those errors if
  // they're set to avoid further interrupts from them.
  if (uart->status.bitfield.framing_error) {
    uart->interrupt_clear.bitfield.framing_error_clear = 1;
  }
  if (uart->status.bitfield.overrun_error) {
    uart->interrupt_clear.bitfield.overrun_clear = 1;
  }

  // See if we received a new byte.
  if (uart->status.bitfield.rx_not_empty) {
    // Add the byte to rx_data_.  If the buffer is full, we'll drop it --
    // what else can we do?
    //
    // TODO: Perhaps log a warning here so we have an idea of whether
    // this buffer is hitting capacity frequently.
    (void)rx_data_.Put(static_cast<uint8_t>(uart->rx_data));
  }

  // Check for transmit data register empty
  if (uart->status.bitfield.tx_empty && uart->control_reg1.bitfield.tx_interrupt) {
    std::optional<uint8_t> ch = tx_data_.Get();

    // If there's nothing left in the transmit buffer,
    // just disable further transmit interrupts.
    if (ch == std::nullopt) {
      uart->control_reg1.bitfield.tx_interrupt = 0;
    } else {
      // Otherwise, Send the next byte.
      uart->tx_data = *ch;
    }
  }
}

}  // namespace UART
