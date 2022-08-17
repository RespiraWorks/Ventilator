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

#include "uart_soft.h"

namespace UART {

class MockChannel : public SoftChannel {
 public:
  explicit MockChannel(uint8_t match_char = 0)
      : SoftChannel(Base::UART1, "mock_uart", "for mock uart", match_char){};

  bool PutRxByte(const uint8_t byte) {
    if (!rx_data_.Put(byte)) {
      return false;
    }
    if (byte == match_char_) {
      OnCharacterMatch();
    }
    return true;
  }

  std::optional<uint8_t> GetTxByte() {
    uint8_t byte{0};
    if (GetTxData(&byte, 1) == 1) {
      return byte;
    }
    return std::nullopt;
  }

  size_t PutRxData(const uint8_t *buffer, size_t length) {
    size_t i;
    for (i = 0; i < length; i++) {
      if (!PutRxByte(*buffer++)) {
        break;
      }
    }
    return i;
  };

  size_t GetTxData(uint8_t *buffer, size_t length) {
    size_t i;
    for (i = 0; i < length; i++) {
      std::optional<uint8_t> ch = tx_data_.Get();
      if (ch == std::nullopt) {
        break;
      }
      *buffer++ = *ch;
    }
    if (tx_data_.FullCount() == 0) {
      Channel::OnTxComplete();
    }
    return i;
  };

  void EnableCharacterMatch() override{};

  size_t GetBufferLength() { return BufferLength; }

  bool TxInProgress() { return tx_in_progress_; };

 private:
  void EnableTxInterrupt() override{};
  void EnableTxCompleteInterrupt() override{};
};

}  // namespace UART
