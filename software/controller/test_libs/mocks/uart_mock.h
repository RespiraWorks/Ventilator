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

#include "uart.h"

namespace UART {

class MockChannel : public Channel {
 public:
  MockChannel() : Channel(Base::UART1){};
  explicit MockChannel(char match_char) : Channel(Base::UART1, match_char){};

  bool PutRxByte(uint8_t byte) { return rx_data_.Put(byte); }

  std::optional<uint8_t> GetTxByte() { return tx_data_.Get(); }

  uint16_t PutRxData(uint8_t *buffer, uint16_t length) {
    uint16_t i;
    for (i = 0; i < length; i++) {
      if (!rx_data_.Put(*buffer++)) break;
    }
    return i;
  };

  uint16_t GetTxData(uint8_t *buffer, uint16_t length) {
    uint16_t i;
    for (i = 0; i < length; i++) {
      std::optional<uint8_t> ch = tx_data_.Get();
      if (ch == std::nullopt) return i;
      *buffer++ = *ch;
    }
    return i;
  };

  void EnableCharacterMatch() override{};

  size_t GetBufferLength() { return BufferLength; }

 private:
  void EnableTxInterrupt() override{};
  void EnableTxCompleteInterrupt() override{};
};

}  // namespace UART
