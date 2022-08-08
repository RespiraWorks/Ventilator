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

#pragma once

#include "spi_chain.h"

namespace SPI {

class MockChannel : public Channel {
 public:
  MockChannel() = default;

  void SetupReception(uint8_t *receive_buffer, size_t length) override {
    dma_rx_buffer_ = receive_buffer;
    dma_buffer_length_ = length;
    dma_buffer_index_ = 0;
    TransferRxQueueToRxBuffer();
  };

  void TransferRxQueueToRxBuffer() {
    for (size_t i = dma_buffer_index_; i < dma_buffer_length_; i++) {
      if (rx_index_ >= rx_count_) {
        return;
      }
      TestReceiveByte(rx_queue_[rx_index_++]);
    }
    if (interrupts_enabled_) {
      RxDMAInterruptHandler();
    }
  };

  void SendCommand(uint8_t *send_buffer, size_t length, bool clear_chip_select) override {
    for (size_t i = 0; i < length; i++) {
      if (tx_flow_ && tx_flow_count_ < tx_flow_length_) {
        tx_flow_[tx_flow_count_++] = send_buffer[i];
      } else {
        // The remaining of the send data will be lost.
        break;
      }
    }
    if (interrupts_enabled_) {
      TxDMAInterruptHandler();
    }
  };

  void SetChipSelect() override{};
  void ClearChipSelect() override{};

  void TxDMAInterruptHandler() override {
    if (tx_listener_) {
      tx_listener_->on_tx_complete();
    };
  };

  void RxDMAInterruptHandler() override {
    if (rx_listener_) {
      rx_listener_->on_rx_complete();
    };
  };

  void WaitResponse() override{};

  void EnableInterrupts() { interrupts_enabled_ = true; }

  // The way this mock class works is by putting all outbound data into a client(tester)-provided
  // array.
  // This method must be called prior to SendCommand, and any time the provided buffer is full so
  // the data can be sent.
  void TestSetSpiFlowTarget(uint8_t *buffer, size_t buffer_length) {
    tx_flow_ = buffer;
    tx_flow_length_ = buffer_length;
    tx_flow_count_ = 0;
  }

  // Queue data that will be copied to the receive buffer is called
  void TestQueueReceiveData(uint8_t *rx_data, size_t length) {
    rx_queue_ = rx_data;
    rx_count_ = length;
    rx_index_ = 0;
    TransferRxQueueToRxBuffer();
  }

  // Put a single byte in the (client-provided) receive buffer.
  // Returns false if it there is no space in the buffer.
  bool TestReceiveByte(uint8_t rx_byte) {
    if (dma_buffer_index_ >= dma_buffer_length_) {
      return false;
    }
    dma_rx_buffer_[dma_buffer_index_++] = rx_byte;
    return true;
  }

 private:
  bool interrupts_enabled_{false};

  // Buffer where receive data can be queued, which will get transfered to the client when
  // SetupReception is called.
  uint8_t *rx_queue_{nullptr};
  size_t rx_count_{0};
  size_t rx_index_{0};

  // In case the client hadn't queued enough data, keep track of his receive buffer outside
  // SetupReception to be able to keep putting data there.
  uint8_t *dma_rx_buffer_{nullptr};
  size_t dma_buffer_length_{0};
  size_t dma_buffer_index_{0};

  // Buffer where we export the transmit flow as we transmit it.
  uint8_t *tx_flow_{nullptr};
  size_t tx_flow_length_{0};
  size_t tx_flow_count_{0};
};

}  // namespace SPI
