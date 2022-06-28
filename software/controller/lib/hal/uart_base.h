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
 * [RM] Chapter 38 defines the USART function.
 *****************************************************************/

#include <algorithm>
#include <cstdint>
#include <optional>

#include "gpio.h"
#include "interrupts.h"
#include "proto_traits.h"
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
  // Note the use of default argument value in virtual function, which is only OK in case
  // overriding functions use the same default value.
  // NOLINTNEXTLINE(google-default-arguments)
  virtual size_t Read(uint8_t *buffer, size_t length, RxListener *rxl = nullptr) = 0;

  // Write length bytes to the tx buffer, and, ultimately, to the serial link.
  // This function does not block, so if there isn't enough space to write
  // length bytes, then only a partial write will occur.
  // The number of bytes actually written is returned.
  // Uses tx listener methods on interrupts (transfer complete, DMA error).
  // Note the use of default argument value in virtual function, which is only OK in case
  // overriding functions use the same default value.
  // NOLINTNEXTLINE(google-default-arguments)
  virtual size_t Write(uint8_t *buffer, size_t length, TxListener *txl = nullptr);

  // Return the number of bytes ready to be read.
  virtual size_t RxFull() const = 0;
  // Returns whether at least one byte has been received
  bool HasReceivedBytes() const { return RxFull() > 0; };

  // Returns the number of free locations in the transmit buffer.
  virtual size_t TxFree() const = 0;
  // Returns true if Tx buffer is full;
  bool CannotTransmit() const { return TxFree() == 0; };

  // Provide a way to stop a transfer and flush the corresponding buffer
  virtual void StopTx() = 0;
  virtual void StopRx() = 0;

  // Interrupt handlers for the UART channel.
  virtual void UARTInterruptHandler();

  // setter for Rx Listener
  void SetRxListener(RxListener *rxl);

  // tester of whether channel uses DMA
  virtual bool IsDMAChannel() { return false; };

 protected:
  Base uart_;

  // event listeners, which provide callbacks for certain events
  // (completion of a transfer, error during transfer and Rx character match)
  RxListener *rx_listener_{nullptr};
  TxListener *tx_listener_{nullptr};

  uint8_t match_char_{0};

  bool tx_in_progress_{false};

  // Length of Buffers, make sure they stay big enough to store data to/from the GUI.
  static constexpr size_t BufferLength{256};

  virtual void EnableTxInterrupt();
  virtual void EnableTxCompleteInterrupt();
  void ClearTxCompleteInterrupt();
  void DisableTxInterrupt();

  uint8_t GetRxReg();
  void PutTxReg(uint8_t byte);

  volatile uint32_t *GetRxAddress();
  volatile uint32_t *GetTxAddress();

  bool RxRegNotEmpty();
  bool TxRegEmpty();

  void EnableClock();
  void SetupPins(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin, std::optional<uint8_t> rts_pin,
                 std::optional<uint8_t> cts_pin, GPIO::AlternativeFunction alt_function);
  void SetupUARTRegisters(Frequency cpu_frequency, Frequency baud, bool dma_enable = false);

  void OnTxComplete();
  void OnCharacterMatch();
};

}  // namespace UART
