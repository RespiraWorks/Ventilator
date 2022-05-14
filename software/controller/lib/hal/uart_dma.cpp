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

#include "uart_dma.h"

#include <string.h>  // used for memcpy

#include "dma.h"

// Declaration of UART channels, as global since hal requires it to exist for interrupt handlers
// TODO: find a way to get rid of these global variable (probably requires a more flexible
// handling of InterruptVector in Reset_Handler() function from hal_stm32.cpp)
#if defined(UART_VIA_DMA)
UART::DMAChannel rpi_uart(UART::Base::UART3, DMA::Base::DMA1);
#endif

namespace UART {

DMAChannel::DMAChannel(Base base, DMA::Base dma, uint8_t match_char) : Channel(base, match_char) {
  // DMA mapping for UART (see [RM] p299)
  static struct {
    DMA::Base dma_base;
    Base uart_base;
    DMA::Channel tx_channel_id;
    DMA::Channel rx_channel_id;
    uint8_t request_number;
  } DmaMap[] = {
      {DMA::Base::DMA1, Base::UART1, DMA::Channel::Chan4, DMA::Channel::Chan5, 2},
      {DMA::Base::DMA1, Base::UART2, DMA::Channel::Chan7, DMA::Channel::Chan6, 2},
      {DMA::Base::DMA1, Base::UART3, DMA::Channel::Chan2, DMA::Channel::Chan3, 2},
      {DMA::Base::DMA2, Base::UART1, DMA::Channel::Chan6, DMA::Channel::Chan7, 2},
      {DMA::Base::DMA2, Base::UART4, DMA::Channel::Chan3, DMA::Channel::Chan5, 2},
  };

  for (auto &map : DmaMap) {
    if (dma == map.dma_base && uart_ == map.uart_base) {
      tx_dma_.emplace(dma, map.tx_channel_id);
      rx_dma_.emplace(dma, map.rx_channel_id);
      request_ = map.request_number;
      break;
    }
  }
}

void DMAChannel::Initialize(GPIO::Port port, uint8_t tx_pin, uint8_t rx_pin,
                            std::optional<uint8_t> rts_pin, std::optional<uint8_t> cts_pin,
                            GPIO::AlternativeFunction alt_function, Frequency cpu_frequency,
                            Frequency baud) {
  EnableClock();
  SetupPins(port, tx_pin, rx_pin, rts_pin, cts_pin, alt_function);
  // Initialize DMA channels
  rx_dma_->Initialize(request_, GetRxAddress(), DMA::ChannelDir::PeripheralToMemory,
                      /*tx_interrupt=*/true, DMA::ChannelPriority::Highest);
  tx_dma_->Initialize(request_, GetTxAddress(), DMA::ChannelDir::MemoryToPeripheral,
                      /*tx_interrupt=*/true, DMA::ChannelPriority::Highest);
  SetupUARTRegisters(cpu_frequency, baud, /*dma_enable=*/true);
}

size_t DMAChannel::Read(uint8_t *buffer, size_t length, RxListener *rxl) {
  // DMA transfers cannot interrupt each other
  if (rx_in_progress_) return 0;

  rx_listener_ = rxl;
  // setup DMA transfer
  rx_dma_->SetupTransfer(buffer, length);
  rx_dma_->Enable();
  rx_in_progress_ = true;
  // though the transfer may take some time, inform the caller that he will
  // eventually get all of his data (when RxFinished() returns true)
  return length;
};

size_t DMAChannel::Write(uint8_t *buffer, size_t length, TxListener *txl) {
  // DMA transfers cannot interrupt each other
  if (tx_in_progress_) return 0;

  if (length > BufferLength) {
    // TODO: log an error, this is not supposed to happen, though it should be fine
    length = BufferLength;
  }

  tx_listener_ = txl;

  memcpy(tx_data_, buffer, length);
  // Setup DMA transfer
  tx_dma_->SetupTransfer(tx_data_, length);
  // clear UART Transfer Complete flag, per [RM] p1230
  ClearTxCompleteInterrupt();
  tx_dma_->Enable();
  tx_in_progress_ = true;
  return length;
}

void DMAChannel::TxDMAInterruptHandler() {
  tx_dma_->Disable();
  tx_in_progress_ = false;
  if (tx_dma_->InterruptStatus(DMA::Interrupt::TransferError)) {
    tx_dma_->ClearInterrupt(DMA::Interrupt::TransferError);
    if (tx_listener_) tx_listener_->on_tx_error();
  }
  if (tx_dma_->InterruptStatus(DMA::Interrupt::TransferComplete)) {
    tx_dma_->ClearInterrupt(DMA::Interrupt::TransferComplete);
    if (tx_listener_) tx_listener_->on_tx_complete();
  }
}

void DMAChannel::RxDMAInterruptHandler() {
  rx_dma_->Disable();
  rx_in_progress_ = false;
  if (rx_dma_->InterruptStatus(DMA::Interrupt::TransferError)) {
    rx_dma_->ClearInterrupt(DMA::Interrupt::TransferError);
    if (rx_listener_) rx_listener_->on_rx_error(RxError::DMA);
  };

  if (rx_dma_->InterruptStatus(DMA::Interrupt::TransferComplete)) {
    rx_dma_->ClearInterrupt(DMA::Interrupt::TransferComplete);
    if (rx_listener_) rx_listener_->on_rx_complete();
  }
}

}  // namespace UART
