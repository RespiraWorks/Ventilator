/* Copyright 2022, RespiraWorks
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

#include "uart_soft.h"

namespace UART {

// Read up to length bytes and store them in the passed buffer.
// This function does not block, so if less then len bytes
// are available it will only return the available bytes
// Returns the number of bytes actually read.
size_t SoftChannel::Read(uint8_t *buffer, size_t length, RxListener *rxl) {
  if (rxl) {
    SetRxListener(rxl);
  }

  for (size_t i = 0; i < length; i++) {
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

size_t SoftChannel::Write(uint8_t *buffer, size_t length, TxListener *txl) {
  size_t i;
  for (i = 0; i < length; i++) {
    if (!tx_data_.Put(*buffer++)) {
      break;
    }
  }
  EnableTxInterrupt();
  Channel::Write(buffer, length, txl);
  return i;
}

void SoftChannel::UARTInterruptHandler() {
  // Process common interrupts
  Channel::UARTInterruptHandler();

  // See if we received a new byte.
  if (RxRegNotEmpty()) {
    // Add the byte to rx_data_.  If the buffer is full, we'll drop it --
    // what else can we do?
    //
    // TODO: Perhaps log a warning here so we have an idea of whether
    // this buffer is hitting capacity frequently.
    (void)rx_data_.Put(static_cast<uint8_t>(*GetRxAddress()));
  }

  // Check for transmit data register empty
  if (TxRegEmpty()) {
    std::optional<uint8_t> ch = tx_data_.Get();

    // If there's nothing left in the transmit buffer,
    // just disable further transmit interrupts, and run callback.
    if (ch == std::nullopt) {
      DisableTxInterrupt();
    } else {
      // Otherwise, send the next byte.
      *GetTxAddress() = *ch;
    }
  }
}

}  // namespace UART
