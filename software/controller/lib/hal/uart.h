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

// \TODO: abstract this better so we don't have to expose registers

#pragma once

/******************************************************************
 * Serial port to GUI
 * [RM] Chapter 38 defines the USART registers.
 *****************************************************************/

#include <cstdint>

#include "circular_buffer.h"
#include "units.h"

// [RM] 38.8 USART Registers (pg 1238)
struct UartStruct {
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
typedef volatile UartStruct UartReg;
inline UartReg *const Uart1Base = reinterpret_cast<UartReg *>(0x40013800);
inline UartReg *const Uart2Base = reinterpret_cast<UartReg *>(0x40004400);
inline UartReg *const Uart3Base = reinterpret_cast<UartReg *>(0x40004800);
inline UartReg *const Uart4Base = reinterpret_cast<UartReg *>(0x40004C00);

class UART {
 public:
  explicit UART(UartReg *r);

  void Init(Frequency cpu_frequency, Frequency baud);

  // This is the interrupt handler for the UART.
  void ISR();

  // Read up to len bytes and store them in the passed buffer.
  // This function does not block, so if less then len bytes
  // are available it will only return the available bytes
  // Returns the number of bytes actually read.
  uint16_t Read(char *buf, uint16_t len);

  // Write up to len bytes to the buffer.
  // This function does not block, so if there isn't enough
  // space to write len bytes, then only a partial write
  // will occur.
  // The number of bytes actually written is returned.
  uint16_t Write(const char *buf, uint16_t len);

  // Return the number of bytes currently in the
  // receive buffer and ready to be read.
  uint16_t RxFull();

  // Returns the number of free locations in the
  // transmit buffer.
  uint16_t TxFree();

 private:
  CircularBuffer<uint8_t, 128> rx_data_;
  CircularBuffer<uint8_t, 128> tx_data_;
  UartReg *const uart_;
};
