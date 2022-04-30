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

////////////////////////////////////////////////////////////////////////////////
/// WARNING: Do not include this file as header, nor compile it as a source! ///
///                                                                          ///
/// This is a template implementation file.                                  ///
/// It is included by the corresponding header.                              ///
////////////////////////////////////////////////////////////////////////////////

template <size_t RxBytesMax>
UartRxBuffer<RxBytesMax>::UartRxBuffer(UART::Channel *uart) : uart_(uart) {}

// Sets up underlying receive infrastructure and starts the first reception
template <size_t RxBytesMax>
bool UartRxBuffer<RxBytesMax>::begin(RxListener *rxl) {
  uart_->EnableCharacterMatch();
  restart_rx(rxl);
  return true;
}

// Restarts the ongoing reception, this means the buffer will be written from the beginning
template <size_t RxBytesMax>
void UartRxBuffer<RxBytesMax>::restart_rx(RxListener *rxl) {
  uart_->StopRx();
  // for a DMA channel, setup reception, for non-DMA channel, only set rx_listener_
  if(uart_->IsDMAChannel()) {
    start_rx(rxl);
  } else {
    uart_->SetRxListener(rxl);
  }
}

// Returns how many bytes were written into rx_buf
template <size_t RxBytesMax>
size_t UartRxBuffer<RxBytesMax>::received_length() const {
  return (uart_->RxFull());
}

// Returns the rx_buffer_
template <size_t RxBytesMax>
uint8_t *UartRxBuffer<RxBytesMax>::get() {
  // Outside of DMA channel, we need to transfer the uart_->rx_data_ buffer to the rx_buffer_
  if(!uart_->IsDMAChannel())
    start_rx(nullptr);
  return rx_buffer_;
}

// Starts the reception itself
template <size_t RxBytesMax>
void UartRxBuffer<RxBytesMax>::start_rx(RxListener *rxl){
  uart_->Read(rx_buffer_, RxBytesMax, rxl);
}
