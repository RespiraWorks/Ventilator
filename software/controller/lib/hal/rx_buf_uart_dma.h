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

#pragma once

#include "network_protocol.pb.h"
#include "rx_buffer.h"
#include "uart_dma.h"

/// \TODO: This class is not (explicitly) tested, but it could be?

// RX buffer wrapper that controls restart of reception and provides count of received bytes.
// We need this class to abstract hardware access from FrameDetector FSM, so it can be used in GUI
template <size_t RxBytesMax>
class RxBufferUartDma : public RxBuffer {
 public:
  explicit RxBufferUartDma(UartDma &uart_dma);

  [[nodiscard]] bool begin(RxListener *rxl) override;
  void restart_rx(RxListener *rxl) override;
  size_t received_length() const override;
  const uint8_t *get() const override;

  /// \TODO find a better way to mock this without ifdefs
#if !defined(BARE_STM32)
  // Puts a byte to rx_buffer_
  void test_put_byte(const uint8_t b);
#endif

 private:
  // UartDma provides receiving to this buffer
  UartDma &uart_dma_;
  // Buffer into which the data is received
  uint8_t rx_buffer_[RxBytesMax] = {0};
};

#include "rx_buf_uart_dma.tpp"
