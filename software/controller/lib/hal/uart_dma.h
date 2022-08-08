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

#include "dma.h"
#include "uart_base.h"

namespace UART {
class DMAChannel : public Channel {
 public:
  // Create a channel with DMA enabled
  DMAChannel(Base base, DMA::Base dma, const char *name, const char *help_supplement,
             uint8_t match_char = 0);

  void Initialize(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin, std::optional<uint8_t> rts_pin,
                  std::optional<uint8_t> cts_pin, GPIO::AlternativeFunction alt_function,
                  Frequency cpu_frequency, Frequency baud) override;

  // Note the use of default argument value in override functions, which is only OK because
  // overriden virtual functions use the same default value.
  // NOLINTNEXTLINE(google-default-arguments)
  size_t Read(uint8_t *buffer, size_t length, RxListener *rxl = nullptr) override;
  // NOLINTNEXTLINE(google-default-arguments)
  size_t Write(uint8_t *buffer, size_t length, TxListener *txl = nullptr) override;

  size_t RxFull() const override;
  size_t TxFree() const override;

  void StopTx() override;
  void StopRx() override;

  bool IsDMAChannel() override { return true; };

  void TxDMAInterruptHandler();
  void RxDMAInterruptHandler();

 private:
  std::optional<DMA::ChannelControl> tx_dma_{std::nullopt};
  std::optional<DMA::ChannelControl> rx_dma_{std::nullopt};
  uint8_t request_{0};  // request number for dma setup

  // Store Tx data in our own buffer, in case caller overwrites its data before DMA has finished
  // transmitting it all
  uint8_t tx_data_[BufferLength] = {0};

  // Store last Rx length to inform caller about how many bytes we put in the buffer
  size_t rx_length_{0};

  bool rx_in_progress_{false};

  Debug::Variable::UInt32 dma_errors_{"uart_dma_errors", Debug::Variable::Access::ReadOnly, 0, "",
                                      "Counter of DMA error on the UART line."};
};

}  // namespace UART
