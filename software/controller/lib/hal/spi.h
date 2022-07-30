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

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t

#include "serial_listeners.h"

namespace SPI {

class Channel {
 public:
  Channel() = default;

  virtual void SetChipSelect() = 0;
  virtual void ClearChipSelect() = 0;

  virtual void SetupReception(uint8_t *receive_buffer, size_t length) = 0;
  virtual void SendCommand(uint8_t *send_buffer, size_t length, bool clear_chip_select) = 0;

  // Interrupt handlers for DMA transfer complete
  virtual void TxDMAInterruptHandler() = 0;
  virtual void RxDMAInterruptHandler() = 0;

  virtual void WaitResponse() = 0;

  void SetRxListener(RxListener *rxl) { rx_listener_ = rxl; };

  void SetTxListener(TxListener *txl) { tx_listener_ = txl; };

 protected:
  // Listeners for transfer complete
  RxListener *rx_listener_{nullptr};
  TxListener *tx_listener_{nullptr};
};

}  // namespace SPI
