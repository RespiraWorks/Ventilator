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

// todo: replace with pragma once
#ifndef __HAL_TRANSPORT
#include "network_protocol.pb.h"
#include "uart_dma.h"

// RX buffer wrapper that controls restart of reception and provides count of
// received bytes.
// We need this class to abstract hardware access from FrameDetector FSM so it
// can be used in GUI code
template <int RxBytesMax> class RxBufferUartDma {
public:
  explicit RxBufferUartDma(UART_DMA &uart_dma) : uart_dma_(uart_dma) {}

  // Sets up underlying receive infrastructure and starts the first reception
  [[nodiscard]] bool Begin(RxListener *rxl) {
    uart_dma_.charMatchEnable();
    return uart_dma_.startRX(rx_buffer_, RxBytesMax, rxl);
  }

  // Restarts the ongoing reception, this means the rx_buf will be written from
  // the beginning
  void RestartRX(RxListener *rxl) {
    uart_dma_.stopRX();
    [[maybe_unused]] bool started =
        uart_dma_.startRX(rx_buffer_, RxBytesMax, rxl);
  }

  // Returns how many bytes were written into rx_buf
  uint32_t ReceivedLength() {
    return (RxBytesMax - uart_dma_.getRxBytesLeft());
  }

  // Returns the rx_buf
  uint8_t *get() { return rx_buffer_; }

#ifdef TEST_MODE
  // Puts a byte to rx_buf
  void test_PutByte(uint8_t b);
#endif

private:
  // UART_DMA provides receiving to this buffer
  UART_DMA &uart_dma_;
  // Buffer into which the data is received
  uint8_t rx_buffer_[RxBytesMax];
};

#ifdef TEST_MODE
extern uint32_t rx_i;
// Puts a byte to rx_buf
template <int RxBytesMax>
void RxBufferUartDma<RxBytesMax>::test_PutByte(uint8_t b) {
  rx_buffer_[rx_i++] = b;
}
#endif

#endif
