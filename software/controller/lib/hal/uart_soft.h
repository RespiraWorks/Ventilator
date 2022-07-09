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

#include "circular_buffer.h"
#include "uart_base.h"

namespace UART {

class SoftChannel : public Channel {
 public:
  explicit SoftChannel(Base base, uint8_t match_char = 0) : Channel(base, match_char){};

  // Note the use of default argument value in override functions, which is only OK because
  // overriden virtual functions use the same default value.
  // NOLINTNEXTLINE(google-default-arguments)
  size_t Read(uint8_t *buffer, size_t length, RxListener *rxl = nullptr) override;
  // NOLINTNEXTLINE(google-default-arguments)
  size_t Write(uint8_t *buffer, size_t length, TxListener *txl = nullptr) override;

  // Return the number of bytes currently in the receive buffer and ready to be read.
  size_t RxFull() const override { return rx_data_.FullCount(); };
  // Returns the number of free locations in the transmit buffer.
  size_t TxFree() const override { return tx_data_.FreeCount(); };

  void StopTx() override { tx_data_.Flush(); };
  void StopRx() override { rx_data_.Flush(); };

  void UARTInterruptHandler() override;

 protected:
  // circular buffers for transmitting and receiving data
  CircularBuffer<uint8_t, BufferLength> tx_data_;
  CircularBuffer<uint8_t, BufferLength> rx_data_;
};

}  // namespace UART
