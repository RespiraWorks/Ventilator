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

////////////////////////////////////////////////////////////////////////////////
/// WARNING: Do not include this file as header, nor compile it as a source! ///
///                                                                          ///
/// This is a template implementation file.                                  ///
/// It is included by the corresponding header.                              ///
////////////////////////////////////////////////////////////////////////////////

template <size_t RxBytesMax>
RxBufferUartDma<RxBytesMax>::RxBufferUartDma(UartDma &uart_dma)
: uart_dma_(uart_dma)
{}

// Sets up underlying receive infrastructure and starts the first reception
template <size_t RxBytesMax>
[[nodiscard]] bool RxBufferUartDma<RxBytesMax>::begin(RxListener *rxl) {
  uart_dma_.enable_character_match();
  return uart_dma_.start_rx(rx_buffer_, RxBytesMax, rxl);
}

// Restarts the ongoing reception, this means the rx_buffer_ will be written from the beginning
template <size_t RxBytesMax>
void RxBufferUartDma<RxBytesMax>::restart_rx(RxListener *rxl) {
  uart_dma_.stop_rx();
  [[maybe_unused]] bool started = uart_dma_.start_rx(rx_buffer_, RxBytesMax, rxl);
}

// Returns how many bytes were written into rx_buf
template <size_t RxBytesMax>
size_t RxBufferUartDma<RxBytesMax>::received_length() const {
  return (RxBytesMax - uart_dma_.rx_bytes_left());
}

// Returns the rx_buffer_
template <size_t RxBytesMax>
const uint8_t *RxBufferUartDma<RxBytesMax>::get() const {
  return rx_buffer_;
}

#if !defined(BARE_STM32)
extern size_t rx_index;
// Puts a byte to rx_buffer_
template <size_t RxBytesMax>
void RxBufferUartDma<RxBytesMax>::test_put_byte(const uint8_t b) {
  rx_buffer_[rx_index++] = b;
}
#endif
