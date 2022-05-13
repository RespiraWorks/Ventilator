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
  explicit Channel(Base base, uint8_t match_char = 0) : uart_(base), match_char_(match_char){};

  // Setup all registers to manage the uart channel, with hardware flow control pins as an option
  virtual void Initialize(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin,
                          std::optional<uint8_t> rts_pin, std::optional<uint8_t> cts_pin,
                          GPIO::AlternativeFunction alt_function, Frequency cpu_frequency,
                          Frequency baud);

  // Setup registers to enable character match
  virtual void EnableCharacterMatch();

  // Read up to length bytes and store them in the passed buffer.
  // This function does not block, so if less then length bytes are available it
  // will only return the available bytes
  // In normal (SW) mode, returns the number of bytes actually read.
  // In DMA mode, this returns 0 on error, or length, but it only sets up the transfer and
  // uses rx listener methods on interrupts (transfer complete, transfer error, character match).
  // This means that you cannot use or reset buffer immediately after calling Read in DMA mode.
  virtual size_t Read(uint8_t *buffer, size_t length, RxListener *rxl = nullptr) = 0;

  // Write length bytes to the tx buffer, and, ultimately, to the serial link.
  // This function does not block, so if there isn't enough space to write
  // length bytes, then only a partial write will occur.
  // The number of bytes actually written is returned.
  // In DMA mode, uses tx listener methods on interrupts.
  virtual size_t Write(uint8_t *buffer, size_t length, TxListener *txl = nullptr) = 0;

  // Return the number of bytes currently in the receive buffer and ready to be read.
  virtual size_t RxFull() const = 0;
  // Returns the number of free locations in the transmit buffer.
  virtual size_t TxFree() const = 0;

  // Provide a way to stop a transfer and flush the corresponding buffer
  virtual void StopTx() = 0;
  virtual void StopRx() = 0;

  // Interrupt handlers for the UART channel.
  virtual void UARTInterruptHandler();

 protected:
  Base uart_;

  // event listeners, which provide callbacks for certain events
  // (completion of a transfer, error during transfer and Rx character match)
  RxListener *rx_listener_{nullptr};
  TxListener *tx_listener_{nullptr};

  uint8_t match_char_{0};

  // Length of Buffers, make sure they stay big enough to store data to/from the GUI.
  static constexpr size_t BufferLength{128};

  virtual void EnableTxInterrupt();
  virtual void EnableTxCompleteInterrupt();

  void EnableClock();
  void SetupPins(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin, std::optional<uint8_t> rts_pin,
                 std::optional<uint8_t> cts_pin, GPIO::AlternativeFunction alt_function);
  void SetupUARTRegisters(Frequency cpu_frequency, Frequency baud, bool dma_enable = false);
};

class SoftChannel : public Channel {
 public:
  explicit SoftChannel(Base base, uint8_t match_char = 0) : Channel(base, match_char){};

  size_t Read(uint8_t *buffer, size_t length, RxListener *rxl = nullptr) override;
  size_t Write(uint8_t *buffer, size_t length, TxListener *txl = nullptr) override;
  // Return the number of bytes currently in the receive buffer and ready to be read.
  size_t RxFull() const override { return rx_data_.FullCount(); };
  // Returns the number of free locations in the transmit buffer.
  size_t TxFree() const override { return tx_data_.FreeCount(); };

  void StopTx() override { tx_data_.Flush(); };
  void StopRx() override { rx_data_.Flush(); };

  void UARTInterruptHandler();

 protected:
  // circular buffers for transmitting and receiving data
  CircularBuffer<uint8_t, BufferLength> tx_data_;
  CircularBuffer<uint8_t, BufferLength> rx_data_;
};

class DMAChannel : public Channel {
 public:
  // Create a channel with DMA enabled
  DMAChannel(Base base, DMA::Base dma, uint8_t match_char = 0);

  void Initialize(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin, std::optional<uint8_t> rts_pin,
                  std::optional<uint8_t> cts_pin, GPIO::AlternativeFunction alt_function,
                  Frequency cpu_frequency, Frequency baud);

  size_t Read(uint8_t *buffer, size_t length, RxListener *rxl = nullptr) override;
  size_t Write(uint8_t *buffer, size_t length, TxListener *txl = nullptr) override;

  // DMA doesn't have an rx buffer, RxFull doesn't make much sense.
  size_t RxFull() const override { return 0; };
  // Returns 0 if transfer is in progress (sending is then impossible) and BufferLength otherwise
  size_t TxFree() const override { return tx_in_progress_ ? 0 : BufferLength; };

  void StopTx() override { tx_dma_->Disable(); };
  void StopRx() override { rx_dma_->Disable(); };

  void TxDMAInterruptHandler();
  void RxDMAInterruptHandler();

 private:
  std::optional<DMA::ChannelControl> tx_dma_{std::nullopt};
  std::optional<DMA::ChannelControl> rx_dma_{std::nullopt};
  uint8_t request_{0};  // request number for dma setup

  // Store Tx data in our own buffer, in case caller overwrites its data before DMA has finished
  // transmitting it all
  uint8_t tx_data_[BufferLength] = {0};

  bool tx_in_progress_{false};
  bool rx_in_progress_{false};
};

}  // namespace UART

#if defined(UART_VIA_DMA)
extern UART::DMAChannel rpi_uart;
#else
extern UART::SoftChannel rpi_uart;
#endif
extern UART::SoftChannel debug_uart;
