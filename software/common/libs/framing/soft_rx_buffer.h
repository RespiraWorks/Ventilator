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

#include "serial_listeners.h"
#include <cstdint>

template <int RxBytesMax> class SoftRxBuffer {
  uint8_t rx_buf_[RxBytesMax];
  uint32_t rx_i_ = 0;
  RxListener *rx_listener_;
  uint8_t match_char_ = 0;

public:
  explicit SoftRxBuffer(uint8_t match_char) : match_char_(match_char){};
  void RestartRX(RxListener *listener) {
    rx_i_ = 0;
    rx_listener_ = listener;
  }

  bool Begin(RxListener *listener) {
    RestartRX(listener);
    return true;
  };

  uint32_t ReceivedLength() { return rx_i_; };

  uint8_t *get() { return rx_buf_; }

  void PutByte(uint8_t b) {
    if (rx_i_ < RxBytesMax) {
      rx_buf_[rx_i_++] = b;
      if (match_char_ == b) {
        if (rx_listener_) {
          rx_listener_->OnCharacterMatch();
        }
      }
    }
    if (rx_i_ >= RxBytesMax) {
      if (rx_listener_) {
        rx_listener_->OnRxComplete();
      }
    }
  }
};
