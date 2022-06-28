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

#include "rx_buffer.h"
#include "uart_base.h"

// RX buffer wrapper that controls restart of reception and provides count of received bytes.
// We need this class to abstract hardware access from FrameDetector FSM, so it can be used in GUI
template <size_t RxBytesMax>
class UartRxBuffer : public RxBuffer {
 public:
  explicit UartRxBuffer(UART::Channel *uart);

  bool begin(RxListener *rxl) override;
  void restart_rx(RxListener *rxl) override;
  size_t received_length() const override;
  uint8_t *get() override;

  /// Using protected so that derived class with back-door can be used for testing
 protected:
  // Uart provides reception to this buffer
  UART::Channel *uart_;
  // Buffer into which the data is received
  uint8_t rx_buffer_[RxBytesMax] = {0};

  // Method that starts the reception
  void start_rx(RxListener *rxl);
};

#include "uart_rx_buffer.tpp"
