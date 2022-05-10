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

#pragma once

/******************************************************************
 * Serial ports to GUI and debug interface (virtual port over USB)
 * use uart.
 * [RM] Chapter 38 defines the USART registers.
 *****************************************************************/

#include <cstdint>

#include "circular_buffer.h"
#include "dma.h"
#include "gpio.h"
#include "serial_listeners.h"
#include "units.h"

namespace UART {

enum class Base { UART1, UART2, UART3, UART4 };

class Channel {
 public:
  Channel(Base base, uint8_t match_char = 0);
  // Create a channel with DMA enabled
  Channel(Base base, DMA::Base dma, uint8_t match_char = 0);

  // Setup all registers to manage the uart channel, with hardware flow control pins as an option
  void Initialize(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin, std::optional<uint8_t> cts_pin,
                  std::optional<uint8_t> rts_pin, GPIO::AlternativeFunction alt_function,
                  Frequency cpu_frequency, Frequency baud);

  // Read up to length bytes and store them in the passed buffer.
  // This function does not block, so if less then length bytes are available it
  // will only return the available bytes
  // Returns the number of bytes actually read.
  // In DMA mode, uses rx listener methods on interrupt.
  uint16_t Read(char *buffer, uint16_t length, RxListener *rxl = nullptr);

  // Write length bytes to the tx buffer, and, ultimately, to the serial link.
  // This function does not block, so if there isn't enough space to write
  // length bytes, then only a partial write will occur.
  // The number of bytes actually written is returned.
  // In DMA mode, uses rx listener methods on interrupts.
  uint16_t Write(const char *buffer, uint16_t length, TxListener *txl = nullptr);

  // Return the number of bytes currently in the receive buffer and ready to be read.
  uint16_t RxFull() const;

  // Returns the number of free locations in the transmit buffer.
  uint16_t TxFree() const;

  // Provide a way to stop a transfer and flush the corresponding buffer
  void StopTx();
  void StopRx();

  // Interrupt handlers for the UART and DMA channels.
  void UARTInterruptHandler();
  void TxDMAInterruptHandler();
  void RxDMAInterruptHandler();

 private:
  Base uart_;
  // Circular buffers, make sure they stay big enough to store data to/from the GUI.
  static constexpr size_t BufferLength{128};
  CircularBuffer<uint8_t, BufferLength> rx_data_;
  CircularBuffer<uint8_t, BufferLength> tx_data_;

  // Channel may or may not use DMA
  bool dma_enable_{false};
  std::optional<DMA::ChannelControl> tx_dma_{std::nullopt};
  std::optional<DMA::ChannelControl> rx_dma_{std::nullopt};
  uint8_t request_{0};  // request number for dma setup
  // Remember how many bytes are transferred by tx_dma in order to pop that many
  // bytes from the tx buffer when the transfer is finished
  uint16_t tx_dma_count_{0};

  // event listeners, which provide callbacks for certain events
  // (completion of a transfer, error during transfer and Rx character match)
  RxListener *rx_listener_{nullptr};
  TxListener *tx_listener_{nullptr};

  uint8_t match_char_{0};
};

}  // namespace UART
