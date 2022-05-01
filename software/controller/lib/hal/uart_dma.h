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

#include "dma.h"
#include "serial_listeners.h"
#include "uart.h"

class UartDma {
 public:
#if !defined(BARE_STM32)
  UartDma() = default;
#endif
  UartDma(UartReg *const uart, char match_char) : uart_(uart), match_char_(match_char){};

  void initialize(Frequency cpu_frequency, Frequency baud, DMA::Base dma, DMA::Channel tx_channel,
                  DMA::Channel rx_channel);

  [[nodiscard]] bool start_tx(uint8_t *buffer, uint32_t length, TxListener *txl);
  [[nodiscard]] bool start_rx(uint8_t *buffer, uint32_t length, RxListener *rxl);

  bool tx_in_progress() const;
  bool rx_in_progress() const;

  void stop_tx();
  void stop_rx();

  uint32_t rx_bytes_left();

  void enable_character_match();

  void UART_interrupt_handler();
  void DMA_rx_interrupt_handler();
  void DMA_tx_interrupt_handler();

 private:
  UartReg *const uart_{nullptr};
  std::optional<DMA::ChannelControl> tx_dma_{std::nullopt};
  std::optional<DMA::ChannelControl> rx_dma_{std::nullopt};
  RxListener *rx_listener_{nullptr};
  TxListener *tx_listener_{nullptr};
  uint8_t match_char_;
  bool tx_in_progress_{false};
  bool rx_in_progress_{false};
};
